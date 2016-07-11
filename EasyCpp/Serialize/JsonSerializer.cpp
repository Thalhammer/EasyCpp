#include "JsonSerializer.h"
#include <vector>
#include <json/json.h>
#include <stdexcept>

namespace EasyCpp
{
	namespace Serialize
	{

		JsonSerializer::JsonSerializer()
		{
		}


		JsonSerializer::~JsonSerializer()
		{
		}

		std::string JsonSerializer::serialize(const AnyValue & a) const
		{
			return Json::FastWriter().write(this->toValue(a));
		}

		AnyValue JsonSerializer::deserialize(const std::string & str)
		{
			Json::Value root;
			Json::CharReaderBuilder rbuilder;
			rbuilder["collectComments"] = false;
			std::unique_ptr<Json::CharReader> rdr(rbuilder.newCharReader());
			if (!rdr->parse(str.data(), str.data() + str.size(), &root, nullptr))
				throw std::runtime_error("Invalid json string");
			return this->toAny(root);
		}

		Json::Value JsonSerializer::toValue(AnyValue val) const
		{
			auto info = val.type_info();
			if (val.isType<bool>())
			{
				bool v = val.as<bool>();
				return Json::Value(v);
			}
			else if (info.isIntegral())
			{
				if (info.isUnsigned())
				{
					uint64_t v = val.as<uint64_t>();
					return Json::Value((Json::Value::UInt64)v);
				}
				else {
					int64_t v = val.as<int64_t>();
					return Json::Value((Json::Value::Int64)v);
				}
			}
			else if (info.isFloatingPoint())
			{
				double v = val.as<double>();
				return Json::Value(v);
			}
			else if (val.isType<std::vector<AnyValue>>())
			{
				Json::Value res = Json::arrayValue;
				std::vector<AnyValue> v = val.as < std::vector<AnyValue>>();
				for (AnyValue any : v)
					res.append(this->toValue(any));
				return res;
			}
			else if (val.isType<Bundle>())
			{
				Json::Value res = Json::objectValue;
				Bundle b = val.as<Bundle>();
				for (const auto& val : b)
				{
					res[val.first] = this->toValue(val.second);
				}
				return res;
			}
			else if (val.isType<std::nullptr_t>())
			{
				return Json::nullValue;
			}
			else if (val.isSerializable())
			{
				return this->toValue(val.serialize());
			}
			else if (val.isConvertibleTo<std::string>())
			{
				std::string v = val.as<std::string>();
				return Json::Value(v);
			}
			else
			{
				throw std::logic_error("Type is not convertible.");
			}
		}

		AnyValue JsonSerializer::toAny(const Json::Value & val)
		{
			std::vector<AnyValue> arr;
			Bundle b;
			switch (val.type())
			{
			case Json::ValueType::arrayValue:
				for (auto e : val) arr.push_back(this->toAny(e));
				return arr;
			case Json::ValueType::booleanValue:
				return val.asBool();
			case Json::ValueType::intValue:
				return (int64_t)val.asInt64();
			case Json::ValueType::nullValue:
				return nullptr;
			case Json::ValueType::objectValue:
				for (Json::ValueConstIterator it = val.begin(); it != val.end(); it++)
				{
					std::string name = it.name();
					b.set(name, this->toAny(*it));
				}
				return b;
			case Json::ValueType::realValue:
				return val.asDouble();
			case Json::ValueType::stringValue:
				return val.asString();
			case Json::ValueType::uintValue:
				return (uint64_t)val.asUInt64();
			}
			return AnyValue();
		}
	}
}
