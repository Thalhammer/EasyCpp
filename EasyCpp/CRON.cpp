#include "CRON.h"
#include <ctime>
#include "StringAlgorithm.h"
#include <map>
#include "SafeTime.h"

namespace EasyCpp
{
	CRON::CRON(const std::string & istr)
	{
		std::string str = istr;
		if (istr == "@daily" || istr == "@midnight")
			str = "0 0 * * *";
		else if (istr == "@hourly")
			str = "0 * * * *";
		else if (istr == "@weekly")
			str = "0 0 * * 0";
		else if (istr == "@monthly")
			str = "0 0 1 * *";
		else if (istr == "@annually" || istr == "@yearly")
			str = "0 0 1 1 *";

		_parts = stringSplit(" ", str);
		if (_parts.size() < 5)
			throw std::runtime_error("Invalid cron string");

		_match_map[0] = GetMatches(_parts[0], 0, 59);
		_match_map[1] = GetMatches(_parts[1], 0, 23);
		_match_map[2] = GetMatches(_parts[2], 1, 31);
		_match_map[3] = GetMatches(_parts[3], 1, 12);
		_match_map[4] = GetMatches(_parts[4], 0, 6);
	}

	CRON::~CRON()
	{
	}

	CRON::tp_t CRON::getNextRun(const tp_t & start_time)
	{
		time_t start = std::chrono::system_clock::to_time_t(start_time);
		// Remove seconds
		start = start - (start % 60);
		
		struct tm time = Time::localtime(start);

		int ntime = -1;
		for (auto& e : _match_map[1]) {
			if (e.second && e.first >= time.tm_hour) {
				for (auto& e2 : _match_map[0]) {
					if (e2.second && e2.first > time.tm_min) {
						ntime = e.first * 100 + e2.first;
						break;
					}
				}
				if (ntime != -1) break;
			}
		}
		if (ntime != -1 && matches(ntime%100, ntime/100, time.tm_mday, time.tm_mon, time.tm_wday)) {
			struct tm next = time;
			next.tm_sec = 0;
			next.tm_hour = ntime / 100;
			next.tm_min = ntime % 100;
			return std::chrono::system_clock::from_time_t(mktime(&next));
		}

		if(ntime == -1)
			ntime = GetFirst(_match_map[1]) * 100 + GetFirst(_match_map[0]);

		int ndate = -1;
		for (auto& e : _match_map[3]) {
			if (e.second && e.first == time.tm_mon + 1) {
				for (auto& e2 : _match_map[2]) {
					if (e2.second && e2.first > time.tm_mday) {
						ndate = e.first * 100 + e2.first;
						break;
					}
				}
				if (ndate != -1) break;
			} else if (e.second && e.first > time.tm_mon + 1) {
				for (auto& e2 : _match_map[2]) {
					if (e2.second) {
						ndate = e.first * 100 + e2.first;
						break;
					}
				}
				if (ndate != -1) break;
			}
		}

		struct tm next;
		next.tm_year = time.tm_year;
		next.tm_isdst = time.tm_isdst;

		next.tm_sec = 0;
		next.tm_hour = ntime / 100;
		next.tm_min = ntime % 100;
		if (ndate == -1) {
			ndate = GetFirst(_match_map[3]) * 100 + GetFirst(_match_map[2]);
			next.tm_year++;
		}
		next.tm_mon = (ndate / 100) - 1;
		next.tm_mday = ndate % 100;
		time_t tnext = mktime(&next);

		if (_parts[4] != "*") {
			struct tm wnext = time;
			wnext.tm_sec = 0;
			wnext.tm_hour = ntime / 100;
			wnext.tm_min = ntime % 100;
			time_t twnext = mktime(&wnext);
			for (auto& e : _match_map[4])
			{
				if (e.second) {
					time_t tmp = twnext;
					if (e.first > wnext.tm_wday) {
						tmp += (e.first - wnext.tm_wday) * (60 * 60 * 24);
					}
					else {
						tmp += ((7- wnext.tm_wday) + e.first) * (60 * 60 * 24);
					}
					// Special case: Only wday relevant
					if (tmp < tnext || _parts[2] == "*") {
						tnext = tmp;
						break;
					}
				}
			}
		}

		return std::chrono::system_clock::from_time_t(tnext);
	}

	std::vector<CRON::tp_t> CRON::getNextRuns(const tp_t & start_time, size_t cnt)
	{
		std::vector<tp_t> res;
		res.push_back(this->getNextRun(start_time));
		for (size_t i = 1; i < cnt; i++)
		{
			res.push_back(this->getNextRun(res[i - 1]));
		}
		return res;
	}

	bool CRON::matches(const tp_t & tp)
	{
		struct tm time = Time::localtime(std::chrono::system_clock::to_time_t(tp));
		return matches(time.tm_min, time.tm_hour, time.tm_mday, time.tm_mon, time.tm_wday);
	}

	bool CRON::matches(int min, int hour, int mday, int mon, int wday)
	{
		if (min != -1 && !_match_map[0].at(min))
			return false;
		if (hour != -1 && !_match_map[1].at(hour))
			return false;
		if (_parts[2] == "*" && _parts[4] != "*") {
			if (wday != -1 && !_match_map[4].at(wday))
				return false;
		} else if (_parts[2] != "*" && _parts[4] != "*") {
			if ((wday != -1 && !_match_map[4].at(wday)) && (mday != -1 && !_match_map[2].at(mday)))
				return false;
		}
		else {
			if (mday != -1 && !_match_map[2].at(mday))
				return false;
		}
		if (mon != -1 && !_match_map[3].at(mon))
			return false;

		return true;
	}

	std::map<int, bool> CRON::GetMatches(const std::string & str, int min, int max)
	{
		std::map<int, bool> res;
		for (int i = min; i <= max; i++)
			res.insert({ i, false });

		auto parts = stringSplit(",", str);
		for (auto& part : parts)
		{
			part = stringReplace(part, "*", std::to_string(min) + "-" + std::to_string(max));
			std::vector<int> matches;
			if (part.find('-') != std::string::npos)
			{
				int skip = 1;
				if (part.find('/') != std::string::npos)
				{
					auto pos = part.find('/');
					skip = std::stoi(part.substr(pos + 1));
					part = part.substr(0, pos);
				}
				auto pos = part.find('-');
				int first = std::stoi(part.substr(0, pos));
				int second = std::stoi(part.substr(pos + 1));
				for (int i = first; i <= second; i+=skip)
					matches.push_back(i);
			}
			else {
				matches.push_back(std::stoi(part));
			}
			for (auto& e : matches)
				res.at(e) = true;
		}

		return res;
	}

	int CRON::GetFirst(const std::map<int, bool>& map)
	{
		for (auto& e : map)
		{
			if (e.second)
				return e.first;
		}
		throw std::runtime_error("No match");
	}
}