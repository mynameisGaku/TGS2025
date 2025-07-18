#pragma once
#include "src/scene/play/ball/BallAttribute.h"

class BallAttribute_Fire : public BallAttribute
{
public:
	BallAttribute_Fire(Ball* owner);
	~BallAttribute_Fire();

	void Throwing() override;
	void OnHave() override;
	void OnHit() override;
	void OnGround() override;
};