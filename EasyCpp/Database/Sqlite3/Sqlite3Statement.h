#pragma once
#include "../Statement.h"
#include "Sqlite3SharedData.h"

namespace EasyCpp
{
	namespace Database
	{
		namespace Sqlite3
		{
			class Sqlite3Statement: public Statement, public NonCopyable
			{
			public:
				Sqlite3Statement(Sqlite3SharedDataPtr shared, std::string sql);
				virtual ~Sqlite3Statement();
				// Geerbt über Statement
				virtual uint64_t execute() override;
				virtual AnyValue executeScalar() override;
				virtual Bundle executeQueryRow() override;
				virtual ResultSet executeQuery() override;
				virtual void bind(uint64_t id, AnyValue value) override;
				virtual void bind(const std::string & id, AnyValue value) override;
			private:
				Sqlite3SharedDataPtr _shared;
				struct sqlite3_stmt* _stmt;

				AnyValue getColumn(int i);
				int getParameterIndex(const std::string& name);
			};
		}
	}
}