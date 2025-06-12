#include "singleton.h"

namespace
{
	std::unique_ptr<std::list<SingletonDeleter::DestroyFunc>> funcs;
}

void SingletonDeleter::Delete()
{
	if (not funcs) return;

	for (auto itr = funcs->begin(); itr != funcs->end(); itr++)
	{
		SingletonDeleter::DestroyFunc func = *itr;

		(*func)();
	}

	funcs.release();
}

void SingletonDeleter::addDeleteTarget(DestroyFunc func)
{
	if (not funcs)
	{
		funcs = std::make_unique<std::list<SingletonDeleter::DestroyFunc>>();
	}

	// �����ƍ폜���t���ɂ��� �����o
	funcs->push_front(func);
}
