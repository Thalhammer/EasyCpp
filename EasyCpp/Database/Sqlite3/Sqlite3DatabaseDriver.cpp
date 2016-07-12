#include "Sqlite3DatabaseDriver.h"
#include "Sqlite3Database.h"

namespace {
	class InitDriver
	{
	public:
		InitDriver() {
			EasyCpp::Database::DatabaseDriver::registerDriver("sqlite3", std::make_shared<EasyCpp::Database::Sqlite3::Sqlite3DatabaseDriver>());
		}
	private:
		static InitDriver _auto_init;
	};
	InitDriver InitDriver::_auto_init;
}

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