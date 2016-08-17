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
			_state.openStandardLibs();
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
			_state.doTransaction([this, &script, &bindings, &res] {
				_state.loadString(script);
				this->setBindings(bindings);
				_state.pcall(0, LuaState::MULTRET());
				for (int i = 0; i < _state.getTop(); i++)
				{
					if (_state.isTable(-i)) {
						res.push_back(_state.toBundle(-i));
					}
					else if (_state.isBoolean(-i))
					{
						res.push_back(_state.toBoolean(-i));
					}
					else if (_state.isDouble(-i))
					{
						res.push_back(_state.toDouble(-i));
					}
					else if (_state.isInteger(-i))
					{
						res.push_back(_state.toInteger(-i));
					}
					else if (_state.isNoneOrNil(-i))
					{
						res.push_back(AnyValue());
					}
					else if (_state.isString(-i))
					{
						res.push_back(_state.toString(-i));
					}
					else if (_state.isFunction(-i))
					{
						// AnyFunction
					}
				}
			});
			return res;
		}

		AnyValue EasyCpp::Scripting::LuaScriptEngine::get(const std::string & name)
		{
			AnyValue val;
			_state.doTransaction([this, &val, &name](){
				_state.getGlobal(name);
				if (_state.isTable(-1)) {
					val = _state.toBundle(-1);
				}
				else if (_state.isBoolean(-1))
				{
					val = _state.toBoolean(-1);
				}
				else if (_state.isDouble(-1))
				{
					val = _state.toDouble(-1);
				}
				else if (_state.isInteger(-1))
				{
					val = _state.toInteger(-1);
				}
				else if (_state.isNoneOrNil(-1))
				{
				}
				else if (_state.isString(-1))
				{
					val = _state.toString(-1);
				}
			});
			return val;
		}

		void EasyCpp::Scripting::LuaScriptEngine::put(const std::string & name, AnyValue value)
		{
			_state.doTransaction([this, &name, &value]() {
				if (value.isType<bool>()) {
					auto v = value.as<bool>();
					_state.pushBoolean(v);
				}
				else if(value.isType<AnyFunction>()) {
					auto v = value.as<AnyFunction>();
					_state.pushFunction([v](EasyCpp::Scripting::LuaState& state) mutable {
						int num_args = state.getTop();
						if (!v.hasVarArgs()) {
							auto param_types = v.getParameterTypes();
							if (num_args != param_types.size())
								throw std::runtime_error("Wrong param count, got " + std::to_string(num_args) + " expected " + std::to_string(param_types.size()));
						}

						std::vector<AnyValue> params;
						for (int i = 1; i <= num_args; i++)
						{
							if (state.isTable(i)) {
								params.push_back(state.toBundle(i));
							}
							else if (state.isBoolean(i))
							{
								params.push_back(state.toBoolean(i));
							}
							else if (state.isDouble(i))
							{
								params.push_back(state.toDouble(i));
							}
							else if (state.isInteger(i))
							{
								params.push_back(state.toInteger(i));
							}
							else if (state.isNoneOrNil(i))
							{
								params.push_back(AnyValue());
							}
							else if (state.isString(i))
							{
								params.push_back(state.toString(i));
							}
							else if (state.isFunction(i))
							{
								// AnyFunction
								params.push_back(AnyValue());
							}
						}

						AnyValue result = v.call(params);
						
						if (result.isType<bool>()) {
							auto v = result.as<bool>();
							state.pushBoolean(v);
							return 1;
						}
						else if (result.type_info().isIntegral()) {
							auto v = result.as<int64_t>();
							state.pushInteger(v);
							return 1;
						}
						else if (result.type_info().isFloatingPoint()) {
							auto v = result.as<double>();
							state.pushDouble(v);
							return 1;
						}
						else if (result.isType<nullptr_t>()) {
							state.pushNil();
							return 1;
						}
						else if (result.isConvertibleTo<Bundle>()) {
							auto v = result.as<Bundle>();
							state.pushBundle(v);
							return 1;
						}
						else {
							auto v = result.as<std::string>();
							state.pushString(v);
							return 1;
						}
						return 0;
					});
				}
				else if (value.type_info().isIntegral()) {
					auto v = value.as<int64_t>();
					_state.pushInteger(v);
				}
				else if (value.type_info().isFloatingPoint()) {
					auto v = value.as<double>();
					_state.pushDouble(v);
				}
				else if (value.isType<nullptr_t>()) {
					_state.pushNil();
				}
				else if (value.isConvertibleTo<Bundle>()) {
					auto v = value.as<Bundle>();
					_state.pushBundle(v);
				}
				else {
					auto v = value.as<std::string>();
					_state.pushString(v);
				}
				_state.setGlobal(name);
			});
		}

		Bundle EasyCpp::Scripting::LuaScriptEngine::getBindings()
		{
			Bundle res;
			_state.doTransaction([this, &res]() {
				_state.pushGlobalTable();
				res = _state.toBundle(-1);
				_state.pop(1);
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