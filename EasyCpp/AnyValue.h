#pragma once
#include "DllExport.h"
#include <typeinfo>
#include <memory>

#include "ValueConverter.h"
#include "TypeInfo.h"
#include "Serialize/Serializable.h"

namespace EasyCpp
{
	class Bundle;

	/// <summary>Class for passing dynamic typed values</summary>
	class DLL_EXPORT AnyValue
	{
	private:
		class ValueBase
		{
		public:
			virtual ~ValueBase() {}

			virtual const TypeInfo& type_info() const = 0;
			virtual void* void_value() const = 0;

			virtual AnyValue copy() const = 0;

			virtual bool isSerializable() const = 0;
			virtual const Serialize::Serializable& asSerializable() const = 0;
		};

		template <typename T>
		class Value : ValueBase
		{
		public:
			Value(T val)
				:_value(val), _info(TypeInfo::CreateInfo<T>())
			{}

			virtual ~Value() {}

			virtual const TypeInfo& type_info() const
			{
				return _info;
			}

			virtual void* void_value() const
			{
				return (void*)&_value;
			}

			virtual AnyValue copy() const
			{
				return AnyValue(_value);
			}

			virtual bool isSerializable() const
			{
				return isSerializableImpl();
			}

			template<typename U = T>
			typename std::enable_if<std::is_base_of<Serialize::Serializable, U>::value, bool>::type
				isSerializableImpl() const
			{
				return true;
			}

			template<typename U = T>
			typename std::enable_if<!std::is_base_of<Serialize::Serializable, U>::value, bool>::type
				isSerializableImpl() const
			{
				return false;
			}

			virtual const Serialize::Serializable& asSerializable() const
			{
				return asSerializableImpl();
			}

			template<typename U = T>
			typename std::enable_if<std::is_base_of<Serialize::Serializable, U>::value, const Serialize::Serializable&>::type
				asSerializableImpl() const
			{
				return dynamic_cast<const Serialize::Serializable&>(_value);
			}

			template<typename U = T>
			typename std::enable_if<!std::is_base_of<Serialize::Serializable, U>::value, const Serialize::Serializable&>::type
				asSerializableImpl() const
			{
				throw std::runtime_error("This Anyvalue does not implement Serializable");
			}

			T value() const
			{
				return _value;
			}

		private:
			T _value;
			TypeInfo _info;
		};
	public:
		/// <summary>Defaultconstruktor, creates a AnyValue containing nullptr.</summary>
		AnyValue();

		/// <summary>Creates a AnyValue which contains the given value.</summary>
		/// <param name="val">The value to store in this AnyValue</param>
		template <typename T>
		AnyValue(T val)
		{
			_value.reset((ValueBase*)new Value<T>(val));
		}

		/// <summary>Defaultdestructor</summary>
		virtual ~AnyValue();

		/// <summary>Tries to convert this AnyValue to type T.
		/// In case T is the AnyValues actual type, it's value is directly returned.
		/// Otherwise we try to convert the value using ValueConverter.
		/// If there's now possible converter but Type T implements Serializable a new instance of T is created and T::fromAnyValue(*this) is called.
		/// If all of these fails a std::runtime_error is thrown.</summary>
		/// <returns>The stored value as type T</returns>
		template <typename T>
		T as() const
		{
			if (!_value)
				throw std::runtime_error("Value is null!");
			if (this->isType<T>())
			{
				return ((Value<T>*)_value.get())->value();
			}
			else if (ValueConverter::isConvertable(this->type(), typeid(T)))
			{
				T res;
				ValueConverter::convert(this->type(), _value->void_value(), typeid(T), &res);
				return res;
			}
			else {
				return tryFromAnyValue<T>();
			}
		}

		/// <summary>Implicit value conversation.</summary>
		/// <see cref="as()"/>
		template <typename T>
		operator T ()
		{
			return this->as<T>();
		}

		/// <summary>Check whether the stored value is of type T</summary>
		/// <returns>true if the stored value if of type T</returns>
		template <typename T>
		bool isType() const
		{
			return isType(typeid(T));
		}
		/// <summary>Check whether the stored value is of the specified type.</summary>
		/// <returns>True if the stored value if of the specified type.</returns>
		bool isType(const TypeInfo& info) const;
		/// <summary>Check whether the stored value is of the specified type.</summary>
		/// <returns>True if the stored value if of the specified type.</returns>
		bool isType(const std::type_info& info) const;

		/// <summary>Check whether it is possible to convert this value to the specified type.</summary>
		/// <returns>True if the stored value can be converted.</returns>
		template <typename T>
		bool isConvertibleTo() const
		{
			return isConvertibleTo(typeid(T));
		}
		/// <summary>Check whether it is possible to convert this value to the specified type.</summary>
		/// <returns>True if the stored value can be converted.</returns>
		bool isConvertibleTo(const TypeInfo& info) const;
		/// <summary>Check whether it is possible to convert this value to the specified type.</summary>
		/// <returns>True if the stored value can be converted.</returns>
		bool isConvertibleTo(const std::type_info& type) const;

		/// <summary>Get extended information about the stored type.</summary>
		TypeInfo type_info() const;
		/// <summary>Get standard information about the stored type.</summary>
		const std::type_info& type() const;
		/// <summary>Create a copy of it's value by calling the corresponding copy constructor.</summary>
		AnyValue copy() const;
		/// <summary>Check if the contained value is serializable.</summary>
		bool isSerializable() const;
		/// <summary>Serialize the contained value.</summary>
		/// <exception cref="std::runtime_error">Thrown if the value does not implement Serializable</exception>
		AnyValue serialize() const;
	private:
		template<typename T>
		typename std::enable_if<!std::is_base_of<Serialize::Serializable, T>::value, T>::type
			tryFromAnyValue() const
		{
			throw std::runtime_error("Bad any cast");
		}

		template<typename T>
		typename std::enable_if<std::is_base_of<Serialize::Serializable, T>::value, T>::type
			tryFromAnyValue() const
		{
			T res;
			res.fromAnyValue(*this);
			return res;
		}

		std::shared_ptr<ValueBase> _value;
	};
}
