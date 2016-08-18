#pragma once
#include <memory>
#include <mutex>
#include <string>
#include "../DllExport.h"
#include "LuaException.h"

struct lua_State;
namespace EasyCpp
{
	class Bundle;
	class AnyValue;
	namespace Scripting
	{
		class LuaState;
		class DLL_EXPORT LuaState
		{
		public:
			LuaState();
			~LuaState();

			// Opens all standard libraries of lua.
			void openStandardLibs();

			// Loads a source file into lua and compiles it.
			void loadFile(const std::string& file);
			// Loads the source string into lua and compiles it.
			void loadString(const std::string& line);

			// Calls the function on top of the stack using nargs number of arguments and returning nresults.
			// If you do not know the number of results supply MULTRET.
			void pcall(int nargs, int nresults);

			/* Stack handling */
			// Converts the Lua value at the given index to a string.
			// If the value is a number, then toString also changes the actual value in the stack to a string.
			std::string toString(int idx);
			// Converts the Lua value at the given index to a boolean value.
			// Like all tests in Lua, toBoolean returns true for any Lua value different from false and nil; otherwise it returns false.
			bool toBoolean(int idx);
			// Converts the Lua value at the given index to the signed integral type int64_t.
			// The Lua value must be an integer, or a number or string convertible to an integer, otherwise returns 0.
			int64_t toInteger(int idx);
			// Converts the Lua value at the given index to a double.
			// The Lua value must be an number, or a string convertible to a number, otherwise returns 0.
			double toDouble(int idx);
			// Converts the Lua value at the given index to a AnyValue.
			// The AnyValue will store one of std::string, bool, int64_t, double, AnyFunction, nullptr_t, Bundle
			AnyValue toAnyValue(int idx);
			// If the value at the given index is a full userdata, returns its block address.
			// Otherwise, returns NULL.
			void* toUserData(int idx);
			// If the value at the given index is a full userdata and it's metatablename is equivalent to "typeid(T).name()"
			// returns the userdata casted to this type. Otherwise throws a exception.
			template<typename T>
			T* toUserData(int idx)
			{
				std::unique_lock<std::recursive_mutex> lck(_state_mtx);
				if (!this->checkUserData(idx, typeid(T).name()))
					throw LuaException("Failed to get Userdata");
				return (T*)this->toUserData(idx);
			}

			// Returns true if the value at the given index is a boolean, and false otherwise.
			bool isBoolean(int idx);
			// Returns true if the value at the given index is a C function, and false otherwise.
			bool isCFunction(int idx);
			// Returns true if the value at the given index is a function, and false otherwise.
			bool isFunction(int idx);
			// Returns true if the value at the given index is a integer, and false otherwise.
			bool isInteger(int idx);
			// Returns true if the value at the given index is nil, and false otherwise.
			bool isNil(int idx);
			// Returns true if the value at the given index is none, and false otherwise.
			bool isNone(int idx);
			// Returns true if the value at the given index is none or nil, and false otherwise.
			bool isNoneOrNil(int idx);
			// Returns true if the value at the given index is a double, and false otherwise.
			bool isDouble(int idx);
			// Returns true if the value at the given index is a string, and false otherwise.
			bool isString(int idx);
			// Returns true if the value at the given index is a table, and false otherwise.
			bool isTable(int idx);
			// Returns true if the value at the given index is a thread, and false otherwise.
			bool isThread(int idx);
			// Returns true if the value at the given index is a userdata, and false otherwise.
			bool isUserdata(int idx);

			// Converts the Lua value at the top of the stack to a string and pops it.
			std::string popString();
			// Converts the Lua value at the top of the stack to a boolean value and pops it.
			bool popBoolean();
			// Converts the Lua value at the top of the stack to a integer value and pops it.
			int64_t popInteger();
			// Converts the Lua value at the top of the stack to a double value and pops it.
			double popDouble();
			// Converts the Lua value at the top of the stack to a AnyValue and pops it.
			AnyValue popAnyValue();
			// Converts the Lua value at the top of the stack to a userdata value and pops it.
			// If the value is not a userdata returns NULL.
			void* popUserData();
			// If the value at the top of the stack is a full userdata and it's metatablename is equivalent to "typeid(T).name()"
			// returns the userdata casted to this type. Otherwise throws a exception and does not pop the value.
			template<typename T>
			T* popUserData()
			{
				std::unique_lock<std::mutex> lck(_state_mtx);
				if (!this->checkUserData(-1, typeid(T).name()))
					throw LuaException("Failed to get Userdata");
				return (T*)this->popUserData();
			}

			// Pushes the string s onto the stack.
			void pushString(const std::string& s);
			// Pushes a boolean value with value b onto the stack.
			void pushBoolean(bool b);
			// Pushes an integer with value n onto the stack.
			void pushInteger(int64_t n);
			// Pushes a double with value d onto the stack.
			void pushDouble(double d);
			// Pushes a nil value onto the stack.
			void pushNil();
			// Pushes a copy of the element at the given index onto the stack.
			void pushValue(int idx);
			// Pushes a function onto the stack.
			// This function is represented by a CClosure.
			void pushFunction(std::function<int(LuaState&)> fn);
			// Pushes a C function onto the stack.
			void pushCFunction(int(*fn)(lua_State*));
			// Push a anyvalue onto the stack trying to convert values as possible.
			void pushAnyValue(AnyValue v);

			// Pushes Global table
			void pushGlobalTable();

			// Pops num elements from the stack.
			void pop(int num);
			// Removes the element at the given valid index, shifting down the elements above this index to fill the gap.
			// This function cannot be called with a pseudo-index, because a pseudo-index is not an actual stack position.
			void remove(int idx);
			// Moves the top element into the given valid index without shifting any element
			// (therefore replacing the value at that given index), and then pops the top element.
			void replace(int idx);
			// Rotates the stack elements between the valid index idx and the top of the stack.
			// The elements are rotated n positions in the direction of the top, for a positive n,
			// or -n positions in the direction of the bottom, for a negative n.
			// The absolute value of n must not be greater than the size of the slice being rotated.
			// This function cannot be called with a pseudo-index, because a pseudo-index is not an actual stack position.
			void rotate(int idx, int n);

			// Check if the userdata at the given index has a meta table with name meta.
			bool checkUserData(int idx, const std::string& meta);

			// This function allocates a new block of memory with the given size, 
			// pushes onto the stack a new full userdata with the block address,
			// and returns this address. The host program can freely use this memory.
			void* newUserData(size_t size);

			// Allocates a new userdata of class T.
			// This ties T's destructor to "__gc" method in lua thus allows it to be called automatically.
			// Note that lifetime of this object is controlled by lua, you should not keep references to it.
			// A good idea is to create some sort of WrapperClass that wraps a shared_ptr to your object and use this wrapperclass with lua.
			template<typename T, typename ...Args>
			T* newUserData(Args&&... params);

			// Creates a types Metatable and leaves it on stack
			template<typename T>
			void createTypeMetaTable()
			{
				if (newMetaTable(typeid(T).name()))
				{
					pushString("__gc");
					pushCFunction(GCMethod<T>);
					setField(-3);
					// Set metatable
					pushString("__index");
					pushValue(-2);
					setField(-3);
				}
			}

			// Creates a new table to be used as a metatable for userdata,
			// adds to this new table the pair __name = name and adds to the registry the pair [tname] = new table.
			// If a metatable with this name already exists it is not changed.
			// In both cases pushes onto the stack the final value associated with name in the registry.
			// Returns true if the metatable was created, false if it already existed
			bool newMetaTable(const std::string& name);

			// Creates a new empty table and pushes it onto the stack.
			void newTable();

			// Pushes onto the stack the value t[field], where t is the value at the given index.
			void getField(int idx, const std::string& field);
			// Does the equivalent to t[field] = v, where t is the value at the given index and v is the value at the top of the stack.
			// This function pops the value from the stack. As in Lua, this function may trigger a metamethod for the "newindex" event.
			void setField(int idx, const std::string& field);
			// Pushes onto the stack the value t[i], where t is the value at the given index.
			void getField(int idx, int64_t i);
			// Does the equivalent to t[i] = v, where t is the value at the given index and v is the value at the top of the stack.
			// This function pops the value from the stack. As in Lua, this function may trigger a metamethod for the "newindex" event.
			void setField(int idx, int64_t i);
			// Pushes onto the stack the value t[k], where t is the value at the given index and k is the value at the top of the stack.
			// This function pops the key from the stack, pushing the resulting value in its place.
			void getField(int idx);
			// Does the equivalent to t[k] = v, where t is the value at the given index, v is the value at the top of the stack, and k is the value just below the top.
			// This function pops both the key and the value from the stack. As in Lua, this function may trigger a metamethod for the "newindex" event.
			void setField(int idx);

			// Pushes onto the stack the value of the global name.
			void getGlobal(const std::string& name);
			// Pops a value from the stack and sets it as the new value of global name.
			void setGlobal(const std::string& name);

			// If the value at the given index has a metatable, the function pushes that metatable onto the stack. Otherwise, the function throws a exception and pushes nothing on the stack.
			void getMetaTable(int idx);
			// Pops a table from the stack and sets it as the new metatable for the value at the given index.
			void setMetaTable(int idx);

			// Returns the index of the top element in the stack. Because indices start at 1, this result is equal to the number of elements in the stack; in particular, 0 means an empty stack.
			int getTop();
			// Accepts any index, or 0, and sets the stack top to this index. If the new top is larger than the old one, then the new elements are filled with nil. If index is 0, then all stack elements are removed.
			void setTop(int idx);

			// Pops a key from the stack, and pushes a key–value pair from the table
			// at the given index (the "next" pair after the given key).
			// If there are no more elements in the table, returns 0 (and pushes nothing).
			bool next(int idx);

			// Returns the pseudo-index that represents the i-th upvalue of the running function.
			int upValueIndex(int i);

			// Creates and returns a reference, in the table at index t, for the object at the top of the stack (and pops the object).
			int ref(int t);
			// Releases reference ref from the table at index t.
			void unref(int t, int ref);

			// Returns true if the two values in indices index1 and index2 are primitively equal (that is, without calling metamethods).
			// Otherwise returns false. Also returns false if any of the indices are not valid.
			bool rawEqual(int idx1, int idx2);
			// Similar to getField(int), but does a raw access (i.e., without metamethods).
			void rawGet(int idx);
			// Pushes onto the stack the value t[n], where t is the table at the given index.
			// The access is raw; that is, it does not invoke metamethods.
			void rawGet(int idx, int64_t n);
			// Returns the raw "length" of the value at the given index: for strings,
			// this is the string length; for tables, this is the result of the
			// length operator ('#') with no metamethods; for userdata, this is the
			// size of the block of memory allocated for the userdata; for other values, it is 0.
			size_t rawLen(int idx);
			// Similar to setField(int), but does a raw assignment (i.e., without metamethods).
			void rawSet(int idx);
			// Does the equivalent of t[i] = v, where t is the table at the given index and v is the value at the top of the stack.
			// This function pops the value from the stack.The assignment is raw; that is, it does not invoke metamethods.
			void rawSet(int idx, int64_t i);

			// Executes a Transaction.
			// This means that other threads can not change the state while this is running.
			void doTransaction(std::function<void()> fn);

			// Iterates over the table and calls fn for each element in this table
			// fn Funktion to execute for each element
			// idx Index of the table to iterate over
			void iterateTable(int idx, std::function<void()> fn);

			// Convert a Bundle to a Lua table and push it onto the stack
			void pushBundle(const Bundle& b);
			// Convert a Lua table to a Bundle
			Bundle toBundle(int idx);

		private:
			std::recursive_mutex _state_mtx;
			std::unique_ptr<lua_State, void(*)(lua_State*)> _state;

			// Template method used for class destructor.
			template<typename T>
			static int GCMethod(lua_State* L);

			// Creates a wrapper around the supplied state.
			// This disables autoclose and does not change the panic method.
			LuaState(lua_State* state);
			class DynamicObjectWrapper;
		public: // Constants
			static int MULTRET();
			static int REGISTRY_INDEX();

			static std::string getVersion();
		};

		template<typename T, typename... Args>
		T* LuaState::newUserData(Args&&... params)
		{
			void* ptr = newUserData(sizeof(T));
			createTypeMetaTable<T>();
			setMetaTable(-2);
			new(ptr) T(params...);
			return (T*)ptr;
		}

		template<typename T>
		int LuaState::GCMethod(lua_State* L)
		{
			LuaState s(L);
			s.toUserData<T>(1)->~T();
			return 0;
		}
	}
}
