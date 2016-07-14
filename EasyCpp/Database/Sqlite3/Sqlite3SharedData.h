#pragma once
extern "C" {
#include <sqlite3/sqlite3.h>
}
#include "../../NonCopyable.h"
#include <memory>
#include <mutex>

namespace EasyCpp
{
	namespace Database
	{
		namespace Sqlite3
		{
			class Sqlite3SharedData: public NonCopyable
			{
			public:
				Sqlite3SharedData(struct sqlite3* db);
				~Sqlite3SharedData();
				std::shared_ptr<struct sqlite3> getDatabase();
			private:
				struct sqlite3* _database;
				std::recursive_mutex _mutex;
			};
			typedef std::shared_ptr<Sqlite3SharedData> Sqlite3SharedDataPtr;
		}
	}
}
