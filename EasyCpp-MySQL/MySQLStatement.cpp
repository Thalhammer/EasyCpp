#include "MySQLStatement.h"
#include <climits>

using namespace EasyCpp;
using namespace EasyCpp::Database;

namespace EasyCppMySql
{

	MySQLStatement::MySQLStatement(const std::string& sql, std::shared_ptr<MySQLHandle> hdl)
		:_stmt(nullptr), _param_bind(nullptr), _hdl(hdl)
	{
		_hdl->executeThreadSafe<void>([this, sql](MySQLHandle::HandleAccessor& hdl) {
			_stmt = mysql_stmt_init(hdl.getHandle());
			if (_stmt == nullptr)
				throw DatabaseException("Failed to initialize statement", {});

			if (sql.length() > ULONG_MAX)
				throw DatabaseException("SQL Statement too long", {});

			// Try to prepare statement
			if (mysql_stmt_prepare(_stmt, sql.c_str(), (unsigned long)sql.length()) != 0)
			{
				throw DatabaseException("Failed to prepare statement", {});
			}

			// Initialize parameter binds
			this->_param_count = mysql_stmt_param_count(_stmt);
			if (_param_count != 0)
			{
				this->_param_bind = (MYSQL_BIND*)malloc(sizeof(MYSQL_BIND)*_param_count);
				memset(_param_bind, 0, sizeof(MYSQL_BIND)*_param_count);
				for (unsigned long i = 0; i < _param_count; i++)
					this->setBind(i, MYSQL_TYPE_NULL, nullptr, 0, false);
			}
		});
	}

	MySQLStatement::~MySQLStatement()
	{

		mysql_stmt_close(_stmt);

		// Free param bind
		if (_param_bind != nullptr) {
			for (unsigned long i = 0; i < _param_count; i++)
			{
				free(_param_bind[i].buffer);
			}
			free(_param_bind);
		}
	}

	uint64_t MySQLStatement::execute()
	{
		return _hdl->executeThreadSafe<uint64_t>([this](MySQLHandle::HandleAccessor& hdl) {
			if (mysql_stmt_bind_param(_stmt, _param_bind) != 0)
				throw DatabaseException("Failed to bind parameters", {});
			if (mysql_stmt_execute(_stmt) != 0)
				throw DatabaseException("Failed to execute statement: " + std::to_string(mysql_stmt_errno(_stmt)) + " " + mysql_stmt_error(_stmt), {});
			auto rows = mysql_stmt_affected_rows(_stmt);

			// Skip all other results
			do {
				mysql_stmt_free_result(_stmt);
			} while (mysql_stmt_next_result(_stmt) == 0);

			if (mysql_stmt_reset(_stmt) != 0)
				throw DatabaseException("Failed to reset statement: " + std::to_string(mysql_stmt_errno(_stmt)) + " " + mysql_stmt_error(_stmt), {});

			return rows;
		});
	}

	AnyValue MySQLStatement::executeScalar()
	{
		return _hdl->executeThreadSafe<AnyValue>([this](MySQLHandle::HandleAccessor& hdl) {
			if (mysql_stmt_bind_param(_stmt, _param_bind) != 0)
				throw DatabaseException("Failed to bind parameters", {});
			if (mysql_stmt_execute(_stmt) != 0)
				throw DatabaseException("Failed to execute statement: " + std::to_string(mysql_stmt_errno(_stmt)) + " " + mysql_stmt_error(_stmt), {});

			auto del = [](MYSQL_RES* res) { mysql_free_result(res); };
			std::unique_ptr<MYSQL_RES, decltype(del)> meta(mysql_stmt_result_metadata(_stmt), del);
			auto bind = getBind(meta.get());

			if (mysql_stmt_bind_result(_stmt, bind.second.get()) != 0)
				throw DatabaseException("Failed to bind buffer", {});

			auto err = mysql_stmt_fetch(_stmt);
			if (err == 1)
				throw DatabaseException("Failed to fetch column:" + std::string(mysql_stmt_error(_stmt)), {});

			if (err == MYSQL_NO_DATA)
				throw DatabaseException("Query returned no rows", {});
			AnyValue res;
			MYSQL_BIND* resbind = bind.second.get();
			if (resbind->buffer == nullptr)
			{
				resbind->buffer = malloc(*(resbind->length));
				resbind->buffer_length = *(resbind->length);
				if (mysql_stmt_fetch_column(_stmt, resbind, 0, 0) != 0)
				{
					free(resbind->buffer);
					resbind->buffer = 0;
					resbind->buffer_length = 0;
					throw DatabaseException("Failed to retrieve result", {});
				}
				res = bind2Result(resbind);
				free(resbind->buffer);
				resbind->buffer = 0;
				resbind->buffer_length = 0;
			}
			else
				res = bind2Result(resbind);

			// Skip all other results
			do {
				mysql_stmt_free_result(_stmt);
			} while (mysql_stmt_next_result(_stmt) == 0);

			if (mysql_stmt_reset(_stmt) != 0)
				throw DatabaseException("Failed to reset statement: " + std::to_string(mysql_stmt_errno(_stmt)) + " " + mysql_stmt_error(_stmt), {});

			return res;
		});
	}

	Bundle MySQLStatement::executeQueryRow()
	{
		return _hdl->executeThreadSafe<Bundle>([this](MySQLHandle::HandleAccessor& hdl) {
			if (mysql_stmt_bind_param(_stmt, _param_bind) != 0)
				throw DatabaseException("Failed to bind parameters", {});
			if (mysql_stmt_execute(_stmt) != 0)
				throw DatabaseException("Failed to execute statement: " + std::to_string(mysql_stmt_errno(_stmt)) + " " + mysql_stmt_error(_stmt), {});

			auto del = [](MYSQL_RES* res) { mysql_free_result(res); };
			std::unique_ptr<MYSQL_RES, decltype(del)> meta(mysql_stmt_result_metadata(_stmt), del);

			auto bind = getBind(meta.get());

			if (mysql_stmt_bind_result(_stmt, bind.second.get()) != 0)
				throw DatabaseException("Failed to bind buffer", {});

			Bundle values;

			int res = mysql_stmt_fetch(_stmt);
			if (res == MYSQL_NO_DATA)
				throw DatabaseException("No row returned", {});
			if (res == 1)
				throw DatabaseException("Failed to retrieve result", {});

			MYSQL_BIND* resbind = bind.second.get();
			for (unsigned int i = 0; i < bind.first.size(); i++)
			{
				MYSQL_BIND* ptr = &resbind[i];
				if (ptr->buffer == nullptr)
				{
					ptr->buffer = malloc(*(ptr->length));
					ptr->buffer_length = *(ptr->length);
					if (mysql_stmt_fetch_column(_stmt, ptr, i, 0) != 0)
					{
						free(ptr->buffer);
						ptr->buffer = 0;
						ptr->buffer_length = 0;
						throw DatabaseException("Failed to retrieve result", {});
					}
					values.set(bind.first[i], bind2Result(ptr));
					free(ptr->buffer);
					ptr->buffer = 0;
					ptr->buffer_length = 0;
				}
				else {
					values.set(bind.first[i], bind2Result(ptr));
				}
			}

			// Skip all other results
			do {
				mysql_stmt_free_result(_stmt);
			} while (mysql_stmt_next_result(_stmt) == 0);

			if (mysql_stmt_reset(_stmt) != 0)
				throw DatabaseException("Failed to reset statement: " + std::to_string(mysql_stmt_errno(_stmt)) + " " + mysql_stmt_error(_stmt), {});

			return values;
		});
	}

	ResultSet MySQLStatement::executeQuery()
	{
		return _hdl->executeThreadSafe<ResultSet>([this](MySQLHandle::HandleAccessor& hdl) {
			if (mysql_stmt_bind_param(_stmt, _param_bind) != 0)
				throw DatabaseException("Failed to bind parameters", {});
			if (mysql_stmt_execute(_stmt) != 0)
				throw DatabaseException("Failed to execute statement: " + std::to_string(mysql_stmt_errno(_stmt)) + " " + mysql_stmt_error(_stmt), {});

			auto del = [](MYSQL_RES* res) { mysql_free_result(res); };
			std::unique_ptr<MYSQL_RES, decltype(del)> meta(mysql_stmt_result_metadata(_stmt), del);

			auto bind = getBind(meta.get());

			if (mysql_stmt_bind_result(_stmt, bind.second.get()) != 0)
				throw DatabaseException("Failed to bind buffer", {});

			MYSQL_BIND* resbind = bind.second.get();
			ResultSet result;
			while (mysql_stmt_fetch(_stmt) != MYSQL_NO_DATA)
			{
				Bundle values;
				for (unsigned int i = 0; i < bind.first.size(); i++)
				{
					MYSQL_BIND* ptr = &resbind[i];
					if (ptr->buffer == nullptr)
					{
						ptr->buffer = malloc(*(ptr->length));
						ptr->buffer_length = *(ptr->length);
						if (mysql_stmt_fetch_column(_stmt, ptr, i, 0) != 0)
						{
							free(ptr->buffer);
							ptr->buffer = 0;
							ptr->buffer_length = 0;
							throw DatabaseException("Failed to retrieve result", {});
						}
						values.set(bind.first[i], bind2Result(ptr));
						free(ptr->buffer);
						ptr->buffer = 0;
						ptr->buffer_length = 0;
					}
					else {
						values.set(bind.first[i], bind2Result(ptr));
					}
				}
				result.appendRow(values);
			}

			// Skip all other results
			do {
				mysql_stmt_free_result(_stmt);
			} while (mysql_stmt_next_result(_stmt) == 0);

			if (mysql_stmt_reset(_stmt) != 0)
				throw DatabaseException("Failed to reset statement: " + std::to_string(mysql_stmt_errno(_stmt)) + " " + mysql_stmt_error(_stmt), {});

			return result;
		});
	}

	void MySQLStatement::bind(uint64_t id, AnyValue value)
	{
		if (value.isType<std::vector<uint8_t>>()) {
			auto v = value.as<std::vector<uint8_t>>();
			if (v.size() > ULONG_MAX)
				throw DatabaseException("Blobvalue too long", {});
			void* data = malloc(v.size());
			memcpy(data, v.data(), v.size());
			this->setBind(id, MYSQL_TYPE_BLOB, data, (unsigned long)v.size(), false);
		}
		else if (value.type_info().isIntegral()) {
			auto v = value.as<int64_t>();
			void* data = malloc(sizeof(int64_t));
			memcpy(data, &v, sizeof(int64_t));
			this->setBind(id, MYSQL_TYPE_LONGLONG, data, sizeof(int64_t), false);
		}
		else if (value.type_info().isFloatingPoint()) {
			auto v = value.as<double>();
			void* data = malloc(sizeof(double));
			memcpy(data, &v, sizeof(double));
			this->setBind(id, MYSQL_TYPE_DOUBLE, data, sizeof(double), false);
		}
		else if (value.isType<nullptr_t>()) {
			this->setBind(id, MYSQL_TYPE_NULL, 0, 0, false);
		}
		else {
			auto v = value.as<std::string>();
			if (v.size() > ULONG_MAX)
				throw DatabaseException("String too long", {});
			void* data = malloc(v.size());
			memcpy(data, v.data(), v.size());
			this->setBind(id, MYSQL_TYPE_STRING, data, (unsigned long)v.size(), false);
		}
	}

	void MySQLStatement::bind(const std::string & id, AnyValue value)
	{
		throw DatabaseException("Binding by name is not supported by this database", {});
	}

	void MySQLStatement::setBind(uint64_t idx, enum_field_types type, void * data, unsigned long dlen, bool sign)
	{
		if (idx >= _param_count)
			throw DatabaseException("Invalid index", EasyCpp::Bundle({
				{"max_index", _param_count},
				{"index", idx}
		}));
		MYSQL_BIND* bind = &_param_bind[idx];
		// Clear data
		free(bind->buffer);

		memset(bind, 0, sizeof(MYSQL_BIND));
		bind->buffer_type = type;
		if (type != MYSQL_TYPE_NULL)
		{
			bind->buffer = data;
			bind->buffer_length = dlen;
			bind->is_unsigned = sign;
		}
	}

	AnyValue MySQLStatement::bind2Result(MYSQL_BIND * bind)
	{
		if (*bind->is_null)
		{
			return AnyValue();
		}
		else {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch" // We know nothing else is possible since we did set it.
#endif
			switch (bind->buffer_type)
			{
			case MYSQL_TYPE_LONGLONG:
				return AnyValue(*((int64_t*)bind->buffer));
			case MYSQL_TYPE_DOUBLE:
				return AnyValue(*((double*)bind->buffer));
			case MYSQL_TYPE_STRING:
				return AnyValue(std::string((const char*)bind->buffer, (const char*)(bind->buffer) + *bind->length));
			case MYSQL_TYPE_BLOB:
				return AnyValue(std::vector<uint8_t>((uint8_t*)bind->buffer, (uint8_t*)(bind->buffer) + *bind->length));
			default:
				return AnyValue();
			}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
		}
	}

	enum_field_types MySQLStatement::convertType(enum_field_types t)
	{
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch" // We know nothing else is possible since we did set it.
#endif
		switch (t)
		{
		case MYSQL_TYPE_TINY:
		case MYSQL_TYPE_SHORT:
		case MYSQL_TYPE_LONG:
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONGLONG:
			return MYSQL_TYPE_LONGLONG;
		case MYSQL_TYPE_DECIMAL:
		case MYSQL_TYPE_NEWDECIMAL:
		case MYSQL_TYPE_FLOAT:
		case MYSQL_TYPE_DOUBLE:
			return MYSQL_TYPE_DOUBLE;
		case MYSQL_TYPE_BIT:
		case MYSQL_TYPE_TIMESTAMP:
		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_YEAR:
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VAR_STRING:
		case MYSQL_TYPE_ENUM:
		case MYSQL_TYPE_SET:
		case MYSQL_TYPE_GEOMETRY:
			return MYSQL_TYPE_STRING;
		case MYSQL_TYPE_BLOB:
			return MYSQL_TYPE_BLOB;
		}
		return MYSQL_TYPE_BLOB;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

	}

	std::pair<std::vector<std::string>, std::unique_ptr<MYSQL_BIND, std::function<void(MYSQL_BIND*)>>> MySQLStatement::getBind(MYSQL_RES * meta)
	{
		std::pair<std::vector<std::string>, std::unique_ptr<MYSQL_BIND, std::function<void(MYSQL_BIND*)>>> res;
		MYSQL_FIELD *fields = mysql_fetch_fields(meta);
		unsigned int num_fields = mysql_num_fields(meta);
		// Dummy bind that receives content length on fetch
		MYSQL_BIND* bind = (MYSQL_BIND*)malloc(sizeof(MYSQL_BIND)*num_fields);
		memset(bind, 0x00, sizeof(MYSQL_BIND)*num_fields);
		for (unsigned long i = 0; i < num_fields; i++) {
			bind[i].length = (unsigned long*)malloc(sizeof(unsigned long));
			bind[i].error = (my_bool*)malloc(sizeof(my_bool));
			bind[i].is_null = (my_bool*)malloc(sizeof(my_bool));
			bind[i].buffer_type = convertType(fields[i].type);
			if (bind[i].buffer_type == MYSQL_TYPE_LONGLONG) {
				bind[i].buffer = malloc(sizeof(long long int));
				bind[i].buffer_length = sizeof(long long int);
			}
			else if (bind[i].buffer_type == MYSQL_TYPE_DOUBLE) {
				bind[i].buffer = malloc(sizeof(double));
				bind[i].buffer_length = sizeof(double);
			}
			res.first.push_back(fields[i].name);
		}
		res.second = std::unique_ptr<MYSQL_BIND, std::function<void(MYSQL_BIND*)>>(bind, [num_fields](MYSQL_BIND* bind) {
			if (bind != nullptr) {
				for (unsigned long i = 0; i < num_fields; i++)
				{
					free(bind[i].buffer);
					free(bind[i].length);
					free(bind[i].error);
					free(bind[i].is_null);
				}
				free(bind);
			}
		});
		return res;
	}

}