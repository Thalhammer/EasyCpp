#pragma once
#include "Preprocessor.h"

#define AUTO_INIT_DETAIL(n, x, y) \
namespace { \
	class n \
	{ \
	public: \
		n() x \
		~n() y \
	private: \
		static n _auto_init; \
	}; \
	n n::_auto_init; \
}

#define AUTO_INIT_DEINIT(x, y) AUTO_INIT_DETAIL(MAKE_UNIQUE(AutoInit), x, y)
#define AUTO_INIT(x) AUTO_INIT_DEINIT(x, {})