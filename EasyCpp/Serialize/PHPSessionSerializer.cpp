#include "PHPSessionSerializer.h"
#include <vector>

namespace EasyCpp
{
	namespace Serialize
	{
		PHPSessionSerializer::PHPSessionSerializer()
		{
		}

		PHPSessionSerializer::~PHPSessionSerializer()
		{
		}

		std::string PHPSessionSerializer::serialize(const AnyValue & any) const
		{
			std::string res = "";
			for (auto elem : any.as<Bundle>())
			{
				res += elem.first + "|" + this->serializeProperty(elem.second);
			}
			return res;
		}

		AnyValue PHPSessionSerializer::deserialize(const std::string & str)
		{
			Bundle b;
			size_t offset = 0;
			do {
				size_t pos = str.find("|", offset);
				if (pos == std::string::npos)
					break;
				std::string key = str.substr(offset, pos - offset);
				AnyValue val = this->deserializeProperty(str, pos + 1, pos);
				b.set(key, val);
				offset = pos;
			} while (offset != std::string::npos);
			return b;
		}

		AnyValue PHPSessionSerializer::deserializeProperty(const std::string & str, size_t offset, size_t& endpos) const
		{
			AnyValue res;
			std::string type = str.substr(offset, 1);
			offset = offset + 2;
			if (type == "b") // Boolean
			{
				if (str[offset] == '0')
					res = false;
				else res = true;
				endpos = offset + 2;
				return res;
			}
			else if (type == "s") // String
			{
				size_t length_endpos = str.find(":", offset);
				uint64_t length = std::stoull(str.substr(offset, length_endpos - offset));
				offset = length_endpos + 2; // Ignore ':' and '"'
				std::string value = str.substr(offset, (size_t)length);
				endpos = offset + (size_t)length + 2; // Ignore '"' and ';'
				return value;
			}
			else if (type == "O") // Object of class
			{ // 8:"stdClass":11:{
				Bundle res;
				size_t length_endpos = str.find(":", offset);
				uint64_t length = std::stoull(str.substr(offset, length_endpos - offset));
				offset = length_endpos + 2; // Ignore ':' and '"'
				std::string classname = str.substr(offset, (size_t)length);
				offset = offset + (size_t)length + 2; // Ignore '"' and ':'
				size_t numelems_endpos = str.find(":", offset);
				uint64_t numelems = std::stoull(str.substr(offset, numelems_endpos - offset));
				offset = numelems_endpos + 2; // Ignore '"' and '{'
				for (size_t i = 0; i < numelems; i++)
				{
					std::string key = this->deserializeProperty(str, offset, offset).as<std::string>();
					AnyValue val = this->deserializeProperty(str, offset, offset);
					res.set(key, val);
				}
				endpos = offset + 1; // Ignore '}'
				return res;
			}
			else if (type == "N")
			{
				endpos = offset;
				return nullptr;
			}
			else if (type == "i")
			{
				size_t pos = str.find(";", offset);
				int64_t num = std::stoll(str.substr(offset, endpos - offset));
				endpos = pos + 1;
				return num;
			}
			else if (type == "a")
			{
				Bundle res;
				size_t numelems_endpos = str.find(":", offset);
				uint64_t numelems = std::stoull(str.substr(offset, numelems_endpos - offset));
				offset = numelems_endpos + 2; // Ignore '"' and '{'
				for (size_t i = 0; i < numelems; i++)
				{
					std::string key = this->deserializeProperty(str, offset, offset).as<std::string>();
					AnyValue val = this->deserializeProperty(str, offset, offset);
					res.set(key, val);
				}
				endpos = offset + 1; // Ignore '}'
				return res;
			}
			else if (type == "d")
			{
				size_t pos = str.find(";", offset);
				auto num = std::stod(str.substr(offset, endpos - offset));
				endpos = pos + 1;
				return num;
			}
			else {
				throw std::runtime_error("Unknown element type " + type);
			}
		}

		std::string PHPSessionSerializer::serializeProperty(AnyValue val) const
		{
			auto info = val.type_info();
			if (val.isType<bool>())
			{
				bool v = val.as<bool>();
				return "b:" + std::to_string(v ? 1 : 0) + ";";
			}
			else if (info.isIntegral())
			{
				if (info.isUnsigned())
				{
					uint64_t v = val.as<uint64_t>();
					return "i:" + std::to_string(v) + ";";
				}
				else {
					int64_t v = val.as<int64_t>();
					return "i:" + std::to_string(v) + ";";
				}
			}
			else if (info.isFloatingPoint())
			{
				double v = val.as<double>();
				return "d:" + std::to_string(v) + ";";
			}
			else if (val.isType<std::vector<AnyValue>>())
			{
				std::string res = "a:";
				std::vector<AnyValue> v = val.as < std::vector<AnyValue>>();
				res += std::to_string(v.size()) + ":{";
				for (size_t i = 0; i < v.size(); i++) {
					res += this->serializeProperty(i) + this->serializeProperty(v[i]);
				}
				res += "}";
				return res;
			}
			else if (val.isType<Bundle>())
			{
				std::string res = "a:";
				Bundle v = val.as<Bundle>();
				size_t count = 0;
				for (auto e : v)
					count++;
				res += std::to_string(count) + ":{";
				for (auto e : v) {
					res += this->serializeProperty(e.first) + this->serializeProperty(e.second);
				}
				res += "}";
				return res;
			}
			else if (val.isType<std::nullptr_t>())
			{
				return "N;";
			}
			else if (val.isSerializable())
			{
				return this->serializeProperty(val.serialize());
			}
			else if (val.isConvertibleTo<std::string>())
			{
				std::string v = val.as<std::string>();
				return std::string("s:") + std::to_string(v.length()) + ":\"" + v + "\";";
			}
			else
			{
				throw std::logic_error(std::string("Type ") + val.type().name() + " is not convertible.");
			}
		}
	}
}