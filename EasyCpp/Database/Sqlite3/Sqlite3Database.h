#pragma once
#include "../Database.h"
#include "Sqlite3SharedData.h"

namespace EasyCpp
{
	namespace Database
	{
		namespace Sqlite3
		{
			class Sqlite3Database : public Database
			{
			public:
				Sqlite3Database(const std::string& uri);
				virtual ~Sqlite3Database();

				// Geerbt über Database
				virtual bool beginTransaction() override;
				virtual bool commit() override;
				virtual bool rollBack() override;
				virtual bool inTransaction() override;
				virtual std::string errorCode() override;
				virtual Bundle errorInfo() override;
				virtual uint64_t exec(const std::string & sql) override;
				virtual AnyValue getAttribute(const std::string & attribute) override;
				virtual bool setAttribute(const std::string & attribute, const AnyValue & value) override;
				virtual std::string lastInsertedId(const std::string & name) override;
				virtual StatementPtr prepare(const std::string & sql, const Bundle & driver_options) override;
			private:
				Sqlite3SharedDataPtr _shared;
			};
		}
	}
}