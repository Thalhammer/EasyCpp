#pragma once
#include "PluginInterface.h"

namespace EasyCpp
{
	namespace Plugin
	{
		class IPluginCanUnload : public PluginInterface<IPluginCanUnload>
		{
		public:
			static constexpr const char* INTERFACE_NAME = "IPluginCanUnload";
			static constexpr uint64_t INTERFACE_VERSION = 0;

			virtual ~IPluginCanUnload() {}
			virtual bool canUnload() = 0;
		};
	}
}