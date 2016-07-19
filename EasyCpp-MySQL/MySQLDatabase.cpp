#include "MySQLDatabase.h"
#include <StringAlgorithm.h>
#include <VFS/Path.h>
#include <Database/DatabaseException.h>

using EasyCpp::Database::DatabaseException;

namespace EasyCppMySql
{

	bool MySQLDatabase::beginTransaction()
	{
		auto res = mysql_autocommit(_hdl.get(), 0);
		if (res == 0) {
			*_in_transaction = true;
		}
		return res == 0;
	}

	bool MySQLDatabase::commit()
	{
		auto res = mysql_commit(_hdl.get());
		if (res == 0) {
			*_in_transaction = false;
		}
		return res == 0;
	}

	bool MySQLDatabase::rollBack()
	{
		auto res = mysql_rollback(_hdl.get());
		if (res == 0) {
			*_in_transaction = false;
		}
		return res == 0;
	}

	bool MySQLDatabase::inTransaction()
	{
		return *_in_transaction;
	}

	std::string MySQLDatabase::errorCode()
	{
		return std::to_string(mysql_errno(_hdl.get()));
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
		return std::to_string(mysql_insert_id(_hdl.get()));
	}

	EasyCpp::Database::StatementPtr MySQLDatabase::prepare(const std::string & sql, const EasyCpp::Bundle & driver_options)
	{
		return EasyCpp::Database::StatementPtr();
	}

	EasyCppMySql::MySQLDatabase::MySQLDatabase(const std::string& dsn, const EasyCpp::Bundle& options)
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
		
		_hdl = std::shared_ptr<MYSQL>(mysql_init(nullptr), mysql_close);
		if (!_hdl) {
			throw DatabaseException("Failed to init mysql handle", {});
		}
		if (ioptions.isSet("autoreconnect") && ioptions["autoreconnect"].as<bool>()) {
			my_bool auto_reconnect = true;
			if (mysql_options(_hdl.get(), MYSQL_OPT_RECONNECT, &auto_reconnect))
				throw DatabaseException("Failed to set options", EasyCpp::Bundle({ { std::string("option"), "autoreconnect"} }));
		}
		if (mysql_real_connect(_hdl.get(),
			ioptions.isSet("host") ? ioptions["host"].as<std::string>().c_str() : nullptr,
			ioptions.isSet("user") ? ioptions["user"].as<std::string>().c_str() : nullptr,
			ioptions.isSet("pass") ? ioptions["pass"].as<std::string>().c_str() : nullptr,
			ioptions.isSet("dbname") ? ioptions["dbname"].as<std::string>().c_str() : nullptr, 
			ioptions.isSet("port") ? ioptions["port"].as<unsigned int>() : 0,
			ioptions.isSet("unix_socket") ? ioptions["unix_socket"].as<std::string>().c_str() : nullptr,
			0) == nullptr)
		{
			unsigned int error = mysql_errno(_hdl.get());
			const char* msg = mysql_error(_hdl.get());
			throw DatabaseException("Failed to connect to database",EasyCpp::Bundle({
				{"code", error},
				{"msg", std::string(msg)}
			}));
		}
		if (ioptions.isSet("charset")) {
			if (mysql_set_character_set(_hdl.get(), ioptions["charset"].as<std::string>().c_str()))
				throw DatabaseException(std::string("Failed to set options."), EasyCpp::Bundle({
					{"option", "charset"},
					{"charset", ioptions["charset"].as<std::string>() }
				}));
		}
	}

	MySQLDatabase::~MySQLDatabase()
	{
	}
}