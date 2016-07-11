#pragma once
namespace EasyCpp
{
	class AnyValue;
	namespace Serialize
	{
		class Serializable
		{
		public:
			virtual AnyValue toAnyValue() const = 0;
			virtual void fromAnyValue(const AnyValue& state) = 0;
		};
	}
}