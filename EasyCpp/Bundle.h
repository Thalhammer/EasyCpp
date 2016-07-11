#pragma once
#include "dllexport.h"
#include "AnyValue.h"

namespace EasyCpp
{
	class AnyValue;
	class DLL_EXPORT Bundle : public Serialize::Serializable
	{
	public:
		typedef std::map<std::string, AnyValue>::iterator iterator;
		typedef std::map<std::string, AnyValue>::value_type value_type;

		Bundle();
		Bundle(const std::map<std::string, AnyValue>& data);
		template<typename T>
		Bundle(const std::map<std::string, T>& data);
		~Bundle();

		AnyValue get(const std::string& name) const;
		template<typename T>
		bool get(const std::string& name, T& var, bool required = true) const
		{
			if (this->get(name).isConvertibleTo<T>())
			{
				var = this->get(name).as<T>();
				return true;
			}
			else {
				if (required)
					throw std::runtime_error("Required property \"" + name + "\" not found");
				return false;
			}
		}
		void set(const std::string& name, const AnyValue& val);
		bool isSet(const std::string& name) const;
		bool isEmpty() const;

		auto begin() { return _data.begin(); }
		auto end() { return _data.end(); }

		auto begin() const { return _data.cbegin(); }
		auto end() const { return _data.cend(); }
		template<typename T>
		T get(std::string name) const;

		AnyValue& operator[](const std::string& idx);
		const AnyValue& operator[](const std::string& idx) const;

		template<typename T>
		std::map<std::string, T> getMap() const;

		virtual AnyValue toAnyValue() const;
		virtual void fromAnyValue(const AnyValue& state);
	private:
		std::map<std::string, AnyValue> _data;
	};

	template<typename T>
	Bundle::Bundle(const std::map<std::string, T>& data)
	{
		for (auto& e : data)
			this->set(e.first, e.second);
	}

	template<typename T>
	T Bundle::get(std::string name) const
	{
		return get(name).as<T>();
	}

	template<typename T>
	std::map<std::string, T> Bundle::getMap() const
	{
		std::map<std::string, T> res;
		for (auto& e : *this)
			res.insert({ e.first, e.second.as<T>() });
		return res;
	}

}
