#pragma once
#include "../DllExport.h"
#include "../ThreadSafe.h"
#include "ScriptEngineFactory.h"
#include <vector>
namespace EasyCpp
{
	namespace Scripting
	{
		class DLL_EXPORT ScriptEngineManager
		{
		public:
			/// <summary>Get a list of registered hashing engines.</summary>
			static std::vector<ScriptEngineFactoryPtr> getAvailableEngines();
			/// <summary>Get a script engine by source file extension.</summary>
			static ScriptEnginePtr getEngineByExtension(const std::string& extension);
			/// <summary>Get a script engine by source file mime type.</summary>
			static ScriptEnginePtr getEngineByMimeType(const std::string& mime);
			/// <summary>Get a script engine by short name.</summary>
			static ScriptEnginePtr getEngineByName(const std::string& sname);
			/// <summary>Register a script engine.</summary>
			static void registerEngineFactory(ScriptEngineFactoryPtr factory);
			/// <summary>Deregister script engine.</summary>
			static void deregisterEngineFactory(const std::string& short_name);
		private:
			ScriptEngineManager();
			static ScriptEngineManager& getInstance();
			
			std::vector<ScriptEngineFactoryPtr> _getAvailableEngines();
			ScriptEnginePtr _getEngineByExtension(const std::string& extension);
			ScriptEnginePtr _getEngineByMimeType(const std::string& mime);
			ScriptEnginePtr _getEngineByName(const std::string& sname);
			void _registerEngineFactory(ScriptEngineFactoryPtr factory);
			void _deregisterEngineFactory(const std::string& short_name);
			
			ThreadSafe<std::vector<ScriptEngineFactoryPtr>> _factories;
		};
	}
}
