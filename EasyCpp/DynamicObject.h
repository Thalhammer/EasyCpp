#pragma once
#include <memory>
#include <string>
#include <vector>
#include "DllExport.h"
namespace EasyCpp
{
	class AnyValue;
	class DLL_EXPORT DynamicObject
	{
	public:
		virtual AnyValue getProperty(const std::string& name) = 0;
		virtual std::vector<std::string> getProperties() = 0;
		virtual void setProperty(const std::string& name, AnyValue value) = 0;

		virtual AnyValue callFunction(const std::string& name, const std::vector<AnyValue>& params) = 0;
		virtual std::vector<std::string> getFunctions() = 0;
	};
	typedef std::shared_ptr<DynamicObject> DynamicObjectPtr;
}
