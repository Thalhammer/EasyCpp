#pragma once
#include "Interface.h"

namespace EasyCpp
{
	namespace Plugin
	{
		class IPluginCanUnload : public Interface<IPluginCanUnload>
		{
		public:
			static constexpr const char* INTERFACE_NAME = "IPluginCanUnload";
			static constexpr uint64_t INTERFACE_VERSION = 0;

			virtual ~IPluginCanUnload() {}
			virtual bool canUnload() = 0;
		};
	}
}