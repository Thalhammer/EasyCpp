#include <gtest/gtest.h>
#include <ThreadSafe.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(ThreadSafe, Default)
	{
		ThreadSafe<std::string> safeString;

		std::string string = *safeString;
		safeString->append("Test");
	}

	TEST(ThreadSafe, Copy)
	{
		ThreadSafe<std::string> safe1("Hallo");

		ThreadSafe<std::string> safe2(safe1);
	}
}