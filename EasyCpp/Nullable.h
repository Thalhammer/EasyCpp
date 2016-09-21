#pragma once
#include <memory>
namespace EasyCpp
{
	template<typename T>
	class Nullable
	{
	public:
		Nullable()
		{
		}

		Nullable(const T& obj)
		{
			_value = std::make_unique<T>(obj);
		}

		Nullable(const Nullable<T>& other)
		{
			if (!other._value)
				return;
			_value = std::make_unique<T>(*other._value);
		}

		~Nullable()
		{
		}

		Nullable<T>& operator=(const Nullable<T>& other)
		{
			if (!other._value)
				return *this;
			this->_value = std::make_unique<T>(*other._value);
			return *this;
		}

		Nullable<T>& operator=(const T& other)
		{
			this->Value(other);
			return *this;
		}

		bool IsNull() const
		{
			return _value == nullptr;
		}

		T& Value() const
		{
			return *_value;
		}

		void Value(const T& val)
		{
			_value = std::make_unique<T>(val);
		}

		T* operator->() const
		{
			return _value.get();
		}

		T& operator*() const
		{
			return *_value;
		}

		operator T() const
		{
			return *_value;
		}
	private:
		std::unique_ptr<T> _value;
	};
}
