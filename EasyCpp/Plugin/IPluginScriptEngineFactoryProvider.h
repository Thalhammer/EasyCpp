#pragma once
#include "Interface.h"
#include "../Scripting/ScriptEngineFactory.h"
#include <vector>

namespace EasyCpp
{
	namespace Plugin
	{
		class IPluginScriptEngineFactoryProvider : public Interface<IPluginScriptEngineFactoryProvider>
		{
		public:
			static constexpr const char* INTERFACE_NAME = "IPluginScriptEngineFactoryProvider";
			static constexpr uint64_t INTERFACE_VERSION = 0;

			virtual ~IPluginScriptEngineFactoryProvider() {}
			virtual std::vector<EasyCpp::Scripting::ScriptEngineFactoryPtr> getFactories() = 0;
		};
	}
}