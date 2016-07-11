#pragma once
#include "dllexport.h"
#include <typeinfo>
#include <memory>

#include "ValueConverter.h"
#include "TypeInfo.h"
#include "Serialize/Serializable.h"

namespace EasyCpp
{
	class Bundle;
	//! \ingroup Util
	//! \class AnyValue
	//! \brief A wrapper class to pass around a variable without the need to know it's type.
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
		//! \brief Defaultconstruktor, erzeugt ein AnyValue, dass nullptr enthält.
		AnyValue();

		//! \brief Erzeugt ein AnyValue, dass die übergebene Variable enthält.
		template <typename T>
		AnyValue(T val)
		{
			_value.reset((ValueBase*)new Value<T>(val));
		}

		//! \brief Defaultkonstruktor
		virtual ~AnyValue();

		//! \brief Versucht den AnyValue in den Typ T zu konvertieren.
		//!
		//! Ist der gespeicherte Wert vom Typ T wird er direkt zurückgegeben.
		//! Ist dies nicht der Fall wird versucht den gespeicherten Typ mit Hilfe von ValueConverter in den Zieltyp zu konvertieren.
		//! Ist dies ebenfalls nicht möglich, aber der Zieltyp implementiert Serializable wird fromAnyValue(*this) aufgerufen, ansonsten wird eine exception geworfen.
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

		//! \brief Überprüft ob der gespeicherte Wert vom Typ T ist.
		template <typename T>
		bool isType() const
		{
			return isType(typeid(T));
		}
		//! \brief Überprüft ob der gespeicherte Wert vom übergebenen TypeInfo ist.
		bool isType(const TypeInfo& info) const;
		//! \brief Überprüft ob der gespeicherte Wert vom übergebenen type_info ist.
		bool isType(const std::type_info& info) const;

		//! \brief Überprüft ob es möglich ist den gespeicherten Wert in den Typ T zu konvertieren.
		template <typename T>
		bool isConvertibleTo() const
		{
			return isConvertibleTo(typeid(T));
		}
		//! \brief Überprüft ob es möglich ist den gespeicherten Wert in den TypeInfo typ zu konvertieren.
		bool isConvertibleTo(const TypeInfo& info) const;
		//! \brief Überprüft ob es möglich ist den gespeicherten Wert in den type_info typ zu konvertieren.
		bool isConvertibleTo(const std::type_info& type) const;

		//! \brief Gibt den Typ des gespeicherten Wertes zurück.
		TypeInfo type_info() const;
		//! \brief Gibt den Typ des gespeicherten Wertes als std::type_info zurück.
		const std::type_info& type() const;
		//! \brief Erstellt eine Kopie des gespeicherten Wertes mit Hilfe des CopyConstructor.
		AnyValue copy() const;
		//! \brief Überprüft ob der gespeicherte Typ Serializable implementiert.
		bool isSerializable() const;
		//! \brief Serialisiert den gespeicherten Wert.
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
