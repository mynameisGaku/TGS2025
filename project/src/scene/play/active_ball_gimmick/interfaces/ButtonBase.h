#pragma once
#include "GimmickBase.h"

class ButtonBase : public GimmickBase
{
public:
	virtual void OnPressed() = 0;
	virtual void OnReleased() = 0;
};