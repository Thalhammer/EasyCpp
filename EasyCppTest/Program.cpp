#include <gtest/gtest.h>
#include <Program.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Program, Execute)
	{
#ifdef _WIN32
		Program p("C:\\Windows\\System32\\cmd.exe", { "/c", "echo", "Hallo" });
		p.wait();
		uint32_t code = p.getExitCode();
		auto out = p.getStdout();
		auto test = out->read(1024);
		ASSERT_EQ(7, test.size());
#elif defined(__linux__)
		Program p("/bin/echo", { "Hallo" });
		p.wait();
		uint32_t code = p.getExitCode();
		auto out = p.getStdout();
		auto test = out->read(1024);
		ASSERT_EQ(6, test.size());
#endif
	}
}