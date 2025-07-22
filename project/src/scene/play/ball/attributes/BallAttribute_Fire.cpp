#include "BallAttribute_Fire.h"
#include "src/scene/play/ball/Ball.h"

#include "src/util/fx/effect/EffectManager.h"

#include "src/scene/play/ability/AbilityFire.h"

BallAttribute_Fire::BallAttribute_Fire(Ball* owner, float interval) : BallAttribute(owner)
{
	m_Interval = interval;
	m_IntervalMax = interval;
	m_Use = false;
}

BallAttribute_Fire::~BallAttribute_Fire()
{
}

void BallAttribute_Fire::Update()
{
}

void BallAttribute_Fire::Throwing()
{
}

void BallAttribute_Fire::OnHave()
{
	if (m_Interval > 0.0f)
		m_Interval -= GTime.DeltaTime();

	if (m_Interval <= 0.0f) {
		m_Interval = m_IntervalMax;
		AbilityFire* fire = Instantiate<AbilityFire>();
		fire->BaseInit(owner->transform->position, 256.0f, 2.0f);
	}
}

void BallAttribute_Fire::OnHit()
{
	AbilityFire* fire = Instantiate<AbilityFire>();
	fire->BaseInit(owner->transform->position, 256.0f, 2.0f);
}

void BallAttribute_Fire::OnGround()
{
	AbilityFire* fire = Instantiate<AbilityFire>();
	fire->BaseInit(owner->transform->position, 256.0f, 2.0f);
}
