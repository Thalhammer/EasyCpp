#include "Sqlite3SharedData.h"

namespace EasyCpp
{
	namespace Database
	{
		namespace Sqlite3
		{
			Sqlite3SharedData::Sqlite3SharedData(struct sqlite3 * db)
				:_database(db)
			{
			}
			Sqlite3SharedData::~Sqlite3SharedData()
			{
				sqlite3_close_v2(_database);
			}
			std::shared_ptr<struct sqlite3> Sqlite3SharedData::getDatabase()
			{
				_mutex.lock();
				return std::shared_ptr<struct sqlite3>(_database, [this](void*) { _mutex.unlock(); });
			}
		}
	}
}