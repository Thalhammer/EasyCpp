#include <gtest/gtest.h>
#include <Backtrace.h>
#include <iostream>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Backtrace, Create)
	{
		Backtrace b;
		b.print(std::cout);
	}

	TEST(Backtrace, ToString)
	{
		Backtrace b;
		std::string r = b.print();
	}
}