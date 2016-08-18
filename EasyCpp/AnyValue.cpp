#include "AnyValue.h"
#include "Bundle.h"

namespace EasyCpp
{
	AnyValue::AnyValue()
		:AnyValue(nullptr)
	{
	}

	AnyValue::~AnyValue()
	{

	}

	bool AnyValue::isType(const TypeInfo& info) const
	{
		return isType(info.getStdTypeInfo());
	}

	bool AnyValue::isType(const std::type_info& info) const
	{
		if (!_value)
			throw std::runtime_error("Value is null!");
		return info == this->type();
	}

	bool AnyValue::isConvertibleTo(const TypeInfo& info) const
	{
		return isConvertibleTo(info.getStdTypeInfo());
	}

	bool AnyValue::isConvertibleTo(const std::type_info& type) const
	{
		return isType(type) || ValueConverter::isConvertable(this->type(), type);
	}

	TypeInfo AnyValue::type_info() const
	{
		if (!_value)
			throw std::runtime_error("Value is null!");
		return _value->type_info();
	}

	const std::type_info& AnyValue::type() const
	{
		if (!_value)
			throw std::runtime_error("Value is null!");
		return _value->type_info().getStdTypeInfo();
	}

	AnyValue AnyValue::copy() const
	{
		if (!_value)
			throw std::runtime_error("Value is null!");
		return _value->copy();
	}

	bool AnyValue::isSerializable() const
	{
		if (!_value)
			throw std::runtime_error("Value is null!");
		return _value->isSerializable();
	}

	AnyValue AnyValue::serialize() const
	{
		if (!_value)
			throw std::runtime_error("Value is null!");
		return _value->asSerializable().toAnyValue();
	}

	bool AnyValue::isDynamicObject() const
	{
		if (!_value)
			throw std::runtime_error("Value is null!");
		return _value->isDynamicObject();
	}

	DynamicObject & AnyValue::asDynamicObject()
	{
		if (!_value)
			throw std::runtime_error("Value is null!");
		return _value->asDynamicObject();
	}
}