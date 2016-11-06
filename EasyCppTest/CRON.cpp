#include <gtest/gtest.h>
#include <CRON.h>
#include <ctime>
#include <SafeTime.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(CRON, Matches)
	{
		std::string crontab = "0 23 1-10/3 * *";
		struct tm time;
		time.tm_hour = 23;
		time.tm_mday = 07;
		time.tm_min = 0;
		time.tm_mon = 11;
		time.tm_sec = 0;
		time.tm_year = 116; // 2016
		struct tm time2 = time;
		time2.tm_hour = 3;
		time_t ttime = mktime(&time);
		time_t ttime2 = mktime(&time2);

		CRON cron(crontab);
		ASSERT_TRUE(cron.matches(std::chrono::system_clock::from_time_t(ttime)));
		ASSERT_FALSE(cron.matches(std::chrono::system_clock::from_time_t(ttime2)));

	}

	TEST(CRON, GetNext)
	{
		std::string crontab = "0 23 1-10/3 * *";
		struct tm time;
		time.tm_year = 116; // 2016
		time.tm_mon = 10;
		time.tm_mday = 06;
		time.tm_hour = 15;
		time.tm_min = 11;
		time.tm_sec = 19;
		auto tp = std::chrono::system_clock::from_time_t(mktime(&time));

		CRON cron(crontab);
		auto next = cron.getNextRun(tp);

		time = Time::localtime(std::chrono::system_clock::to_time_t(next));
		ASSERT_EQ(116, time.tm_year);
		ASSERT_EQ(10, time.tm_mon);
		ASSERT_EQ(7, time.tm_mday);
		ASSERT_EQ(23, time.tm_hour);
		ASSERT_EQ(0, time.tm_min);
		ASSERT_EQ(0, time.tm_sec);

		next = cron.getNextRun(next);

		time = Time::localtime(std::chrono::system_clock::to_time_t(next));
		ASSERT_EQ(116, time.tm_year);
		ASSERT_EQ(10, time.tm_mon);
		ASSERT_EQ(10, time.tm_mday);
		ASSERT_EQ(23, time.tm_hour);
		ASSERT_EQ(0, time.tm_min);
		ASSERT_EQ(0, time.tm_sec);
	}

	TEST(CRON, GetNextMulti)
	{
		std::string crontab = "0 23 1-10/3 * *";
		struct tm time;
		time.tm_year = 116; // 2016
		time.tm_mon = 10;
		time.tm_mday = 06;
		time.tm_hour = 15;
		time.tm_min = 11;
		time.tm_sec = 19;
		auto tp = std::chrono::system_clock::from_time_t(mktime(&time));

		CRON cron(crontab);
		auto next = cron.getNextRuns(tp, 3);

		ASSERT_EQ(3, next.size());

		time = Time::localtime(std::chrono::system_clock::to_time_t(next[0]));
		ASSERT_EQ(116, time.tm_year);
		ASSERT_EQ(10, time.tm_mon);
		ASSERT_EQ(7, time.tm_mday);
		ASSERT_EQ(23, time.tm_hour);
		ASSERT_EQ(0, time.tm_min);
		ASSERT_EQ(0, time.tm_sec);

		time = Time::localtime(std::chrono::system_clock::to_time_t(next[1]));
		ASSERT_EQ(116, time.tm_year);
		ASSERT_EQ(10, time.tm_mon);
		ASSERT_EQ(10, time.tm_mday);
		ASSERT_EQ(23, time.tm_hour);
		ASSERT_EQ(0, time.tm_min);
		ASSERT_EQ(0, time.tm_sec);

		time = Time::localtime(std::chrono::system_clock::to_time_t(next[2]));
		ASSERT_EQ(116, time.tm_year);
		ASSERT_EQ(11, time.tm_mon);
		ASSERT_EQ(1, time.tm_mday);
		ASSERT_EQ(23, time.tm_hour);
		ASSERT_EQ(0, time.tm_min);
		ASSERT_EQ(0, time.tm_sec);
	}

	TEST(CRON, GetNextWeekly)
	{
		std::string crontab = "@weekly";
		struct tm time;
		time.tm_year = 116; // 2016
		time.tm_mon = 10;
		time.tm_mday = 06;
		time.tm_hour = 15;
		time.tm_min = 11;
		time.tm_sec = 19;
		auto tp = std::chrono::system_clock::from_time_t(mktime(&time));

		CRON cron(crontab);
		auto next = cron.getNextRun(tp);

		time = Time::localtime(std::chrono::system_clock::to_time_t(next));
		ASSERT_EQ(116, time.tm_year);
		ASSERT_EQ(10, time.tm_mon);
		ASSERT_EQ(13, time.tm_mday);
		ASSERT_EQ(0, time.tm_hour);
		ASSERT_EQ(0, time.tm_min);
		ASSERT_EQ(0, time.tm_sec);

		next = cron.getNextRun(next);

		time = Time::localtime(std::chrono::system_clock::to_time_t(next));
		ASSERT_EQ(116, time.tm_year);
		ASSERT_EQ(10, time.tm_mon);
		ASSERT_EQ(20, time.tm_mday);
		ASSERT_EQ(0, time.tm_hour);
		ASSERT_EQ(0, time.tm_min);
		ASSERT_EQ(0, time.tm_sec);
	}
}