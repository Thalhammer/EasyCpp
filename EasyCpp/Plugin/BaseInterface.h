#pragma once
#include "InitArgs.h"
#include "DeinitArgs.h"
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
			virtual void deinit(DeinitArgs& args) = 0;
		};
	}
}