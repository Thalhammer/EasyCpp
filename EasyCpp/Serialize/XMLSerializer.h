#pragma once
#include "Serializer.h"
#include "../AnyValue.h"

namespace EasyCpp
{
	namespace Serialize
	{
		class DLL_EXPORT XMLSerializer : public Serializer
		{
		public:
			XMLSerializer();
			~XMLSerializer();

			virtual std::string serialize(const AnyValue& a) const override;
			virtual AnyValue deserialize(const std::string& str) override;
		};
	}
}