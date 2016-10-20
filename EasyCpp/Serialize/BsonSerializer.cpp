#include "BsonSerializer.h"
#include <string>
#include "../BufferReader.h"
#include "../BufferWriter.h"

namespace EasyCpp
{
	namespace Serialize
	{
		BsonSerializer::BsonSerializer()
		{
		}


		BsonSerializer::~BsonSerializer()
		{
		}

		std::vector<uint8_t> BsonSerializer::writeDocument(Bundle& value) const
		{
			std::vector<uint8_t> res;
			BufferWriter wrt(res);

			// Document length
			wrt.writeInt32(0);

			for (auto& elem : value)
			{
				auto info = elem.second.type_info();
				if (info.isArithmetic())
				{
					if (info.isFloatingPoint())
					{
						wrt.writeUInt8(0x01); // double
						wrt.writeCString(elem.first);
						wrt.writeDouble(elem.second.as<double>());
					}
					else {
						wrt.writeUInt8(0x12); // int64
						wrt.writeCString(elem.first);
						wrt.writeInt64(elem.second.as<int64_t>());
					}
				}
				else if (elem.second.isSerializable())
				{
					wrt.writeUInt8(0x03);
					wrt.writeCString(elem.first);
					Bundle v = elem.second.serialize().as<Bundle>();
					auto val = this->writeDocument(v);
					wrt.writeBytes(val.data(), val.size());
				}
				else if (elem.second.isType<std::vector<AnyValue>>())
				{
					wrt.writeUInt8(0x04);
					wrt.writeCString(elem.first);
					std::vector<AnyValue> v = elem.second.as<std::vector<AnyValue>>();
					Bundle b;
					for (size_t i = 0; i < v.size(); i++) b.set(std::to_string(i), v[i]);
					auto val = this->writeDocument(b);
					wrt.writeBytes(val.data(), val.size());
				}
				else if (elem.second.isType<std::vector<uint8_t>>())
				{
					wrt.writeUInt8(0x05);
					wrt.writeCString(elem.first);
					std::vector<uint8_t> v = elem.second.as<std::vector<uint8_t>>();
					if (v.size() > INT32_MAX) throw std::runtime_error("Data too large");
					wrt.writeInt32((int32_t)v.size()); // Binary size
					wrt.writeUInt8(0x00); // Subtype
					wrt.writeBytes(v.data(), v.size()); // Data
				}
				else if (elem.second.isType<bool>())
				{
					wrt.writeUInt8(0x08);
					wrt.writeCString(elem.first);
					bool v = elem.second.as<bool>();
					wrt.writeUInt8(v ? 0x01 : 0x00);
				}
				else if (elem.second.isType<std::nullptr_t>())
				{
					wrt.writeUInt8(0x0A);
					wrt.writeCString(elem.first);
				}
				else if (elem.second.isConvertibleTo<std::string>())
				{
					wrt.writeUInt8(0x02);
					wrt.writeCString(elem.first);
					std::string v = elem.second.as<std::string>();
					if (v.size() > INT32_MAX - 1) throw std::runtime_error("String too large");
					wrt.writeInt32((int32_t)v.size() + 1);
					wrt.writeCString(v);
				}
			}

			wrt.writeUInt8(0x00);

			wrt.setPosition(0);
			wrt.setOverwrite(true);

			// Update Size value
			if (wrt.getSize() > INT32_MAX) throw std::runtime_error("Document to large");
			wrt.writeInt32((int32_t)wrt.getSize());
			return res;
		}

		std::string BsonSerializer::serialize(const AnyValue & any) const
		{
			auto value = any.as<Bundle>();
			auto res = writeDocument(value);
			return std::string(res.begin(), res.end());
		}

		AnyValue BsonSerializer::deserialize(const std::string & str)
		{
			Bundle res;
			std::vector<uint8_t> data(str.begin(), str.end());
			BufferReader rdr(data);
			if (rdr.getSize() < 4)
				throw std::invalid_argument("Document must be at least 4 byte long");

			rdr.readInt32();
			while (true)
			{
				uint8_t field_type = rdr.readUInt8();
				if (field_type == 0x00)
					break;

				std::string name = rdr.readCString();

				if (field_type == 0x01)
				{
					res.set(name, rdr.readDouble());
				}
				else if (field_type == 0x02)
				{
					int32_t len = rdr.readInt32();
					res.set(name, rdr.readString(len - 1));
					rdr.readUInt8(); // skip trailing 0
				}
				else if (field_type == 0x03)
				{
					// Document
					int32_t len = rdr.peekInt32();
					const uint8_t* ptr = rdr.readBytes(len);
					std::string document(ptr, ptr + len);
					res.set(name, this->deserialize(document));
				}
				else if (field_type == 0x04)
				{
					// Array
					int32_t len = rdr.peekInt32();
					const uint8_t* ptr = rdr.readBytes(len);
					std::string document(ptr, ptr + len);
					std::vector<AnyValue> bdocres;
					for (auto& elem : this->deserialize(document).as<Bundle>()) bdocres.push_back(elem.second);

					res.set(name, bdocres);
				}
				else if (field_type == 0x05)
				{
					// Binary Data
					int32_t len = rdr.readInt32();
					rdr.readUInt8(); // Subtype
									 // TODO: What to do with subtype ?
					auto ptr = rdr.readBytes(len);
					res.set(name, std::vector<uint8_t>(ptr, ptr + len));
				}
				else if (field_type == 0x06)
				{
					// Deprecated
				}
				else if (field_type == 0x07)
				{
					// ObjectID
					// We don't know what to do with it in standard implementations.
					rdr.readBytes(12);
				}
				else if (field_type == 0x08)
				{
					// Boolean
					res.set(name, (rdr.readUInt8() == 1));
				}
				else if (field_type == 0x09)
				{
					// UTC Date int64
					res.set(name, rdr.readInt64());
				}
				else if (field_type == 0x0A)
				{
					res.set(name, nullptr);
				}
				else if (field_type == 0x0B)
				{
					rdr.readCString();
					rdr.readCString();
				}
				else if (field_type == 0x0C)
				{
					// DB Pointer
					// Deprecated
					int32_t len = rdr.readInt32();
					rdr.readBytes(len + 12);
				}
				else if (field_type == 0x0D)
				{
					// Javascript code
					// Is this a good idea ?
					Bundle js;
					int32_t len = rdr.readInt32();
					js.set("js", rdr.readString(len));
					rdr.readUInt8(); // skip trailing 0
					js.set("scope", Bundle());
					res.set(name, js);
				}
				else if (field_type == 0x0E)
				{
					// Deprecated
					int32_t len = rdr.readInt32();
					rdr.readBytes(len);
				}
				else if (field_type == 0x0F)
				{
					// Javascript code with scope
					// Is this a good idea ?
					Bundle js;
					rdr.readInt32(); // code length
					int32_t jslen = rdr.readInt32();
					js.set("js", rdr.readString(jslen));
					rdr.readUInt8(); // skip trailing 0
					int32_t doclen = rdr.peekInt32();
					auto docptr = rdr.readBytes(doclen);
					js.set("scope", this->deserialize(std::string(docptr, docptr + doclen)));
					res.set(name, js);
				}
				else if (field_type == 0x10)
				{
					// INT32
					res.set(name, rdr.readInt32());
				}
				else if (field_type == 0x11)
				{
					// Timestamp
					res.set(name, rdr.readInt64());
				}
				else if (field_type == 0x12)
				{
					// Int64
					res.set(name, rdr.readInt64());
				}
				else if (field_type == 0xFF)
				{
					// Min key
				}
				else if (field_type == 0x7F)
				{
					// Max key
				}
			}

			return res;
		}
	}
}
