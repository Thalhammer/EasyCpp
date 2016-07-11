#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "../Bundle.h"
#include "Serializer.h"

namespace EasyCpp
{
	class BufferWriter;
	class AnyValue;
	namespace Serialize
	{
		//! Support for Thalhammer MINIStore
		class DLL_EXPORT MinistoreSerializer : public Serializer
		{
		public:
			MinistoreSerializer();
			virtual ~MinistoreSerializer();

			void setCodeTable(const std::vector<std::string>& table);
			std::vector<std::string> getCodeTable() const;

			void setValueBackref(bool enabled);
			bool isValueBackref();

			void setValueCodeTable(bool enabled);
			bool isValueCodeTable();

			// Geerbt über Serializer
			virtual std::string serialize(const AnyValue & any) const override;
			virtual AnyValue deserialize(const std::string & str) override;

		private:
			void writeDocument(BufferWriter& wrt, const Bundle& b, std::vector<std::string>& backref) const;
			void writeArray(BufferWriter& wrt, std::vector<AnyValue>& b, std::vector<std::string>& backref) const;
			void writeDocumentElement(BufferWriter& wrt, const std::string& key, const AnyValue& val, std::vector<std::string>& backref) const;
			void writeToken(BufferWriter& wrt, uint8_t start, uint8_t refid, const std::string& key, const AnyValue& val, std::vector<std::string>& backref) const;

			std::vector<std::string> _codeTable;

			bool _valueBackref = false;
			bool _valueCodeTable = true;
		};
	}
}
