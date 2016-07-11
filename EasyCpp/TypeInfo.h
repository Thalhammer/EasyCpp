#pragma once
#include <type_traits>
#include <memory>

namespace EasyCpp
{
	class TypeInfo
	{
	public:
		virtual ~TypeInfo()
		{
		}
		template<typename T>
		static TypeInfo CreateInfo()
		{
			return TypeInfo(std::shared_ptr<TypeBase>((TypeBase*)new Type<T>()));
		}

		//! An abstract class is a class that has at least one pure virtual function.
		//! @return true if this is a abstract class.
		bool isAbstract() const { return _info->isAbstract(); }
		//! An arithmetic type is a type is a fundamental type that is either a integer or a float.
		//! @return true if this is an arithmetic type.
		bool isArithmetic() const { return _info->isArithmetic(); }
		//! @return true if this is an array.
		bool isArray() const { return _info->isArray(); }
		//! @return true if this is a class.
		bool isClass() const { return _info->isClass(); }
		bool isCompound() const { return _info->isCompound(); }
		bool isConst() const { return _info->isConst(); }
		bool isEmpty() const { return _info->isEmpty(); }
		bool isEnum() const { return _info->isEnum(); }
		bool isFloatingPoint() const { return _info->isFloatingPoint(); }
		bool isFunction() const { return _info->isFunction(); }
		bool isFundamental() const { return _info->isFundamental(); }
		bool isIntegral() const { return _info->isIntegral(); }
		bool isLiteralType() const { return _info->isLiteralType(); }
		bool isScalar() const { return _info->isScalar(); }
		bool isSigned() const { return _info->isSigned(); }
		bool isUnsigned() const { return _info->isUnsigned(); }
		bool isTrivial() const { return _info->isTrivial(); }
		bool isUnion() const { return _info->isUnion(); }
		bool isObject() const { return _info->isObject(); }
		bool isPOD() const { return _info->isPOD(); }
		bool isNullPointer() const { return _info->isNullPointer(); }
		bool isPointer() const { return _info->isPointer(); }
		bool isPolymorphic() const { return _info->isPolymorphic(); }
		bool isReference() const { return _info->isReference(); }
		bool isVoid() const { return _info->isVoid(); }

		const std::type_info& getStdTypeInfo() const { return _info->getStdTypeInfo(); }

		bool operator==(const TypeInfo& other) const { return this->getStdTypeInfo() == other.getStdTypeInfo(); }
		bool operator!=(const TypeInfo& other) const { return this->getStdTypeInfo() != other.getStdTypeInfo(); }
	private:
		class TypeBase
		{
		public:
			virtual ~TypeBase() {}

			virtual bool isAbstract() const = 0;
			virtual bool isArithmetic() const = 0;
			virtual bool isArray() const = 0;
			virtual bool isClass() const = 0;
			virtual bool isCompound() const = 0;
			virtual bool isConst() const = 0;
			virtual bool isEmpty() const = 0;
			virtual bool isEnum() const = 0;
			virtual bool isFloatingPoint() const = 0;
			virtual bool isFunction() const = 0;
			virtual bool isFundamental() const = 0;
			virtual bool isIntegral() const = 0;
			virtual bool isLiteralType() const = 0;
			virtual bool isScalar() const = 0;
			virtual bool isSigned() const = 0;
			virtual bool isUnsigned() const = 0;
			virtual bool isTrivial() const = 0;
			virtual bool isUnion() const = 0;
			virtual bool isObject() const = 0;
			virtual bool isPOD() const = 0;
			virtual bool isNullPointer() const = 0;
			virtual bool isPointer() const = 0;
			virtual bool isPolymorphic() const = 0;
			virtual bool isReference() const = 0;
			virtual bool isVoid() const = 0;

			virtual const std::type_info& getStdTypeInfo() const = 0;
		};

		template<typename T>
		class Type : public TypeBase
		{
		public:
			virtual ~Type() {}


			virtual bool isAbstract() const { return std::is_abstract<T>(); }
			virtual bool isArithmetic() const { return std::is_arithmetic<T>(); }
			virtual bool isArray() const { return std::is_array<T>(); }
			virtual bool isClass() const { return std::is_class<T>(); }
			virtual bool isCompound() const { return std::is_compound<T>(); }
			virtual bool isConst() const { return std::is_const<T>(); }
			virtual bool isEmpty() const { return std::is_empty<T>(); }
			virtual bool isEnum() const { return std::is_enum<T>(); }
			virtual bool isFloatingPoint() const { return std::is_floating_point<T>(); }
			virtual bool isFunction() const { return std::is_function<T>(); }
			virtual bool isFundamental() const { return std::is_fundamental<T>(); }
			virtual bool isIntegral() const { return std::is_integral<T>(); }
			virtual bool isLiteralType() const { return std::is_literal_type<T>(); }
			virtual bool isScalar() const { return std::is_scalar<T>(); }
			virtual bool isSigned() const { return std::is_signed<T>(); }
			virtual bool isUnsigned() const { return std::is_unsigned<T>(); }
			virtual bool isTrivial() const { return std::is_trivial<T>(); }
			virtual bool isUnion() const { return std::is_union<T>(); }
			virtual bool isObject() const { return std::is_object<T>(); }
			virtual bool isPOD() const { return std::is_pod<T>(); }
			virtual bool isNullPointer() const { return std::is_null_pointer<T>(); }
			virtual bool isPointer() const { return std::is_pointer<T>(); }
			virtual bool isPolymorphic() const { return std::is_polymorphic<T>(); }
			virtual bool isReference() const { return std::is_reference<T>(); }
			virtual bool isVoid() const { return std::is_void<T>(); }

			virtual const std::type_info& getStdTypeInfo() const { return typeid(T); }
		};

		TypeInfo(std::shared_ptr<TypeBase> ptr)
			:_info(ptr)
		{}

		std::shared_ptr<TypeBase> _info;
	};
}