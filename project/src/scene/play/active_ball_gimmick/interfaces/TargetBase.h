#pragma once
#include "GimmickBase.h"

class TargetBase : public GimmickBase
{
public:
	virtual void SpawnReward() = 0;
};