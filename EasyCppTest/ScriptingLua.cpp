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

	class SampleDynamicObject: public EasyCpp::DynamicObject
	{
		int test = 100;
	public:
		// Geerbt über DynamicObject
		virtual EasyCpp::AnyValue getProperty(const std::string & name) override
		{
			if(name == "test"){return test;}
			throw std::runtime_error("Property not found");
		}
		virtual std::vector<std::string> getProperties() override
		{
			return{ "test" };
		}
		virtual void setProperty(const std::string & name, EasyCpp::AnyValue value) override
		{
			if (name == "test") { test = value.as<int>(); }
		}
		virtual EasyCpp::AnyValue callFunction(const std::string & name, const std::vector<EasyCpp::AnyValue>& params) override
		{
			if (name == "testfn") return 100;
			throw std::runtime_error("Function not found");
		}
		virtual std::vector<std::string> getFunctions() override
		{
			return{ "testfn" };
		}
	};

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
		{
			auto engine = factory->getScriptEngine();
			engine->put("x", SampleDynamicObject());
			engine->eval("x.test = 200");
			engine->eval("y = x:testfn() + x.test");
			auto x = engine->get("y");
			ASSERT_EQ(300, x.as<int>());
		}
	}
}