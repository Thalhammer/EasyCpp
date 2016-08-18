#pragma once
#include <memory>
#include <string>
#include "ScriptEngineFactory.h"
namespace EasyCpp
{
	namespace Scripting
	{
		class DLL_EXPORT LuaScriptEngineFactory: public ScriptEngineFactory, public std::enable_shared_from_this<LuaScriptEngineFactory>
		{
		public:
			

			// Geerbt über ScriptEngineFactory
			virtual std::string getEngineName() override;

			virtual std::string getEngineVersion() override;

			virtual std::vector<std::string> getExtensions() override;

			virtual std::string getLanguageName() override;

			virtual std::string getLanguageVersion() override;

			virtual std::string getMethodCallSyntax(const std::string & obj, const std::string & m, const std::vector<std::string>& args) override;

			virtual std::vector<std::string> getMimeTypes() override;

			virtual std::vector<std::string> getNames() override;

			virtual ScriptEnginePtr getScriptEngine() override;

		};
	}
}
