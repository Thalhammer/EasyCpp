#include "RefCounted.h"

std::shared_ptr<void> EasyCppMySql::RefCounted::_global_refcounter;

EasyCppMySql::RefCounted::RefCounted()
	:_ref(getRef())
{
}

EasyCppMySql::RefCounted::~RefCounted()
{
}

bool EasyCppMySql::RefCounted::canDeInit()
{
	return _global_refcounter.use_count() <= 1;
}

std::shared_ptr<void> EasyCppMySql::RefCounted::getRef()
{
	if (!_global_refcounter)
		_global_refcounter = std::make_shared<int>();
	return _global_refcounter;
}
