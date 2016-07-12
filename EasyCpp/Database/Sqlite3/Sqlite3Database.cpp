#include "Sqlite3Database.h"
#include "../DatabaseException.h"
#include "Sqlite3Statement.h"

namespace EasyCpp
{
	namespace Database
	{
		namespace Sqlite3
		{
			Sqlite3Database::Sqlite3Database(const std::string & uri)
			{
				struct sqlite3* db;
				int rc = sqlite3_open_v2(uri.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, nullptr);
				if (rc != SQLITE_OK)
					throw DatabaseException("(" + std::to_string(rc) + ") " + sqlite3_errmsg(db), Bundle({ {"uri", uri} }));
				this->_shared = std::make_shared<Sqlite3SharedData>(db);
			}

			Sqlite3Database::~Sqlite3Database()
			{
			}

			bool Sqlite3Database::beginTransaction()
			{
				this->exec("BEGIN TRANSACTION");
				return true;
			}

			bool Sqlite3Database::commit()
			{
				this->exec("COMMIT");
				return true;
			}

			bool Sqlite3Database::rollBack()
			{
				this->exec("ROLLBACK");
				return true;
			}

			bool Sqlite3Database::inTransaction()
			{
				return sqlite3_get_autocommit(_shared->getDatabase().get()) == 0;
			}

			std::string Sqlite3Database::errorCode()
			{
				return std::string();
			}

			Bundle Sqlite3Database::errorInfo()
			{
				return Bundle();
			}

			uint64_t Sqlite3Database::exec(const std::string & sql)
			{
				auto stmt = this->prepare(sql, {});
				return stmt->execute();
			}

			AnyValue Sqlite3Database::getAttribute(const std::string & attribute)
			{
				throw std::out_of_range("attribute not found");
			}

			bool Sqlite3Database::setAttribute(const std::string & attribute, const AnyValue & value)
			{
				return false;
			}

			std::string Sqlite3Database::lastInsertedId(const std::string & name)
			{
				return std::to_string(sqlite3_last_insert_rowid(_shared->getDatabase().get()));
			}

			StatementPtr Sqlite3Database::prepare(const std::string & sql, const Bundle & driver_options)
			{
				return std::make_shared<Sqlite3Statement>(_shared, sql);
			}
		}
	}
}