#pragma once
#include "src/scene/play/ball/attributes/BallAttribute.h"

class BallAttribute_Explosion : public BallAttribute
{
public:
	BallAttribute_Explosion(Ball* owner);
	~BallAttribute_Explosion();

	void Update() override;

	void Throwing() override;
	void OnHave() override;
	void OnHit() override;
	void OnGround() override;

private:

};