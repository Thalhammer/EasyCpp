#pragma once
#include "DynamicObject.h"
#include "AnyFunction.h"
#include "Preprocessor.h"

namespace EasyCpp
{
	class DLL_EXPORT DynamicObjectHelper : public DynamicObject
	{
	public:
		// Geerbt über DynamicObject
		virtual AnyValue getProperty(const std::string & name) override;
		virtual std::vector<std::string> getProperties() override;
		virtual void setProperty(const std::string & name, AnyValue value) override;
		virtual AnyValue callFunction(const std::string & name, const std::vector<AnyValue>& params) override;
		virtual std::vector<std::string> getFunctions() override;
	protected:
		typedef std::function<AnyValue()> getter_fn;
		typedef std::function<void(AnyValue)> setter_fn;
		void addProperty(const std::string& name, getter_fn get, setter_fn set);
		void addFunction(const std::string& name, AnyFunction fn);
		template<typename PropertyType>
		void addProperty(const std::string& name, PropertyType& property)
		{
			this->addProperty(name, [&property](){
				return PropertyType(property);
			}, [&property](AnyValue val){
				property = val.as<PropertyType>();
			});
		}
		template<typename Obj, typename Result, typename ...Args>
		void addFunction(const std::string& name, Result(Obj::*pfn)(Args...) const)
		{
			auto fn = EasyCpp::AnyFunction::fromFunction(pfn);
			this->addFunction(name, EasyCpp::AnyFunction::fromDynamicFunction([this, fn](const EasyCpp::AnyArray& params) mutable {
				EasyCpp::AnyArray args;
				args.push_back(static_cast<Obj*>(this));
				args.insert(args.end(), params.cbegin(), params.cend());
				return fn.call(args);
			}));
		}
		template<typename Obj, typename Result, typename ...Args>
		void addFunction(const std::string& name, Result(Obj::*pfn)(Args...))
		{
			auto fn = EasyCpp::AnyFunction::fromFunction(pfn);
			this->addFunction(name, EasyCpp::AnyFunction::fromDynamicFunction([this, fn](const EasyCpp::AnyArray& params) mutable {
				EasyCpp::AnyArray args;
				args.push_back(static_cast<Obj*>(this));
				args.insert(args.end(), params.cbegin(), params.cend());
				return fn.call(args);
			}));
		}
	private:
		std::map<std::string, std::pair<getter_fn, setter_fn>> _properties;
		std::map<std::string, AnyFunction> _functions;
	};
}

#define DOH_PROPERTY(x) addProperty(#x, x);
#define DOH_FUNCTION(x) addFunction(#x, &x);