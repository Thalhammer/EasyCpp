#include <gtest/gtest.h>
#include <Scripting/LuaState.h>
#include <Scripting/LuaException.h>

using namespace EasyCpp::Scripting;

namespace UtilTest
{
	TEST(Lua, State)
	{
		LuaState state;
		state.openStandardLibs();
	}

	TEST(Lua, LoadString)
	{
		LuaState state;
		state.openStandardLibs();
		state.loadString("print \"Hello\"");
	}


	TEST(Lua, LoadStringInvalid)
	{
		LuaState state;
		state.openStandardLibs();
		ASSERT_THROW({
			state.loadString("1+1");
		}, LuaException);
	}

	TEST(Lua, PCall)
	{
		LuaState state;
		state.openStandardLibs();
		state.loadString("print \"Hello\"");
		state.pcall(0, LuaState::MULTRET());
	}

	TEST(Lua, CallLambda)
	{
		LuaState state;
		state.pushFunction([](LuaState& s) {
			std::cout << "Hello" << std::endl;
			return 0;
		});
		state.setGlobal("test");

		state.loadString("test()");
		state.pcall(0, 0);
	}
}