#include "BallAttribute_Fire.h"
#include "src/scene/play/ball/Ball.h"

#include "src/util/fx/effect/EffectManager.h"

BallAttribute_Fire::BallAttribute_Fire(Ball* owner) : BallAttribute(owner)
{
}

BallAttribute_Fire::~BallAttribute_Fire()
{
}

void BallAttribute_Fire::Throwing()
{
	EffectManager::Play3D_Loop("Hit_Red.efk", *owner->transform, "Fire");
}

void BallAttribute_Fire::OnHave()
{
	EffectManager::Play3D_Loop("Hit_Red.efk", *owner->transform, "Fire");
}

void BallAttribute_Fire::OnHit()
{
	EffectManager::Play3D_Loop("Hit_Red.efk", *owner->transform, "Fire");
}

void BallAttribute_Fire::OnGround()
{
	EffectManager::Play3D_Loop("Hit_Red.efk", *owner->transform, "Fire");
}
