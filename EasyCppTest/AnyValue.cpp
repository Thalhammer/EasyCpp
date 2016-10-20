#include <gtest/gtest.h>
#include <AnyValue.h>
#include <Serialize/Serializable.h>
#include <Bundle.h>
#include <DynamicObjectHelper.h>

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

	TEST(AnyValue, CastOperator)
	{
		AnyValue a = std::string("20");

		std::string s = a;
		ASSERT_EQ(std::string("20"), s);
		int i = a;
		ASSERT_EQ(20, i);
	}

	TEST(AnyValue, NullNotNullptr)
	{
		AnyValue a((void*)0x00);
		ASSERT_FALSE(a.isType<nullptr_t>());
	}

	class SimpleSample
	{
	public:
		SimpleSample()
		{
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
}

namespace EasyCpp
{
	template<>
	struct TypeCheck<EasyCppTest::SimpleSample*>
	{
		typedef EasyCppTest::SimpleSample Type;
		static bool IsSerializable()
		{
			return false;
		}

		static bool IsDynamicObject()
		{
			return true;
		}

		static const Serialize::Serializable& AsSerializable(const EasyCppTest::SimpleSample* value)
		{
			throw std::runtime_error("This Anyvalue does not implement Serializable");
		}

		static std::shared_ptr<DynamicObject> AsDynamicObject(EasyCppTest::SimpleSample* value)
		{
			class DynamicWrapper : public DynamicObjectHelper
			{
			public:
				DynamicWrapper(EasyCppTest::SimpleSample* value)
					:_value(value)
				{
					this->addFunction("setText", AnyFunction::fromDynamicFunction([this](const AnyArray& params) {
						this->_value->setText(params[0].as<std::string>());
						return AnyValue();
					}));
				}
			private:
				EasyCppTest::SimpleSample* _value;
			};
			return std::make_shared<DynamicWrapper>(value);
		}
	};

	template<>
	struct TypeCheck<EasyCppTest::SimpleSample>
	{
		typedef EasyCppTest::SimpleSample Type;
		static bool IsSerializable()
		{
			return TypeCheck<EasyCppTest::SimpleSample*>::IsSerializable();
		}

		static bool IsDynamicObject()
		{
			return TypeCheck<EasyCppTest::SimpleSample*>::IsDynamicObject();
		}

		static const Serialize::Serializable& AsSerializable(const EasyCppTest::SimpleSample& value)
		{
			return TypeCheck<EasyCppTest::SimpleSample*>::AsSerializable(&value);
		}

		static std::shared_ptr<DynamicObject> AsDynamicObject(EasyCppTest::SimpleSample& value)
		{
			return TypeCheck<EasyCppTest::SimpleSample*>::AsDynamicObject(&value);
		}
	};
}
namespace EasyCppTest
{
	TEST(AnyValue, ExternalTypeCheck)
	{
		SimpleSample sample;
		AnyValue val(&sample);

		ASSERT_TRUE(val.isDynamicObject());
		auto& obj = val.asDynamicObject();
		obj.callFunction("setText", { std::string("test_value") });

		ASSERT_EQ("test_value", sample.getText());
	}
}