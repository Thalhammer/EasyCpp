#include "ResultSet.h"

namespace EasyCpp
{
	namespace Database
	{
		ResultSet::ResultSet()
		{
		}

		ResultSet::ResultSet(std::unordered_set<std::string> columns)
			:_columns(columns)
		{
		}

		ResultSet::~ResultSet()
		{
		}

		Bundle & ResultSet::operator[](uint64_t idx)
		{
			return _rows[(size_t)idx];
		}

		const Bundle & ResultSet::operator[](uint64_t idx) const
		{
			return _rows[(size_t)idx];
		}

		const std::unordered_set<std::string>& ResultSet::getColumns()
		{
			return _columns;
		}

		void ResultSet::appendRow(const Bundle & row)
		{
			for (const auto& it : row)
			{
				if (!_columns.count(it.first))
					_columns.insert(it.first);
			}
			_rows.push_back(row);
		}

		size_t ResultSet::size()
		{
			return _rows.size();
		}
	}
}