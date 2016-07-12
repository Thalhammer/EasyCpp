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
			ResultSet();
			ResultSet(std::unordered_set<std::string> columns);
			~ResultSet();

			Bundle& operator[](uint64_t);
			const Bundle& operator[](uint64_t) const;

			auto begin() { return _rows.begin(); }
			auto end() { return _rows.end(); }

			auto begin() const { return _rows.cbegin(); }
			auto end() const { return _rows.cend(); }

			const std::unordered_set<std::string> getColumns();

			void appendRow(const Bundle&);
			size_t size();
		private:
			std::unordered_set<std::string> _columns;
			std::vector<Bundle> _rows;
		};
	}
}