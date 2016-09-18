#include "LuaState.h"
#include "LuaException.h"
#include "../Bundle.h"
#include "../AnyValue.h"
#include "../AnyFunction.h"
#include <lua/lua.hpp>

namespace EasyCpp
{
	namespace Scripting
	{
		LuaState::LuaState()
			:_state(nullptr, [](auto p) {})
		{
			_state = std::unique_ptr<lua_State, void(*)(lua_State*)>(luaL_newstate(), [](lua_State* ptr) {
				if (ptr != nullptr)
					lua_close(ptr);
			});
			if (!_state)
				throw LuaException("Failed to initialize Lua state");

			lua_atpanic(_state.get(), [](lua_State* s) ->int {
				throw LuaException("Panic called!");
			});
		}

		LuaState::LuaState(lua_State* state)
			:_state(state, [](auto p) {})
		{
			// We do nothing here.
		}

		int LuaState::MULTRET()
		{
			return LUA_MULTRET;
		}

		int LuaState::REGISTRY_INDEX()
		{
			return LUA_REGISTRYINDEX;
		}

		std::string LuaState::getVersion()
		{
			return std::string(LUA_VERSION);
		}

		LuaState::~LuaState()
		{
		}

		void LuaState::openStandardLibs()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			luaL_openlibs(_state.get());
		}

		void LuaState::loadString(const std::string & line)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			int res = luaL_loadbuffer(_state.get(), line.data(), line.size(), "loadString");
			if (res != 0)
				throw LuaException("Failed to load string:" + this->popString());
		}

		void LuaState::loadFile(const std::string & file)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			int res = luaL_loadfile(_state.get(), file.c_str());
			if (res != 0)
				throw LuaException("Failed to load file:" + this->popString());
		}

		void LuaState::pcall(int nargs, int nresults)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			int res = lua_pcall(_state.get(), nargs, nresults, 0);
			if (res != LUA_OK)
			{
				throw LuaException("Failed to call script:" + this->popString());
			}
		}

		std::string LuaState::toString(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			size_t len = 0;
			const char* ptr = lua_tolstring(_state.get(), idx, &len);
			if (ptr == nullptr)
				throw LuaException("Value is not convertible to string");
			return std::string(ptr, ptr + len);
		}

		bool LuaState::toBoolean(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_toboolean(_state.get(), idx) == 1;
		}

		int64_t LuaState::toInteger(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			int status;
			lua_Integer res = lua_tointegerx(_state.get(), idx, &status);
			if (!status)
				throw LuaException("Failed to get Integer");
			return res;
		}

		double LuaState::toDouble(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			int status;
			lua_Number res = lua_tonumberx(_state.get(), idx, &status);
			if (!status)
				throw LuaException("Failed to get Double");
			return res;
		}

		AnyValue LuaState::toAnyValue(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			if (isTable(idx)) {
				return toBundle(idx);
			}
			else if (isBoolean(idx))
			{
				return toBoolean(idx);
			}
			else if (isDouble(idx))
			{
				return toDouble(idx);
			}
			else if (isInteger(idx))
			{
				return toInteger(idx);
			}
			else if (isNoneOrNil(idx))
			{
				return AnyValue();
			}
			else if (isString(idx))
			{
				return toString(idx);
			}
			else if (isFunction(idx))
			{
				this->pushValue(idx);
				std::shared_ptr<int> ref(new int(this->ref(REGISTRY_INDEX())), [this](int* ref) {
					this->unref(REGISTRY_INDEX(), *ref);
					delete ref;
				});
				auto fn = EasyCpp::AnyFunction::fromDynamicFunction([this, ref](const EasyCpp::AnyArray& params) {
					int top = this->getTop();
					this->rawGet(REGISTRY_INDEX(), *ref);
					for (auto& e : params) {
						this->pushAnyValue(e);
					}
					this->pcall(params.size(), MULTRET());
					int num_rets = this->getTop() - top;
					if (num_rets == 0) return AnyValue();
					if (num_rets == 1) return this->popAnyValue();
					EasyCpp::AnyArray results;
					for (int i = 0; i < num_rets; i++) {
						results.insert(results.begin(), this->popAnyValue());
					}
					return AnyValue(results);
				});
				return fn;
			}
			else
			{
				throw std::runtime_error("Unknown Lua type");
			}
		}

		void* LuaState::toUserData(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_touserdata(_state.get(), idx);
		}

		std::string LuaState::popString()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			std::string res = this->toString(-1);
			this->pop(1);
			return res;
		}

		bool LuaState::popBoolean()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			bool res = this->toBoolean(-1);
			this->pop(1);
			return res;
		}

		int64_t LuaState::popInteger()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			int64_t res = this->toInteger(-1);
			this->pop(1);
			return res;
		}

		double LuaState::popDouble()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			double res = this->toDouble(-1);
			this->pop(1);
			return res;
		}

		AnyValue LuaState::popAnyValue()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			auto res = this->toAnyValue(-1);
			this->pop(1);
			return res;
		}

		void* LuaState::popUserData()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			void* ptr = this->toUserData(-1);
			this->pop(1);
			return ptr;
		}

		void LuaState::pushString(const std::string & s)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pushlstring(_state.get(), s.data(), s.size());
		}

		void LuaState::pushBoolean(bool b)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pushboolean(_state.get(), b ? 1 : 0);
		}

		void LuaState::pushInteger(int64_t n)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pushinteger(_state.get(), n);
		}

		void LuaState::pushDouble(double d)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pushnumber(_state.get(), d);
		}

		void LuaState::pushNil()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pushnil(_state.get());
		}

		void LuaState::pushValue(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pushvalue(_state.get(), idx);
		}

		void LuaState::pushFunction(std::function<int(LuaState&)> fn)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			void* ptr = newUserData(sizeof(decltype(fn)));
			if (newMetaTable(typeid(decltype(fn)).name()))
			{
				pushString("__gc");
				pushCFunction(GCMethod<decltype(fn)>);
				setField(-3);
			}
			setMetaTable(-2);
			new(ptr) decltype(fn)(fn);

			lua_pushcclosure(_state.get(), [](lua_State* s)->int {
				auto ptr = (std::function<int(LuaState&)>*)lua_touserdata(s, lua_upvalueindex(1));
				LuaState state(s);
				try {
					return (*ptr)(state);
				}
				catch (std::exception& e)
				{
					luaL_error(s, e.what());
				}
				// any other exception as lua_error with no description
				catch (...) {
					luaL_error(s, "Unknown error");
				}
				return 0;
			}, 1);
		}

		void LuaState::pushCFunction(int(*fn)(lua_State *))
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pushcfunction(_state.get(), fn);
		}

		class LuaState::DynamicObjectWrapper
		{
		public:
			AnyValue object;

			static int s_index(lua_State* s) {
				LuaState state(s);
				DynamicObjectWrapper* wrapper = state.toUserData<DynamicObjectWrapper>(1);
				auto& object = wrapper->object.asDynamicObject();
				std::string index = state.toString(2);
				for (auto& e : object.getProperties())
				{
					if (e == index) {
						try {
							state.pushAnyValue(object.getProperty(index));
						}
						catch (const std::exception&) {
							state.pushNil();
						}
						return 1;
					}
				}
				for (auto& e : object.getFunctions())
				{
					if (e == index) {
						state.pushFunction([e](LuaState& state) {
							int num_args = state.getTop();
							DynamicObjectWrapper* wrapper = state.toUserData<DynamicObjectWrapper>(1);
							auto& object = wrapper->object.asDynamicObject();
							std::vector<AnyValue> params;
							for (int i = 2; i <= num_args; i++)
							{
								params.push_back(state.toAnyValue(i));
							}

							AnyValue result = object.callFunction(e, params);

							try {
								state.pushAnyValue(result);
							}
							catch (const std::exception&) {
								return 0;
							}
							return 1;
						});
						return 1;
					}
				}
				state.pushNil();
				return 1;
			}
			static int s_newindex(lua_State* s) {
				LuaState state(s);
				DynamicObjectWrapper* wrapper = state.toUserData<DynamicObjectWrapper>(1);
				auto& object = wrapper->object.asDynamicObject();
				std::string index = state.toString(2);
				AnyValue value = state.toAnyValue(3);
				for (auto& e : object.getProperties())
				{
					if (e == index) {
						try {
							object.setProperty(e, value);
						}
						catch (const std::exception&) {}
					}
				}
				return 0;
			}
		};

		void LuaState::pushAnyValue(AnyValue v)
		{
			if (v.type_info().isFundamental())
			{
				if (v.type_info().isArithmetic())
				{
					if (v.type_info().isFloatingPoint())
						this->pushDouble(v.as<double>());
					else this->pushInteger(v.as<int64_t>());
				}
				else if (v.isConvertibleTo<bool>())
				{
					this->pushBoolean(v.as<bool>());
				}
			}
			else if (v.isType<nullptr_t>())
			{
				pushNil();
			}
			else if (v.isType<Bundle>())
			{
				pushBundle(v.as<Bundle>());
			}
			else if (v.isDynamicObject())
			{
				this->doTransaction([this, v]() mutable {
					// Object
					DynamicObjectWrapper* wrapper = newUserData<DynamicObjectWrapper>();
					wrapper->object = v;
					// Object, orig_meta
					getMetaTable(-1); // Get Type metatable

					// Object, orig_meta, fn
					pushCFunction(&DynamicObjectWrapper::s_index);
					// Object, orig_meta
					setField(-2, "__index");

					// Object, orig_meta, fn
					pushCFunction(&DynamicObjectWrapper::s_newindex);
					// Object, orig_meta
					setField(-2, "__newindex");

					// Pop metatable
					pop(1);
				});
			}
			else if (v.isType<AnyFunction>()) {
				auto fn = v.as<AnyFunction>();
				pushFunction([fn](EasyCpp::Scripting::LuaState& state) mutable {
					int num_args = state.getTop();
					if (!fn.hasVarArgs()) {
						auto param_types = fn.getParameterTypes();
						if ((size_t)num_args != param_types.size())
							throw std::runtime_error("Wrong param count, got " + std::to_string(num_args) + " expected " + std::to_string(param_types.size()));
					}

					std::vector<AnyValue> params;
					for (int i = 1; i <= num_args; i++)
					{
						params.push_back(state.toAnyValue(i));
					}

					AnyValue result = fn.call(params);

					try {
						state.pushAnyValue(result);
					}catch(const std::exception&){
						return 0;
					}
					return 1;
				});
			}
			else if (v.isConvertibleTo<std::string>())
			{
				pushString(v.as<std::string>());
			}
			else {
				throw std::runtime_error("Failed to convert Value");
			}
		}

		void LuaState::pushGlobalTable()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pushglobaltable(_state.get());
		}

		void LuaState::pop(int num)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_pop(_state.get(), num);
		}

		void LuaState::remove(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_remove(_state.get(), idx);
		}

		void LuaState::replace(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_replace(_state.get(), idx);
		}

		void LuaState::rotate(int idx, int n)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_rotate(_state.get(), idx, n);
		}

		bool LuaState::checkUserData(int idx, const std::string & meta)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return luaL_checkudata(_state.get(), idx, meta.c_str()) != nullptr;
		}

		void * LuaState::newUserData(size_t size)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_newuserdata(_state.get(), size);
		}

		bool LuaState::newMetaTable(const std::string & name)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return luaL_newmetatable(_state.get(), name.c_str()) == 1;
		}

		void LuaState::newTable()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_newtable(_state.get());
		}

		void LuaState::getField(int idx, const std::string & field)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_getfield(_state.get(), idx, field.c_str());
		}

		void LuaState::setField(int idx, const std::string & field)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_setfield(_state.get(), idx, field.c_str());
		}

		void LuaState::getField(int idx, int64_t i)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_geti(_state.get(), idx, i);
		}

		void LuaState::setField(int idx, int64_t i)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_seti(_state.get(), idx, i);
		}

		void LuaState::getField(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_gettable(_state.get(), idx);
		}

		void LuaState::setField(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_settable(_state.get(), idx);
		}

		void LuaState::getGlobal(const std::string & name)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_getglobal(_state.get(), name.c_str());
		}

		void LuaState::setGlobal(const std::string & name)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_setglobal(_state.get(), name.c_str());
		}

		void LuaState::getMetaTable(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			int i = lua_getmetatable(_state.get(), idx);
			if (i == 0)
				throw LuaException("No Metatable assigned");
		}

		void LuaState::setMetaTable(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_setmetatable(_state.get(), idx);
		}

		int LuaState::getTop()
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_gettop(_state.get());
		}

		void LuaState::setTop(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_settop(_state.get(), idx);
		}

		bool LuaState::next(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_next(_state.get(), idx) != 0;
		}

		int LuaState::upValueIndex(int i)
		{
			// std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_upvalueindex(i);
		}

		int LuaState::ref(int t)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return luaL_ref(_state.get(), t);
		}

		void LuaState::unref(int t, int ref)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			luaL_unref(_state.get(), t, ref);
		}

		bool LuaState::rawEqual(int idx1, int idx2)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_rawequal(_state.get(), idx1, idx2) == 1;
		}

		void LuaState::rawGet(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_rawget(_state.get(), idx);
		}

		void LuaState::rawGet(int idx, int64_t n)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_rawgeti(_state.get(), idx, n);
		}

		size_t LuaState::rawLen(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_rawlen(_state.get(), idx);
		}

		void LuaState::rawSet(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_rawset(_state.get(), idx);
		}

		void LuaState::rawSet(int idx, int64_t i)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			lua_rawseti(_state.get(), idx, i);
		}

		void LuaState::doTransaction(std::function<void()> fn)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			fn();
		}

		void LuaState::iterateTable(int idx, std::function<void()> fn)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			this->pushValue(idx);		// Bring table to top
			this->pushNil();			// Initial key value
			while (this->next(-2))		// Iterate
			{
				this->pushValue(-2);	// Copy key to top
				fn();
				this->pop(2);			// Value + copy of key
			}
			this->pop(1);				// Pop copy of table
		}

		void LuaState::pushBundle(const Bundle & b)
		{
			this->doTransaction([&b, this]() {
				this->newTable();
				for (auto e : b)
				{
					this->pushAnyValue(e.second);
					this->setField(-2, e.first);
				}
			});
		}

		Bundle LuaState::toBundle(int idx)
		{
			Bundle res;
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			this->iterateTable(idx, [this, &res]() {
				std::string name = this->toString(-1);
				res.set(name, this->toAnyValue(-2));
			});
			return res;
		}

		bool LuaState::isBoolean(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isboolean(_state.get(), idx);
		}

		bool LuaState::isCFunction(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_iscfunction(_state.get(), idx) == 1;
		}

		bool LuaState::isFunction(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isfunction(_state.get(), idx);
		}

		bool LuaState::isInteger(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isinteger(_state.get(), idx) == 1;
		}

		bool LuaState::isNil(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isnil(_state.get(), idx);
		}

		bool LuaState::isNone(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isnone(_state.get(), idx);
		}

		bool LuaState::isNoneOrNil(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isnoneornil(_state.get(), idx);
		}

		bool LuaState::isDouble(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isnumber(_state.get(), idx) == 1;
		}

		bool LuaState::isString(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isstring(_state.get(), idx) == 1;
		}

		bool LuaState::isTable(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_istable(_state.get(), idx);
		}

		bool LuaState::isThread(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isthread(_state.get(), idx);
		}

		bool LuaState::isUserdata(int idx)
		{
			std::unique_lock<std::recursive_mutex> lck(_state_mtx);
			return lua_isuserdata(_state.get(), idx) == 1;
		}
	}
}
