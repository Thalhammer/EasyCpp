#pragma once
#include "Preprocessor.h"

#define AUTO_INIT_DETAIL(n, x) \
namespace { \
	class n \
	{ \
	public: \
		n() x \
	private: \
		static n _auto_init; \
	}; \
	n n::_auto_init; \
}

#define AUTO_INIT(x) AUTO_INIT_DETAIL(MAKE_UNIQUE(AutoInit), x)