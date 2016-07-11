#include "Bundle.h"


namespace EasyCpp
{
	Bundle::Bundle()
	{
	}

	Bundle::Bundle(const std::map<std::string, AnyValue>& data)
		:_data(data)
	{
	}

	Bundle::~Bundle()
	{
	}

	void Bundle::set(const std::string& name, const AnyValue & val)
	{
		_data.insert(std::pair<std::string, AnyValue>(name, val));
	}

	bool Bundle::isSet(const std::string & name) const
	{
		return _data.count(name) != 0;
	}

	bool Bundle::isEmpty() const
	{
		return _data.empty();
	}

	AnyValue & Bundle::operator[](const std::string & idx)
	{
		return _data[idx];
	}

	const AnyValue & Bundle::operator[](const std::string & idx) const
	{
		return _data.at(idx);
	}

	AnyValue Bundle::toAnyValue() const
	{
		return *this;
	}

	void Bundle::fromAnyValue(const AnyValue & state)
	{
		if (!state.isType<Bundle>())
			return;
		*this = state.as<Bundle>();
	}

	AnyValue Bundle::get(const std::string& name) const
	{
		auto it = _data.find(name);
		if (it != _data.end())
			return it->second;
		return AnyValue();
	}
}