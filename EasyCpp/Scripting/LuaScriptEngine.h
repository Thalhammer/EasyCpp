#pragma once
#include "ScriptEngine.h"
#include "LuaState.h"
namespace EasyCpp
{
	namespace Scripting
	{
		class DLL_EXPORT LuaScriptEngine: public ScriptEngine, public std::enable_shared_from_this<LuaScriptEngine>
		{
		public:
			LuaScriptEngine(std::weak_ptr<ScriptEngineFactory> factory);
			virtual ~LuaScriptEngine();
			// Geerbt über ScriptEngine
			virtual AnyValue eval(VFS::InputStreamPtr) override;
			virtual AnyValue eval(VFS::InputStreamPtr, const Bundle &) override;
			virtual AnyValue eval(const std::string &) override;
			virtual AnyValue eval(const std::string &, const Bundle &) override;
			virtual AnyValue get(const std::string &) override;
			virtual void put(const std::string &, AnyValue) override;
			virtual Bundle getBindings() override;
			virtual void setBindings(const Bundle &) override;
			virtual std::shared_ptr<ScriptEngineFactory> getFactory() override;
		private:
			std::weak_ptr<ScriptEngineFactory> _factory;
			LuaState _state;
		};
	}
}
