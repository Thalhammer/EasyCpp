#include <gtest/gtest.h>
#include <AnyFunction.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(AnyFunction, Basic)
	{
		std::function<int()> fn = []() { return 0; };
		AnyFunction afn(fn);

		AnyValue res = afn.call({});
	}

	TEST(AnyFunction, BasicWithParams)
	{
		std::function<int(int,int)> fn = [](int i1, int i2) { return i1 + i2; };
		AnyFunction afn(fn);

		AnyValue res = afn.call({10, 20});
		ASSERT_TRUE(res.isType<int>());
		ASSERT_EQ(30, res.as<int>());
	}

	TEST(AnyFunction, VoidFunction)
	{
		std::function<void()> fn = []() { };
		AnyFunction afn(fn);

		AnyValue res = afn.call({});
	}

	TEST(AnyFunction, ThrowMissingParams)
	{
		std::function<void(int, int)> fn = [](int i, int i2) {};
		AnyFunction afn(fn);

		ASSERT_THROW({
			AnyValue res = afn.call({});
		}, std::out_of_range);
	}

	TEST(AnyFunction, ParamTypes)
	{
		std::function<double(int, unsigned int)> fn = [](int i, unsigned int i2) { return 0.1; };
		AnyFunction afn(fn);

		auto ret_type = afn.getReturnType();
		auto params = afn.getParameterTypes();

		ASSERT_EQ(typeid(double), ret_type.getStdTypeInfo());
		ASSERT_EQ(2, params.size());
		ASSERT_EQ(typeid(int), params[0].getStdTypeInfo());
		ASSERT_EQ(typeid(unsigned int), params[1].getStdTypeInfo());
	}
}