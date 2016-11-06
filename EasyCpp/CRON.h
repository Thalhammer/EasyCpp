#pragma once
#include "DllExport.h"
#include <string>
#include <vector>
#include <chrono>
#include <array>
#include <map>

namespace EasyCpp
{
	class DLL_EXPORT CRON
	{
	public:
		typedef std::chrono::system_clock::time_point tp_t;

		CRON(const std::string& str);
		~CRON();

		tp_t getNextRun(const tp_t& start_time = std::chrono::system_clock::now());
		std::vector<tp_t> getNextRuns(const tp_t& start_time = std::chrono::system_clock::now(), size_t cnt = 5);
		bool matches(const tp_t& tp);
	private:
		bool matches(int min, int hour, int mday, int mon, int wday=-1);

		std::vector<std::string> _parts;
		std::array<std::map<int, bool>, 5> _match_map;

		static std::map<int, bool> GetMatches(const std::string& str, int min, int max);
		static int GetFirst(const std::map<int, bool>& map);
	};
}