#pragma once
#include <memory>
#include <string>
#include "../DllExport.h"
#include "../AnyValue.h"
#include "../VFS/InputStream.h"
namespace EasyCpp
{
	namespace Scripting
	{
		class DLL_EXPORT ScriptEngine
		{
		public:
			virtual AnyValue eval(VFS::InputStreamPtr) = 0;
			virtual AnyValue eval(VFS::InputStreamPtr, const Bundle&) = 0;
			virtual AnyValue eval(const std::string&) = 0;
			virtual AnyValue eval(const std::string&, const Bundle&) = 0;
			virtual AnyValue get(const std::string&) = 0;
			virtual void put(const std::string&, AnyValue) = 0;
			virtual Bundle getBindings() = 0;
			virtual void setBindings(const Bundle&) = 0;

			virtual std::shared_ptr<ScriptEngineFactory> getFactory() = 0;
		};
		typedef std::shared_ptr<ScriptEngine> ScriptEnginePtr;
	}
}
