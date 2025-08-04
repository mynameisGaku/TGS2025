#pragma once
#include "GimmickBase.h"

class TrampolineBase : public GimmickBase
{
public:
	virtual void ReflectBall(Ball* pBall) = 0;
	virtual void ReflectChara(Chara* pChara) = 0;
};