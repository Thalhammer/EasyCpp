#include <gtest/gtest.h>
#include <AnyValue.h>
#include <Serialize/Serializable.h>
#include <Bundle.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(AnyValue, Int)
	{
		int val = 10;
		AnyValue any(val);
		ASSERT_TRUE(any.isType<int>());
		ASSERT_TRUE(any.isConvertibleTo<int>());
		ASSERT_EQ(val, any.as<int>());
	}

	TEST(AnyValue, Default)
	{
		AnyValue any;
		ASSERT_TRUE(any.isType<nullptr_t>());
	}

	TEST(AnyValue, Convert)
	{
		AnyValue any(true);
		ASSERT_TRUE(any.isType<bool>());
		ASSERT_TRUE(any.isConvertibleTo<std::string>());
		ASSERT_EQ(std::string("true"), any.as<std::string>());
	}

	TEST(AnyValue, isSerializable)
	{
		class SimpleSerializable : public Serialize::Serializable
		{
		public:
			virtual AnyValue toAnyValue() const override
			{
				return AnyValue();
			}
			virtual void fromAnyValue(const AnyValue& any) override
			{}
		};

		SimpleSerializable t;
		AnyValue any(t);
		ASSERT_TRUE(any.isType<SimpleSerializable>());
		ASSERT_TRUE(any.isSerializable());

		AnyValue any2(true);
		ASSERT_TRUE(any2.isType<bool>());
		ASSERT_FALSE(any2.isSerializable());
	}
}