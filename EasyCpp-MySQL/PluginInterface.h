#pragma once
#include <Plugin/Base.h>

namespace EasyCppMySql
{
	class PluginInterface : public EasyCpp::Plugin::Base
	{
	public:
		// Geerbt �ber Base
		virtual void deinit() override;
		virtual void pluginInit(EasyCpp::Plugin::InitArgs & args) override;
	};
}