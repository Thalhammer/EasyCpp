#include <gtest/gtest.h>
#include <AnyFunction.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(AnyFunction, Basic)
	{
		std::function<int()> fn = []() { return 0; };
		AnyFunction afn = AnyFunction::fromFunction(fn);

		AnyValue res = afn.call({});
	}

	TEST(AnyFunction, BasicWithParams)
	{
		std::function<int(int,int)> fn = [](int i1, int i2) { return i1 + i2; };
		AnyFunction afn = AnyFunction::fromFunction(fn);

		AnyValue res = afn.call({10, 20});
		ASSERT_TRUE(res.isType<int>());
		ASSERT_EQ(30, res.as<int>());
	}

	TEST(AnyFunction, BasicWithParam)
	{
		std::function<int(int)> fn = [](int i1) { return i1; };
		AnyFunction afn = AnyFunction::fromFunction(fn);

		AnyValue res = afn.call({ 10 });
		ASSERT_TRUE(res.isType<int>());
		ASSERT_EQ(10, res.as<int>());
	}

	TEST(AnyFunction, VoidFunction)
	{
		std::function<void()> fn = []() { };
		AnyFunction afn = AnyFunction::fromFunction(fn);

		AnyValue res = afn.call({});
	}

	TEST(AnyFunction, ThrowMissingParams)
	{
		std::function<void(int, int)> fn = [](int i, int i2) {};
		AnyFunction afn = AnyFunction::fromFunction(fn);

		ASSERT_THROW({
			AnyValue res = afn.call({ 10});
		}, std::out_of_range);
	}

	TEST(AnyFunction, ParamTypes)
	{
		std::function<double(int, unsigned int)> fn = [](int i, unsigned int i2) { return 0.1; };
		AnyFunction afn = AnyFunction::fromFunction(fn);

		auto ret_type = afn.getReturnType();
		auto params = afn.getParameterTypes();

		ASSERT_EQ(typeid(double), ret_type.getStdTypeInfo());
		ASSERT_EQ(2, params.size());
		ASSERT_EQ(typeid(int), params[0].getStdTypeInfo());
		ASSERT_EQ(typeid(unsigned int), params[1].getStdTypeInfo());
	}

	class TestClass {
	public:
		static int smethod(int i) { return 1; }
		int method(int i) { return 1; }
	};

	TEST(AnyFunction, FunctionPointer)
	{
		auto member = AnyFunction::fromFunction(&TestClass::method);
		auto ret_type = member.getReturnType();
		auto params = member.getParameterTypes();

		ASSERT_EQ(typeid(int), ret_type.getStdTypeInfo());
		ASSERT_EQ(2, params.size());
		ASSERT_EQ(typeid(TestClass*), params[0].getStdTypeInfo());
		ASSERT_EQ(typeid(int), params[1].getStdTypeInfo());

		auto stati = AnyFunction::fromFunction(&TestClass::smethod);
		ret_type = stati.getReturnType();
		params = stati.getParameterTypes();

		ASSERT_EQ(typeid(int), ret_type.getStdTypeInfo());
		ASSERT_EQ(1, params.size());
		ASSERT_EQ(typeid(int), params[0].getStdTypeInfo());
	}

	TEST(AnyFunction, DynamicFunction)
	{
		auto fn = AnyFunction::fromDynamicFunction([](const AnyArray& args) -> AnyValue {
			return 10;
		});

		ASSERT_TRUE(fn.hasVarArgs());
		ASSERT_TRUE(fn.hasVarReturnValue());
	}
}