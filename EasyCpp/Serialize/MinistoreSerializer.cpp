#include "MinistoreSerializer.h"
#include "../BufferWriter.h"

namespace EasyCpp
{
	namespace Serialize
	{
		MinistoreSerializer::MinistoreSerializer()
		{
		}

		MinistoreSerializer::~MinistoreSerializer()
		{
		}

		void MinistoreSerializer::setCodeTable(const std::vector<std::string>& table)
		{
			if (table.size() > 1536)
				throw std::runtime_error("Codetable is to large");
			_codeTable = table;
		}

		std::vector<std::string> MinistoreSerializer::getCodeTable() const
		{
			return _codeTable;
		}

		void MinistoreSerializer::setValueBackref(bool enabled)
		{
			_valueBackref = enabled;
		}

		bool MinistoreSerializer::isValueBackref()
		{
			return _valueBackref;
		}

		void MinistoreSerializer::setValueCodeTable(bool enabled)
		{
			_valueCodeTable = enabled;
		}

		bool MinistoreSerializer::isValueCodeTable()
		{
			return _valueCodeTable;
		}

		void MinistoreSerializer::writeDocument(BufferWriter & wrt, const Bundle & b, std::vector<std::string>& backref) const
		{
			size_t start = wrt.getPosition();
			wrt.writeUInt32(0);

			for (auto& elem : b)
			{
				writeDocumentElement(wrt, elem.first, elem.second, backref);
			}

			size_t end = wrt.getPosition();
			size_t len = end - start;
			if (len > UINT32_MAX) throw std::runtime_error("Document too large");
			wrt.setPosition(start);
			wrt.writeUInt32((uint32_t)len);
			wrt.setPosition(end);
		}

		void MinistoreSerializer::writeArray(BufferWriter & wrt, std::vector<AnyValue>& b, std::vector<std::string>& backref) const
		{
			size_t start = wrt.getPosition();
			wrt.writeUInt32(0);

			for (auto& elem : b)
			{
				this->writeToken(wrt, 0x0, 0, "", elem, backref);
			}

			size_t end = wrt.getPosition();
			size_t len = end - start;
			if (len > UINT32_MAX) throw std::runtime_error("Document too large");
			wrt.setPosition(start);
			wrt.writeUInt32((uint32_t)len);
			wrt.setPosition(end);
		}

		void MinistoreSerializer::writeDocumentElement(BufferWriter& wrt, const std::string & key, const AnyValue & val, std::vector<std::string>& backref) const
		{
			uint8_t refid = 0;
			uint8_t start = 0;
			for (size_t i = 0; i < _codeTable.size(); i++)
			{
				if (_codeTable[i] == key)
				{
					start = (uint8_t)(i % 256 + 9);
					refid = (uint8_t)(i / 256);
					break;
				}
			}
			if (start == 0)
			{
				for (size_t i = 0; i < backref.size(); i++)
				{
					if (backref[i] == key)
					{
						start = (uint8_t)(i / 256 + 2);
						refid = (uint8_t)(i % 256);
						break;
					}
				}
				if (start == 0)
				{
					if (backref.size() == 2048)
					{
						start = 1;
					}
					else {
						backref.push_back(key);
						start = (uint8_t)((backref.size() - 1) / 256 + 2);
						refid = (backref.size() - 1) % 256;
					}
				}
			}
			if (start == 0)
				throw std::logic_error("Internal error");

			this->writeToken(wrt, start, refid, key, val, backref);
		}

		void MinistoreSerializer::writeToken(BufferWriter & wrt, uint8_t start, uint8_t refid, const std::string& key, const AnyValue & val, std::vector<std::string>& backref) const
		{
			uint16_t val_refid = UINT16_MAX;
			// Check type
			uint8_t type = 0xf;
			if (val.isType<std::nullptr_t>())
				type = 0x0;
			else if (val.isSerializable())
				type = 0x1;
			else if (val.isType<std::vector<AnyValue>>())
				type = 0x2;
			else if (val.type_info().isFloatingPoint())
				type = 0x3;
			else if (val.isType<bool>())
				type = (val.as<bool>() ? 0x4 : 0x5);
			else if (val.type_info().isIntegral())
			{
				int64_t v = val.as<int64_t>();
				if (v <= INT8_MAX && v >= INT8_MIN)
					type = 0xB;
				else if (v <= INT16_MAX && v >= INT16_MIN)
					type = 0xC;
				else if (v <= INT32_MAX && v >= INT32_MIN)
					type = 0xD;
				else type = 0xE;
			}
			else if (val.isConvertibleTo<std::string>())
			{
				// TODO: Backref usage
				std::string str = val.as<std::string>();
				if (_valueCodeTable)
				{
					for (size_t i = 0; i < _codeTable.size(); i++)
					{
						if (_codeTable[i] == str)
						{
							type = 0xA;
							val_refid = (uint16_t)i | 0x8000;	// Codetable flag
							break;
						}
					}
				}
				if (val_refid == UINT16_MAX)
				{
					if (str.size() <= UINT8_MAX)
						type = 0x6;
					else if (str.size() <= UINT16_MAX)
						type = 0x7;
					else if (str.size() <= 0xffffff)
						type = 0x8;
					else if (str.size() <= UINT32_MAX)
						type = 0x9;
					else throw std::runtime_error("String to large");
				}
			}
			else throw std::runtime_error("Cannot write type:" + std::string(val.type().name()));

			uint8_t token = start << 4 | type;

			wrt.writeUInt8(token);
			if (start != 0)
			{
				if (start == 1)
					wrt.writeCString(key);
				else wrt.writeUInt8(refid);
			}

			if (type != 0x0 && type != 0x4 && type != 0x5)
			{
				if (type == 0x1)
				{
					Bundle b = val.serialize().as<Bundle>();
					this->writeDocument(wrt, b, backref);
				}
				else if (type == 0x2)
				{
					std::vector<AnyValue> v = val.as<std::vector<AnyValue>>();
					this->writeArray(wrt, v, backref);
				}
				else if (type == 0x3)
				{
					double v = val.as<double>();
					wrt.writeDouble(v);
				}
				else if (type == 0x6)
				{
					std::string v = val.as<std::string>();
					wrt.writeUInt8((uint8_t)v.size());
					wrt.writeString(v);
				}
				else if (type == 0x7)
				{
					std::string v = val.as<std::string>();
					wrt.writeUInt16((uint16_t)v.size());
					wrt.writeString(v);
				}
				else if (type == 0x8)
				{
					std::string v = val.as<std::string>();
					wrt.writeUInt8((uint8_t)(v.size() >> 16));
					wrt.writeUInt16((uint16_t)v.size());
					wrt.writeString(v);
				}
				else if (type == 0x9)
				{
					std::string v = val.as<std::string>();
					if (v.size() > UINT32_MAX) throw std::runtime_error("String too large");
					wrt.writeUInt32((uint32_t)v.size());
					wrt.writeString(v);
				}
				else if (type == 0xA)
				{
					wrt.writeUInt16(val_refid);
				}
				else if (type == 0xB)
				{
					int8_t v = val.as<int8_t>();
					wrt.writeInt8(v);
				}
				else if (type == 0xC)
				{
					int16_t v = val.as<int16_t>();
					wrt.writeInt16(v);
				}
				else if (type == 0xD)
				{
					int32_t v = val.as<int32_t>();
					wrt.writeInt32(v);
				}
				else if (type == 0xE)
				{
					int64_t v = val.as<int64_t>();
					wrt.writeInt64(v);
				}
			}
		}

		std::string MinistoreSerializer::serialize(const AnyValue & any) const
		{
			std::vector<std::string> backrefTable;
			std::vector<uint8_t> res;
			BufferWriter wrt(res);
			wrt.setOverwrite(true);

			// Write Document
			writeDocument(wrt, any.as<Bundle>(), backrefTable);
			// Write Backref table
			for (const auto& ref : backrefTable)
				wrt.writeCString(ref);
			return std::string(res.begin(), res.end());
		}

		AnyValue MinistoreSerializer::deserialize(const std::string & str)
		{
			return AnyValue();
		}
	}
}
