#pragma once
#include "src/scene/play/ball/attributes/BallAttribute.h"

class BallAttribute_Fire : public BallAttribute
{
public:
	BallAttribute_Fire(Ball* owner, float interval);
	~BallAttribute_Fire();

	void Update() override;

	void Throwing() override;
	void OnHave() override;
	void OnHit() override;
	void OnGround() override;

private:
	float m_Interval;
	float m_IntervalMax;
	bool m_Use;
};