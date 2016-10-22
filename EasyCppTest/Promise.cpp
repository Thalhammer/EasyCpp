#include <gtest/gtest.h>
#include <Promise.h>
#include <vector>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Promise, Then)
	{
		bool executed = false;
		Promise<int> promise;
		auto p = promise.then(std::function<int(int&)>([&executed](int& val) -> int {
			executed = true;
			[&](){
				ASSERT_EQ(10, val);
			}();
			return 0;
		}));
		ASSERT_FALSE(executed);
		promise.resolve(10);
		ASSERT_TRUE(executed);
	}

	TEST(Promise, ThenFulfilled)
	{
		bool executed = false;
		Promise<int> promise;
		promise.resolve(10);
		auto p = promise.then(std::function<int(int&)>([&executed](int& val) -> int {
			executed = true;
			[&]() {
				ASSERT_EQ(10, val);
			}();
			return 0;
		}));
		ASSERT_TRUE(executed);
	}

	TEST(Promise, Chain)
	{
		Promise<int> promise;
		promise.then(std::function<int(int&)>([](int& val) -> int {
			return 0;
		})).then(std::function<std::string(int&)>([](int& val) -> std::string {
			return std::to_string(val);
		}));
		promise.resolve(10);
	}

	TEST(Promise, Void)
	{
		Promise<void> promise;
		promise.then(std::function<void()>([]() {
		}));
		promise.resolve();
	}

	TEST(Promise, UnWrap)
	{
		uint32_t step = 0;
		Promise<int> promise;
		Promise<std::string> p;
		promise.then(std::function<Promise<std::string>(int&)>([p, &step](int& val) {
			[&]() {
				ASSERT_EQ(1, step++);
			}();
			return p;
		})).then(std::function<int(std::string&)>([&step](std::string& val) {
			[&]() {
				ASSERT_EQ(3, step++);
			}();
			return 0;
		}));

		ASSERT_EQ(0, step++);
		promise.resolve(100);
		ASSERT_EQ(2, step++);
		p.resolve("Hallo");
		ASSERT_EQ(4, step++);
	}

	TEST(Promise, All)
	{
		Promise<size_t> p1;
		Promise<size_t> p2;
		Promise<size_t> p3;
		bool executed = false;

		auto all = Promise<size_t>::All({ p1,p2,p3 });
		all.then<void>([&executed](std::vector<size_t>& val) {
			executed = true;
			ASSERT_EQ(1, val[0]);
			ASSERT_EQ(2, val[1]);
			ASSERT_EQ(3, val[2]);
		});

		ASSERT_FALSE(executed);
		p1.resolve(1);
		ASSERT_FALSE(executed);
		p2.resolve(2);
		ASSERT_FALSE(executed);
		p3.resolve(3);
		ASSERT_TRUE(executed);
	}

	TEST(Promise, Race)
	{
		Promise<size_t> p1;
		Promise<size_t> p2;
		Promise<size_t> p3;
		bool executed = false;

		auto any = Promise<size_t>::Race({ p1,p2,p3 });
		any.then<void>([&executed](size_t& val) {
			executed = true;
			ASSERT_EQ(1, val);
		});

		ASSERT_FALSE(executed);
		p1.resolve(1);
		ASSERT_TRUE(executed);
		p2.resolve(2);
		ASSERT_TRUE(executed);
		p3.resolve(3);
		ASSERT_TRUE(executed);
	}
}
