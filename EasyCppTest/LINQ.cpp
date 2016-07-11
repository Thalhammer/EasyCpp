#include <gtest/gtest.h>
#include <LINQ.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(LINQ, OrderBy)
	{
		std::vector<std::string> elems = {
			"E",
			"A",
			"C",
			"B",
			"D",
			"F"
		};
		auto query = LINQ(elems)->OrderBy<std::string>([](const auto& elem) {
			return elem;
		})->ToVector();

		ASSERT_EQ(6, query.size());
		ASSERT_EQ("A", query[0]);
		ASSERT_EQ("B", query[1]);
		ASSERT_EQ("C", query[2]);
		ASSERT_EQ("D", query[3]);
		ASSERT_EQ("E", query[4]);
		ASSERT_EQ("F", query[5]);
	}

	TEST(LINQ, Sum)
	{
		std::vector<int> elems = {
			10,
			100,
			1000
		};
		auto res = LINQ(elems)->Sum();
		ASSERT_EQ(1110, res);
	}

	TEST(LINQ, Average)
	{
		std::vector<double> elems = {
			10,
			20,
			60
		};
		auto res = LINQ(elems)->Average();
		ASSERT_EQ(30, res);
	}

	TEST(LINQ, MaxMin)
	{
		std::vector<int> elems = {
			10,
			20,
			60
		};
		auto max = LINQ(elems)->Maximum();
		auto min = LINQ(elems)->Minimum();
		ASSERT_EQ(60, max);
		ASSERT_EQ(10, min);
	}

	TEST(LINQ, SingleFirstCount)
	{
		std::vector<int> elems_single = {
			10
		};
		std::vector<int> elems = {
			10,
			20,
			100
		};
		auto single = LINQ(elems_single)->Single();
		ASSERT_EQ(10, single);
		auto first = LINQ(elems)->First();
		ASSERT_EQ(10, first);
		auto count = LINQ(elems)->Count();
		ASSERT_EQ(3, count);
	}

	TEST(LINQ, Where)
	{
		std::vector<std::string> elems = {
			"Hallo",
			"World",
			"!"
		};
		auto res = LINQ(elems)->Where([](auto e) {
			return e.size() == 5;
		})->ToVector();
		ASSERT_EQ(2, res.size());
		ASSERT_EQ("Hallo", res[0]);
		ASSERT_EQ("World", res[1]);
	}

	TEST(LINQ, GroupBy)
	{
		std::vector<std::string> elems = {
			"Hallo",
			"World",
			"!"
		};
		auto res = LINQ(elems)->GroupBy<size_t>([](auto e) { return e.size(); })->ToVector();
		ASSERT_EQ(2, res.size());
		auto res1 = res[0].getIterator()->ToVector();
		auto res2 = res[1].getIterator()->ToVector();
		if (res[0].getKey() == 5) {
			ASSERT_EQ(2, res1.size());
			ASSERT_EQ(1, res2.size());
			ASSERT_EQ(5, res[0].getKey());
			ASSERT_EQ(1, res[1].getKey());
			ASSERT_EQ("Hallo", res1[0]);
			ASSERT_EQ("World", res1[1]);
			ASSERT_EQ("!", res2[0]);
		}
		else {
			ASSERT_EQ(2, res2.size());
			ASSERT_EQ(1, res1.size());
			ASSERT_EQ(5, res[1].getKey());
			ASSERT_EQ(1, res[0].getKey());
			ASSERT_EQ("Hallo", res2[0]);
			ASSERT_EQ("World", res2[1]);
			ASSERT_EQ("!", res1[0]);
		}
	}
}
