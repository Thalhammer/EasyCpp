#include "MySQLStatement.h"

using namespace EasyCpp;
using namespace EasyCpp::Database;

namespace EasyCppMySql
{

	MySQLStatement::MySQLStatement(const std::string& sql, std::shared_ptr<MySQLHandle> hdl)
		:_stmt(nullptr), _param_bind(nullptr), _result_meta_bind(nullptr), _hdl(hdl)
	{
		_hdl->executeThreadSafe<void>([this, sql](MySQLHandle::HandleAccessor& hdl) {
			_stmt = mysql_stmt_init(hdl.getHandle());
			if (_stmt == nullptr)
				throw DatabaseException("Failed to initialize statement", {});

			// Try to prepare statement
			if (mysql_stmt_prepare(_stmt, sql.c_str(), sql.length()) != 0)
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

			// Get information about result
			auto del = [](MYSQL_RES* res) { mysql_free_result(res); };
			std::unique_ptr<MYSQL_RES, decltype(del)> meta(mysql_stmt_result_metadata(_stmt), del);
			if (meta) {
				_result_meta_count = mysql_num_fields(meta.get());
				MYSQL_FIELD *fields = mysql_fetch_fields(meta.get());
				// Dummy bind that receives content length on fetch
				this->_result_meta_bind = (MYSQL_BIND*)malloc(sizeof(MYSQL_BIND)*_result_meta_count);
				memset(_result_meta_bind, 0x00, sizeof(MYSQL_BIND)*_result_meta_count);
				for (unsigned long i = 0; i < _result_meta_count; i++) {
					_result_meta_bind[i].length = (unsigned long*)malloc(sizeof(unsigned long));
					_result_meta_bind[i].error = (my_bool*)malloc(sizeof(my_bool));
					_result_meta_bind[i].is_null = (my_bool*)malloc(sizeof(my_bool));
					_result_meta_bind[i].buffer_type = convertType(fields[i].type);
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch" // We know nothing else is possible since we did set it.
#endif
					switch (_result_meta_bind[i].buffer_type)
					{
					case MYSQL_TYPE_LONGLONG:
						_result_meta_bind[i].buffer = malloc(sizeof(long long int));
						_result_meta_bind[i].buffer_length = sizeof(long long int);
						break;
					case MYSQL_TYPE_DOUBLE:
						_result_meta_bind[i].buffer = malloc(sizeof(double));
						_result_meta_bind[i].buffer_length = sizeof(double);
						break;
					}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
					_result_names.push_back(fields[i].name);
				}
				if (mysql_stmt_bind_result(_stmt, _result_meta_bind) != 0)
					throw DatabaseException("Failed to bind buffer", {});
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

		if (_result_meta_bind != nullptr) {
			for (unsigned long i = 0; i < _result_meta_count; i++)
			{
				free(_result_meta_bind[i].buffer);
				free(_result_meta_bind[i].length);
				free(_result_meta_bind[i].error);
				free(_result_meta_bind[i].is_null);
			}
			free(_result_meta_bind);
		}
	}

	uint64_t MySQLStatement::execute()
	{
		return _hdl->executeThreadSafe<uint64_t>([this](MySQLHandle::HandleAccessor& hdl) {
			if (mysql_stmt_bind_param(_stmt, _param_bind) != 0)
				throw DatabaseException("Failed to bind parameters", {});
			if (mysql_stmt_execute(_stmt) != 0)
				throw DatabaseException("Failed to execute statement", {});
			return mysql_stmt_affected_rows(_stmt);
		});
	}

	AnyValue MySQLStatement::executeScalar()
	{
		return AnyValue();
	}

	Bundle MySQLStatement::executeQueryRow()
	{
		return Bundle();
	}

	ResultSet MySQLStatement::executeQuery()
	{
		return ResultSet();
	}

	void MySQLStatement::bind(uint64_t id, AnyValue value)
	{
	}

	void MySQLStatement::bind(const std::string & id, AnyValue value)
	{
		throw DatabaseException("Binding by name is not supported by this database", {});
	}

	void MySQLStatement::setBind(unsigned long idx, enum_field_types type, void * data, unsigned long dlen, bool sign)
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

}