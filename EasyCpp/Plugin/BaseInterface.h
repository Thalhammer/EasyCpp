#pragma once
#include "InitArgs.h"
#include "../DllExport.h"

namespace EasyCpp
{
	namespace Plugin
	{
		class DLL_EXPORT BaseInterface
		{
		public:
			virtual ~BaseInterface() {}
			virtual void init(InitArgs& args) = 0;
			virtual void deinit() = 0;
		};
#define EASYCPP_PLUGIN_ENTRY(x) \
extern "C" { \
	DLL_EXPORT EasyCpp::Plugin::BaseInterface* createBaseInterface() \
	{ \
		return new x(); \
	} \
	\
	DLL_EXPORT void deleteBaseInterface(EasyCpp::Plugin::BaseInterface* iface) \
	{ \
		delete ((x*)iface); \
	}\
}
	}
}