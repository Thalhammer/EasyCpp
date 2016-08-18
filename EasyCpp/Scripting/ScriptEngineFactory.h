#pragma once
#include <memory>
#include <string>
#include "ScriptEngine.h"
namespace EasyCpp
{
	namespace Scripting
	{
		class DLL_EXPORT ScriptEngineFactory
		{
		public:
			virtual ~ScriptEngineFactory() {}
			virtual std::string getEngineName() = 0;
			virtual std::string getEngineVersion() = 0;
			virtual std::vector<std::string> getExtensions() = 0;
			virtual std::string getLanguageName() = 0;
			virtual std::string getLanguageVersion() = 0;
			virtual std::string getMethodCallSyntax(const std::string& obj, const std::string& m, const std::vector<std::string>& args) = 0;
			virtual std::vector<std::string> getMimeTypes() = 0;
			virtual std::vector<std::string> getNames() = 0;

			virtual ScriptEnginePtr getScriptEngine() = 0;
		};
		typedef std::shared_ptr<ScriptEngineFactory> ScriptEngineFactoryPtr;
	}
}
