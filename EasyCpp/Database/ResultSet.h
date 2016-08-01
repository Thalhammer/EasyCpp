#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "../Bundle.h"

namespace EasyCpp
{
	namespace Database
	{
		class DLL_EXPORT ResultSet
		{
		public:
			/// <summary>Create a empty ResultSet.</summary>
			ResultSet();
			/// <summary>Create a empty ResultSet with specified columns.</summary>
			ResultSet(std::unordered_set<std::string> columns);
			~ResultSet();

			/// <summary>Access row by index.</summary>
			Bundle& operator[](uint64_t);
			/// <summary>Access row by index.</summary>
			const Bundle& operator[](uint64_t) const;

			/// <summary>Get start iterator of rows.</summary>
			auto begin() { return _rows.begin(); }
			/// <summary>Get end iterator of rows.</summary>
			auto end() { return _rows.end(); }

			/// <summary>Get start iterator of rows.</summary>
			auto begin() const { return _rows.cbegin(); }
			/// <summary>Get end iterator of rows.</summary>
			auto end() const { return _rows.cend(); }

			/// <summary>Get a set of all columns found in any of the rows of this resultset.
			/// A row is not required to contain any of those columns, but every column in this set was found in at least one row.</summary>
			const std::unordered_set<std::string>& getColumns();

			/// <summary>Append a row to this ResultSet.</summary>
			void appendRow(const Bundle&);
			/// <summary>Return number of rows.</summary>
			size_t size();
		private:
			std::unordered_set<std::string> _columns;
			std::vector<Bundle> _rows;
		};
	}
}