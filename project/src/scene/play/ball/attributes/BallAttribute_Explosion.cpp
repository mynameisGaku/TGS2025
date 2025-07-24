#include "BallAttribute_Explosion.h"
#include "src/scene/play/ball/Ball.h"

#include "src/util/fx/effect/EffectManager.h"

#include "src/scene/play/ability/AbilityExplosion.h"

BallAttribute_Explosion::BallAttribute_Explosion(Ball* owner) : BallAttribute(owner)
{
}

BallAttribute_Explosion::~BallAttribute_Explosion()
{
}

void BallAttribute_Explosion::Update()
{
}

void BallAttribute_Explosion::Throwing()
{
}

void BallAttribute_Explosion::OnHave()
{
}

void BallAttribute_Explosion::OnHit()
{
	AbilityExplosion* explosion = Instantiate<AbilityExplosion>();
	explosion->BaseInit(owner->transform->position, 512.0f, 2.0f);
}

void BallAttribute_Explosion::OnGround()
{
}
