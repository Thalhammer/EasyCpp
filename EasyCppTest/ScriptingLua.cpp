#include <gtest/gtest.h>
#include <Scripting/LuaState.h>
#include <Scripting/LuaException.h>
#include <Scripting/LuaScriptEngineFactory.h>
#include <Bundle.h>
#include <AnyFunction.h>

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

	TEST(Lua, ScriptEngine)
	{
		auto factory = std::make_shared<LuaScriptEngineFactory>();
		{
			auto engine = factory->getScriptEngine();
			engine->put("x", 100);
			engine->eval("x = x*x");
			auto x = engine->get("x");
			ASSERT_EQ(10000, x.as<int>());
		}
		{
			auto engine = factory->getScriptEngine();
			engine->put("x", EasyCpp::Bundle({
				{"num1", 100},
				{"num2", 200}
			}));
			engine->eval("y = x.num1 + x.num2");
			auto x = engine->get("y");
			ASSERT_EQ(300, x.as<int>());
		}
		{
			auto engine = factory->getScriptEngine();
			engine->put("fn", EasyCpp::AnyFunction::fromFunction(std::function<int(int,int)>([](int a, int b) {
				return a > b ? a : b;
			})));
			engine->eval("y = fn(100, 1000)");
			auto x = engine->get("y");
			ASSERT_EQ(1000, x.as<int>());
		}
	}
}