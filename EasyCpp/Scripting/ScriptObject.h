#pragma once
#include "../DynamicObject.h"
namespace EasyCpp
{
	namespace Scripting
	{
		class ScriptEngine;
		class DLL_EXPORT ScriptObject: public DynamicObject
		{
		public:
			virtual std::shared_ptr<ScriptEngine> getEngine() = 0;
		};
		typedef std::shared_ptr<ScriptObject> ScriptObjectPtr;
	}
}
