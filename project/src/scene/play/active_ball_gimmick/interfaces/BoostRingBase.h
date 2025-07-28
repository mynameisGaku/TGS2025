#pragma once
#include "GimmickBase.h"

class BoostRingBase : public GimmickBase
{
public:
	virtual void Boost(const Ball* pBall) = 0;
	virtual void Boost(const Chara* pChara) = 0;
};