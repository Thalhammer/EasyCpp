#include "Sqlite3Statement.h"
#include <cstddef>
#include "../DatabaseException.h"

#include "../../Finally.h"

namespace EasyCpp
{
	namespace Database
	{
		namespace Sqlite3
		{
			Sqlite3Statement::Sqlite3Statement(Sqlite3SharedDataPtr shared, std::string sql)
				:_shared(shared)
			{
				auto db = _shared->getDatabase();
				if (sql.size() > INT_MAX) throw std::string("Query too large");
				int rc = sqlite3_prepare_v2(db.get(), sql.c_str(), (int)sql.size(), &_stmt, nullptr);
				if (rc != SQLITE_OK)
					throw DatabaseException("(" + std::to_string(rc) + ") " + sqlite3_errmsg(db.get()), Bundle({ { "sql", sql } }));
			}

			Sqlite3Statement::~Sqlite3Statement()
			{
				sqlite3_finalize(_stmt);
			}

			uint64_t Sqlite3Statement::execute()
			{
				auto db = _shared->getDatabase();
				Finally finally([this, db]() {
					sqlite3_reset(_stmt);
				});
				int rc = sqlite3_reset(_stmt);
				// Do we need to check the return code ?
				rc = sqlite3_step(_stmt);
				if (rc == SQLITE_ROW)
					throw DatabaseException("Statement returned rows", {});
				if (rc != SQLITE_DONE)
					throw DatabaseException("Failed to execute statement", {});
				int res = sqlite3_changes(db.get());
				return res;
			}

			AnyValue Sqlite3Statement::executeScalar()
			{
				auto db = _shared->getDatabase();
				Finally finally([this, db]() {
					sqlite3_reset(_stmt);
				});
				int rc = sqlite3_reset(_stmt);
				// Do we need to check the return code ?
				rc = sqlite3_step(_stmt);
				if (rc != SQLITE_OK&&rc != SQLITE_ROW)
					throw DatabaseException("Failed to execute statement", {});
				return getColumn(0);
			}

			Bundle Sqlite3Statement::executeQueryRow()
			{
				auto db = _shared->getDatabase();
				Finally finally([this, db]() {
					sqlite3_reset(_stmt);
				});
				int rc = sqlite3_reset(_stmt);
				// Do we need to check the return code ?
				rc = sqlite3_step(_stmt);
				if (rc != SQLITE_OK&&rc != SQLITE_ROW)
					throw DatabaseException("Failed to execute statement", {});

				int colcount = sqlite3_column_count(_stmt);
				if (rc == SQLITE_DONE) // No result rows
					throw DatabaseException("Query did not return a result", {});
				Bundle row;
				for (int i = 0; i < colcount; i++)
					row.set(sqlite3_column_name(_stmt, i), getColumn(i));
				return row;
			}

			ResultSet Sqlite3Statement::executeQuery()
			{
				auto db = _shared->getDatabase();
				Finally finally([this, db]() {
					sqlite3_reset(_stmt);
				});
				int rc = sqlite3_reset(_stmt);
				// Do we need to check the return code ?
				rc = sqlite3_step(_stmt);
				if (rc != SQLITE_OK&&rc != SQLITE_ROW)
					throw DatabaseException("Failed to execute statement", {});

				int colcount = sqlite3_column_count(_stmt);
				std::vector<std::string> names;
				for (int i = 0; i < colcount; i++)
					names.push_back(sqlite3_column_name(_stmt, i));
				ResultSet res(std::unordered_set<std::string>(names.begin(), names.end()));

				if (rc == SQLITE_DONE) // No result rows
					return res;
				do {
					Bundle row;
					for (int i = 0; i < colcount; i++)
						row.set(names[i], getColumn(i));
					res.appendRow(row);

					// Read next row
					rc = sqlite3_step(_stmt);
					if (rc != SQLITE_DONE&&rc != SQLITE_ROW)
						throw DatabaseException("Failed to execute statement", {});
				} while (rc != SQLITE_DONE);
				return res;
			}

			void Sqlite3Statement::bind(uint64_t id, AnyValue value)
			{
				auto db = _shared->getDatabase();
				if (value.isType<std::vector<uint8_t>>()) {
					auto v = value.as<std::vector<uint8_t>>();
					int rc = sqlite3_bind_blob64(_stmt, (int)(id + 1), v.data(), v.size(), SQLITE_TRANSIENT);
					if (rc != SQLITE_OK)
						throw DatabaseException(sqlite3_errmsg(db.get()), {});
				}
				else if (value.type_info().isIntegral()) {
					auto v = value.as<int64_t>();
					int rc = sqlite3_bind_int64(_stmt, (int)(id + 1), v);
					if (rc != SQLITE_OK)
						throw DatabaseException(sqlite3_errmsg(db.get()), {});
				}
				else if (value.type_info().isFloatingPoint()) {
					auto v = value.as<double>();
					int rc = sqlite3_bind_double(_stmt, (int)(id + 1), v);
					if (rc != SQLITE_OK)
						throw DatabaseException(sqlite3_errmsg(db.get()), {});
				}
				else if (value.isType<nullptr_t>()) {
					int rc = sqlite3_bind_null(_stmt, (int)(id + 1));
					if (rc != SQLITE_OK)
						throw DatabaseException(sqlite3_errmsg(db.get()), {});
				}
				else {
					auto v = value.as<std::string>();
					int rc = sqlite3_bind_text64(_stmt, (int)(id + 1), v.data(), v.length(), SQLITE_TRANSIENT, SQLITE_UTF8);
					if (rc != SQLITE_OK)
						throw DatabaseException(sqlite3_errmsg(db.get()), {});
				}
			}

			void Sqlite3Statement::bind(const std::string & id, AnyValue value)
			{
				this->bind(this->getParameterIndex(id), value);
			}

			AnyValue Sqlite3Statement::getColumn(int i)
			{
				int dtype = sqlite3_column_type(_stmt, i);
				if (dtype == SQLITE_INTEGER)
				{
					return AnyValue((int64_t)sqlite3_column_int64(_stmt, i));
				}
				else if (dtype == SQLITE_FLOAT)
				{
					return AnyValue(sqlite3_column_double(_stmt, i));
				}
				else if (dtype == SQLITE_BLOB)
				{
					const uint8_t* ptr = (const uint8_t*)sqlite3_column_blob(_stmt, i);
					int length = sqlite3_column_bytes(_stmt, i);
					return AnyValue(std::vector<uint8_t>(ptr, ptr + length));
				}
				else if (dtype == SQLITE3_TEXT)
				{
					int length = sqlite3_column_bytes(_stmt, i);
					auto ptr = (const char*)sqlite3_column_text(_stmt, i);
					return AnyValue(std::string(ptr, ptr + length));
				}
				else if (dtype == SQLITE_NULL)
				{
					return AnyValue();
				}
				throw DatabaseException("Invalid result type", {});
			}
			int Sqlite3Statement::getParameterIndex(const std::string & name)
			{
				int rc = sqlite3_bind_parameter_index(_stmt, name.c_str());
				if (rc == 0)
					throw DatabaseException("Parameter not found", {});
				return rc - 1;
			}
		}
	}
}
