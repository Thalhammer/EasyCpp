#include "LuaScriptEngine.h"
#include "../Bundle.h"
#include "../AnyFunction.h"
#include "../VFS/StringReader.h"

namespace EasyCpp
{
	namespace Scripting
	{
		LuaScriptEngine::LuaScriptEngine(std::weak_ptr<ScriptEngineFactory> factory)
			: _factory(factory)
		{
			_state = std::make_shared<LuaState>();
			_state->openStandardLibs();
		}

		LuaScriptEngine::~LuaScriptEngine()
		{
		}

		AnyValue EasyCpp::Scripting::LuaScriptEngine::eval(VFS::InputStreamPtr stream)
		{
			return this->eval(stream, {});
		}

		AnyValue EasyCpp::Scripting::LuaScriptEngine::eval(VFS::InputStreamPtr stream, const Bundle & bindings)
		{
			VFS::StringReader rdr(stream);
			return this->eval(rdr.readToEnd(), bindings);
		}

		AnyValue EasyCpp::Scripting::LuaScriptEngine::eval(const std::string & script)
		{
			return this->eval(script, {});
		}

		AnyValue EasyCpp::Scripting::LuaScriptEngine::eval(const std::string & script, const Bundle & bindings)
		{
			std::vector<AnyValue> res;
			_state->doTransaction([this, &script, &bindings, &res] {
				_state->loadString(script);
				this->setBindings(bindings);
				_state->pcall(0, LuaState::MULTRET());
				for (int i = 0; i < _state->getTop(); i++)
				{
					res.push_back(_state->toAnyValue(-i));
				}
			});
			return res;
		}

		AnyValue EasyCpp::Scripting::LuaScriptEngine::get(const std::string & name)
		{
			AnyValue val;
			_state->doTransaction([this, &val, &name](){
				_state->getGlobal(name);
				val = _state->popAnyValue();
			});
			return val;
		}

		void EasyCpp::Scripting::LuaScriptEngine::put(const std::string & name, AnyValue value)
		{
			_state->doTransaction([this, &name, &value]() {
				_state->pushAnyValue(value);
				_state->setGlobal(name);
			});
		}

		Bundle EasyCpp::Scripting::LuaScriptEngine::getBindings()
		{
			Bundle res;
			_state->doTransaction([this, &res]() {
				_state->pushGlobalTable();
				res = _state->toBundle(-1);
				_state->pop(1);
			});
			return res;
		}

		void EasyCpp::Scripting::LuaScriptEngine::setBindings(const Bundle & b)
		{
			for (auto& e : b)
			{
				this->put(e.first, e.second);
			}
		}

		std::shared_ptr<ScriptEngineFactory> EasyCpp::Scripting::LuaScriptEngine::getFactory()
		{
			return _factory.lock();
		}

	}
}