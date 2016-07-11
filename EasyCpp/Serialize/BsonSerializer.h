#pragma once
#include <vector>
#include <cstdint>
#include "Serializer.h"
#include "../Bundle.h"

namespace EasyCpp
{
	namespace Serialize
	{
		class DLL_EXPORT BsonSerializer : public Serializer
		{
		public:
			BsonSerializer();
			~BsonSerializer();

			// Geerbt über Serializer
			virtual std::string serialize(const AnyValue & any) const override;
			virtual AnyValue deserialize(const std::string & str) override;
		private:
			std::vector<uint8_t> writeDocument(Bundle& value) const;
		};
	}
}