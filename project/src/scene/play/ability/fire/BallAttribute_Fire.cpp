#include "BallAttribute_Fire.h"
#include "src/scene/play/ball/Ball.h"

#include "src/util/fx/effect/EffectManager.h"

#include "src/scene/play/ability/explosion/AbilityExplosion.h"

BallAttribute_Fire::BallAttribute_Fire(Ball* owner, float interval) : BallAttribute(owner)
{
	m_Interval = interval;
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
}

void BallAttribute_Fire::OnHit()
{
	AbilityExplosion* fire = Instantiate<AbilityExplosion>();
	fire->BaseInit(owner->transform->position, 256.0f, 2.0f);
}

void BallAttribute_Fire::OnGround()
{
	AbilityExplosion* fire = Instantiate<AbilityExplosion>();
	fire->BaseInit(owner->transform->position, 256.0f, 2.0f);
}
