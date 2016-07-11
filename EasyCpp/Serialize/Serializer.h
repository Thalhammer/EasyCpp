#pragma once
#include <string>

namespace EasyCpp
{
	class AnyValue;
	namespace Serialize
	{
		class Serializer
		{
		public:
			virtual std::string serialize(const AnyValue& any) const = 0;
			virtual AnyValue deserialize(const std::string& str) = 0;
		};
	}
}