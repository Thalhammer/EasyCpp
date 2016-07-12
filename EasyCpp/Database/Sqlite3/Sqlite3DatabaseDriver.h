#pragma once
#include "../DatabaseDriver.h"
#include <vector>

namespace EasyCpp
{
	namespace Database
	{
		namespace Sqlite3
		{
			class Sqlite3DatabaseDriver: public EasyCpp::Database::DatabaseDriver
			{
			public:
				Sqlite3DatabaseDriver();
				virtual ~Sqlite3DatabaseDriver();
				virtual DatabasePtr createInstance(const std::string& dsn, const Bundle& options = {}) override;
			private:
			};
		}
	}
}