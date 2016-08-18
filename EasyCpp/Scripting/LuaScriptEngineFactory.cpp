#include "LuaScriptEngineFactory.h"
#include "LuaScriptEngine.h"
#include "LuaState.h"
#include "../StringAlgorithm.h"
#include "../AutoInit.h"
#include "ScriptEngineManager.h"

namespace EasyCpp
{
	namespace Scripting
	{
		AUTO_INIT({
			ScriptEngineManager::registerEngineFactory(std::make_shared<LuaScriptEngineFactory>());
		});

		std::string LuaScriptEngineFactory::getEngineName()
		{
			return "Lua";
		}

		std::string LuaScriptEngineFactory::getEngineVersion()
		{
			return LuaState::getVersion();
		}

		std::vector<std::string> LuaScriptEngineFactory::getExtensions()
		{
			return{ "lua" };
		}

		std::string LuaScriptEngineFactory::getLanguageName()
		{
			return this->getEngineName();
		}

		std::string LuaScriptEngineFactory::getLanguageVersion()
		{
			return this->getEngineVersion();
		}

		std::string LuaScriptEngineFactory::getMethodCallSyntax(const std::string & obj, const std::string & m, const std::vector<std::string>& args)
		{
			return obj + ":" + m + "(" + implode<std::string>(",", args) + ")";
		}

		std::vector<std::string> LuaScriptEngineFactory::getMimeTypes()
		{
			return{ "text/x-script.lua" };
		}

		std::vector<std::string> LuaScriptEngineFactory::getNames()
		{
			return{ "lua" };
		}

		ScriptEnginePtr LuaScriptEngineFactory::getScriptEngine()
		{
			return std::make_shared<LuaScriptEngine>(this->shared_from_this());
		}

	}
}
