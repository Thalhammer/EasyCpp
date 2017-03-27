#include "MySQLDatabase.h"
#include <StringAlgorithm.h>
#include <VFS/Path.h>
#include <Database/DatabaseException.h>
#include "MySQLStatement.h"

using EasyCpp::Database::DatabaseException;

namespace EasyCppMySql
{
	thread_local MySQLHandle::ThreadInitializer MySQLHandle::initializer;

	bool MySQLDatabase::beginTransaction()
	{
		return _hdl->executeThreadSafe<bool>([](MySQLHandle::HandleAccessor& hdl) {
			auto res = mysql_autocommit(hdl.getHandle(), 0);
			if (res == 0) {
				hdl.setInTransaction(true);
			}
			return res == 0;
		});
	}

	bool MySQLDatabase::commit()
	{
		return _hdl->executeThreadSafe<bool>([](MySQLHandle::HandleAccessor& hdl) {
			auto res = mysql_commit(hdl.getHandle());
			if (res == 0) {
				hdl.setInTransaction(false);
			}
			return res == 0;
		});
	}

	bool MySQLDatabase::rollBack()
	{
		return _hdl->executeThreadSafe<bool>([](MySQLHandle::HandleAccessor& hdl) {
			auto res = mysql_rollback(hdl.getHandle());
			if (res == 0) {
				hdl.setInTransaction(false);
			}
			return res == 0;
		});
	}

	bool MySQLDatabase::inTransaction()
	{
		return _hdl->executeThreadSafe<bool>([](MySQLHandle::HandleAccessor& hdl) {
			return hdl.getInTransaction();
		});
	}

	std::string MySQLDatabase::errorCode()
	{
		return _hdl->executeThreadSafe<std::string>([](MySQLHandle::HandleAccessor& hdl) {
			return std::to_string(mysql_errno(hdl.getHandle()));
		});
	}

	EasyCpp::Bundle MySQLDatabase::errorInfo()
	{
		return EasyCpp::Bundle();
	}

	uint64_t MySQLDatabase::exec(const std::string & sql)
	{
		auto stmt = this->prepare(sql);
		return stmt->execute();
	}

	EasyCpp::AnyValue MySQLDatabase::getAttribute(const std::string & attribute)
	{
		throw std::out_of_range("attribute not found");
	}

	bool MySQLDatabase::setAttribute(const std::string & attribute, const EasyCpp::AnyValue & value)
	{
		return false;
	}

	std::string MySQLDatabase::lastInsertedId(const std::string & name)
	{
		return _hdl->executeThreadSafe<std::string>([](MySQLHandle::HandleAccessor& hdl) {
			return std::to_string(mysql_insert_id(hdl.getHandle()));
		});
	}

	EasyCpp::Database::StatementPtr MySQLDatabase::prepare(const std::string & sql, const EasyCpp::Bundle & driver_options)
	{
		return std::make_shared<MySQLStatement>(sql, _hdl, _unloadp);
	}

	EasyCppMySql::MySQLDatabase::MySQLDatabase(const std::string& dsn, const EasyCpp::Bundle& options, std::shared_ptr<void> unloadp)
		:_unloadp(unloadp)
	{
		std::string idsn = dsn;
		EasyCpp::Bundle ioptions = options;
		if (idsn.length() >= 6 && idsn.substr(0, 6) == "mysql:")
			idsn = idsn.substr(6);

		for(auto& part: EasyCpp::stringSplit(";", idsn))
		{
			auto parts = EasyCpp::stringSplit("=", part);
			if (parts.size() == 2) {
				if (ioptions.isSet(parts[0]))
					throw DatabaseException("Duplicated option " + parts[0], {});

				if (parts[0] == "host") {
					ioptions.set(parts[0], parts[1]);
				} else if (parts[0] == "port") {
					std::string sport = EasyCpp::trim(parts[1]);
					size_t pos = -1;
					int num = std::stoi(parts[1], &pos);
					if (pos != sport.size() || num < 0 || num > UINT16_MAX)
						throw DatabaseException("Not a valid port: " + sport, {});
					ioptions.set(parts[0], num);
				} else if (parts[0] == "dbname") {
					ioptions.set(parts[0], parts[1]);
				} else if (parts[0] == "unix_socket") {
					ioptions.set(parts[0], parts[1]);
				} else if (parts[0] == "charset") {
					ioptions.set(parts[0], parts[1]);
				}
			}
		}
		if (ioptions.isSet("unix_socket") && (ioptions.isSet("host") || ioptions.isSet("port"))) {
			throw DatabaseException("Only specify host or unix_socket", {});
		}
		
		_hdl = std::make_shared<MySQLHandle>();
		
		
		_hdl->executeThreadSafe<void>([this,&ioptions](MySQLHandle::HandleAccessor& hdl) {
			if (ioptions.isSet("autoreconnect") && ioptions["autoreconnect"].as<bool>()) {
				my_bool auto_reconnect = true;
				if (mysql_options(hdl.getHandle(), MYSQL_OPT_RECONNECT, &auto_reconnect))
					throw DatabaseException("Failed to set options", EasyCpp::Bundle({ { std::string("option"), "autoreconnect" } }));
			}

			if (mysql_real_connect(hdl.getHandle(),
				ioptions.isSet("host") ? ioptions["host"].as<std::string>().c_str() : nullptr,
				ioptions.isSet("user") ? ioptions["user"].as<std::string>().c_str() : nullptr,
				ioptions.isSet("pass") ? ioptions["pass"].as<std::string>().c_str() : nullptr,
				ioptions.isSet("dbname") ? ioptions["dbname"].as<std::string>().c_str() : nullptr,
				ioptions.isSet("port") ? ioptions["port"].as<unsigned int>() : 0,
				ioptions.isSet("unix_socket") ? ioptions["unix_socket"].as<std::string>().c_str() : nullptr,
				0) == nullptr)
			{
				unsigned int error = mysql_errno(hdl.getHandle());
				const char* msg = mysql_error(hdl.getHandle());
				throw DatabaseException("Failed to connect to database", EasyCpp::Bundle({
					{ "code", error },
					{ "msg", std::string(msg) }
				}));
			}
			if (ioptions.isSet("charset")) {
				if (mysql_set_character_set(hdl.getHandle(), ioptions["charset"].as<std::string>().c_str()))
					throw DatabaseException(std::string("Failed to set options."), EasyCpp::Bundle({
						{ "option", "charset" },
						{ "charset", ioptions["charset"].as<std::string>() }
				}));
			}
		});
	}

	MySQLDatabase::~MySQLDatabase()
	{
	}
}