#include <gtest/gtest.h>
#include <Scripting/ScriptEngineManager.h>
#include <DynamicObjectHelper.h>

using namespace EasyCpp::Scripting;
using namespace EasyCpp;

namespace EasyCppTest
{
	// Sampleclass used in script
	class Sample : public DynamicObjectHelper
	{
	public:
		Sample()
		{
			DOH_FUNCTION(Sample::getText);
			DOH_FUNCTION(Sample::setText);
			DOH_PROPERTY(Sample::_text);
			setText("Hello");
		}

		std::string getText()
		{
			return _text;
		}

		void setText(const std::string& text)
		{
			_text = text;
		}

	private:
		std::string _text;
	};

	TEST(DynamicObject, ScriptEngine)
	{
		{
			auto engine = ScriptEngineManager::getEngineByName("lua");
			auto sample = std::make_shared<Sample>();
			engine->put("sample", sample);
			engine->eval("test = sample:getText()\n"
						 "test = test .. \" World\"\n"
						 "sample:setText(test)\n");
			ASSERT_EQ(std::string("Hello World"), sample->getText());
		}

		{
			auto engine = ScriptEngineManager::getEngineByName("lua");
			engine->eval("function test(var1) return var1 .. \"lua\" end");
			EasyCpp::AnyFunction function = engine->get("test");

			std::string test = function.call({ "test " });
			ASSERT_EQ("test lua", test);
		}
	}
}