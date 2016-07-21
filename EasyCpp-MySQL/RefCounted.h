#pragma once
#include <memory>

namespace EasyCppMySql
{
	class RefCounted
	{
	public:
		RefCounted();
		virtual ~RefCounted();

		static bool canDeInit();
	private:
		std::shared_ptr<void> _ref;
		static std::shared_ptr<void> getRef();
		static std::shared_ptr<void> _global_refcounter;
	};
}