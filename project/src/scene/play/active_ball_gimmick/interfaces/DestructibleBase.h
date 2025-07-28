#pragma once
#include "GimmickBase.h"

class DestructibleBase : public GimmickBase
{
public:
	virtual bool CanBreak(const Ball* ball) const = 0;
	virtual void Break() = 0;
};