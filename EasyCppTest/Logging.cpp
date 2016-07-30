#include <gtest/gtest.h>
#include <Logging/SystemLogger.h>
#include <Logging/ConsoleLogger.h>

using namespace EasyCpp::Logging;

namespace EasyCppTest
{
	TEST(Logging, SystemLogger)
	{
		auto logger = std::make_shared<SystemLogger>("EasyCppTest");
		logger->Critical("Error !", {});
		logger->Warning("Warning !", {});
		logger->Info("Info !", {});
	}

	TEST(Logging, ConsoleLogger)
	{
		auto logger = std::make_shared<ConsoleLogger>("EasyCppTest");
		logger->Critical("Error !", {});
		logger->Warning("Warning !", {});
		logger->Info("Info !", {});
	}
}