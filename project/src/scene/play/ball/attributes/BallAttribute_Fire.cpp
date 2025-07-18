#include "BallAttribute_Fire.h"
#include "src/scene/play/ball/Ball.h"

#include "src/util/fx/effect/EffectManager.h"

#include "src/scene/play/ability/fire/AbilityFire.h"

BallAttribute_Fire::BallAttribute_Fire(Ball* owner) : BallAttribute(owner)
{
	m_Interval = 0.0f;
}

BallAttribute_Fire::~BallAttribute_Fire()
{
}

void BallAttribute_Fire::Update()
{
	m_Interval -= GTime.DeltaTime();
	if (m_Interval <= 0.0f)
		m_Interval = 5.0f;
}

void BallAttribute_Fire::Throwing()
{
	EffectManager::Play3D_Loop("Hit_Red.efk", *owner->transform, "Fire");
}

void BallAttribute_Fire::OnHave()
{
	EffectManager::Play3D_Loop("Hit_Red.efk", *owner->transform, "Fire");

	if (m_Interval == 5.0f)
	{
		AbilityFire* fire = Instantiate<AbilityFire>();
		fire->BaseInit(owner->transform->position, 256.0f, 10.0f);
	}
}

void BallAttribute_Fire::OnHit()
{

}

void BallAttribute_Fire::OnGround()
{
	AbilityFire* fire = Instantiate<AbilityFire>();
	fire->BaseInit(owner->transform->position, 256.0f, 10.0f);
}
