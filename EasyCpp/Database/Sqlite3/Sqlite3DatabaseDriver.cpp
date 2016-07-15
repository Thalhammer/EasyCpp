#include "Sqlite3DatabaseDriver.h"
#include "Sqlite3Database.h"
#include "../DatabaseDriverManager.h"
#include "../../AutoInit.h"

AUTO_INIT({
	EasyCpp::Database::DatabaseDriverManager::registerDriver("sqlite3", std::make_shared<EasyCpp::Database::Sqlite3::Sqlite3DatabaseDriver>());
})

namespace EasyCpp
{
	namespace Database
	{
		namespace Sqlite3
		{
			Sqlite3DatabaseDriver::~Sqlite3DatabaseDriver()
			{
			}

			DatabasePtr Sqlite3DatabaseDriver::createInstance(const std::string & dsn, const Bundle & options)
			{
				return std::make_shared<Sqlite3::Sqlite3Database>(dsn);
			}

			Sqlite3DatabaseDriver::Sqlite3DatabaseDriver()
			{
			}
		}
	}
}