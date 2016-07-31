#include "TypeInfo.h"

namespace EasyCpp
{
	bool TypeInfo::isAbstract() const { return _info->isAbstract(); }
	bool TypeInfo::isArithmetic() const { return _info->isArithmetic(); }
	bool TypeInfo::isArray() const { return _info->isArray(); }
	bool TypeInfo::isClass() const { return _info->isClass(); }
	bool TypeInfo::isCompound() const { return _info->isCompound(); }
	bool TypeInfo::isConst() const { return _info->isConst(); }
	bool TypeInfo::isEmpty() const { return _info->isEmpty(); }
	bool TypeInfo::isEnum() const { return _info->isEnum(); }
	bool TypeInfo::isFloatingPoint() const { return _info->isFloatingPoint(); }
	bool TypeInfo::isFunction() const { return _info->isFunction(); }
	bool TypeInfo::isFundamental() const { return _info->isFundamental(); }
	bool TypeInfo::isIntegral() const { return _info->isIntegral(); }
	bool TypeInfo::isLiteralType() const { return _info->isLiteralType(); }
	bool TypeInfo::isScalar() const { return _info->isScalar(); }
	bool TypeInfo::isSigned() const { return _info->isSigned(); }
	bool TypeInfo::isUnsigned() const { return _info->isUnsigned(); }
	bool TypeInfo::isTrivial() const { return _info->isTrivial(); }
	bool TypeInfo::isUnion() const { return _info->isUnion(); }
	bool TypeInfo::isObject() const { return _info->isObject(); }
	bool TypeInfo::isPOD() const { return _info->isPOD(); }
	bool TypeInfo::isNullPointer() const { return _info->isNullPointer(); }
	bool TypeInfo::isPointer() const { return _info->isPointer(); }
	bool TypeInfo::isPolymorphic() const { return _info->isPolymorphic(); }
	bool TypeInfo::isReference() const { return _info->isReference(); }
	bool TypeInfo::isVoid() const { return _info->isVoid(); }

	const std::type_info& TypeInfo::getStdTypeInfo() const { return _info->getStdTypeInfo(); }
}