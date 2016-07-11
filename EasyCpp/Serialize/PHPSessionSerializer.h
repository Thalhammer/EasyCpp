#pragma once
#include "Serializable.h"
#include "Serializer.h"
#include "../Bundle.h"

namespace EasyCpp
{
	namespace Serialize
	{
		class DLL_EXPORT PHPSessionSerializer : public Serializer
		{
		public:
			PHPSessionSerializer();
			~PHPSessionSerializer();

			virtual std::string serialize(const AnyValue& any) const override;
			virtual AnyValue deserialize(const std::string& str) override;
		private:
			AnyValue deserializeProperty(const std::string& str, size_t offset, size_t& endpos) const;
			std::string serializeProperty(AnyValue val) const;
		};
	}
}