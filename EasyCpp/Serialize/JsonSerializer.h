#pragma once
#include "Serializable.h"
#include "Serializer.h"
#include "../Bundle.h"

namespace Json
{
	class Value;
}

namespace EasyCpp
{
	namespace Serialize
	{
		class DLL_EXPORT JsonSerializer : public Serializer
		{
		public:
			JsonSerializer();
			~JsonSerializer();

			virtual std::string serialize(const AnyValue& a) const override;
			virtual AnyValue deserialize(const std::string& str) override;
		private:
			Json::Value toValue(AnyValue val) const;
			AnyValue toAny(const Json::Value& val);
		};
	}
}