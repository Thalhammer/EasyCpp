#include "DynamicObjectHelper.h"
#include "AnyValue.h"

namespace EasyCpp
{
	AnyValue DynamicObjectHelper::getProperty(const std::string & name)
	{
		if (_properties.count(name) == 0)
			return AnyValue();
		return _properties.at(name).first();
	}

	std::vector<std::string> DynamicObjectHelper::getProperties()
	{
		std::vector<std::string> res;
		for (const auto& e : _properties)
		{
			res.push_back(e.first);
		}
		return res;
	}

	void DynamicObjectHelper::setProperty(const std::string & name, AnyValue value)
	{
		if (_properties.count(name) == 0)
			return;
		_properties.at(name).second(value);
	}

	AnyValue DynamicObjectHelper::callFunction(const std::string & name, const std::vector<AnyValue>& params)
	{
		if (_functions.count(name) == 0)
			return AnyValue();
		return _functions.at(name).call(params);
	}

	std::vector<std::string> DynamicObjectHelper::getFunctions()
	{
		std::vector<std::string> res;
		for (const auto& e : _functions)
		{
			res.push_back(e.first);
		}
		return res;
	}

	void DynamicObjectHelper::addProperty(const std::string & name, getter_fn get, setter_fn set)
	{
		std::string newname = name;
		size_t pos = newname.find_last_of(":");
		if (pos != std::string::npos) {
			newname = newname.substr(pos + 1);
		}
		_properties.insert({ newname, {get, set} });
	}

	void DynamicObjectHelper::addFunction(const std::string & name, AnyFunction fn)
	{
		std::string newname = name;
		size_t pos = newname.find_last_of(":");
		if (pos != std::string::npos) {
			newname = newname.substr(pos + 1);
		}
		_functions.insert({ newname, fn });
	}
}