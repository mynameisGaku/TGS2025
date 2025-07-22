#include "AbilityFire.h"

#include "src/scene/play/chara/Chara.h"
#include "src/common/component/collider/ColliderSphere.h"

#include "src/util/fx/effect/EffectManager.h"

AbilityFire::AbilityFire()
{
	m_pColSphere = nullptr;
}

AbilityFire::~AbilityFire()
{
}

void AbilityFire::BaseInit(const Vector3& position, float radius, float duration_sec)
{
	transform->position = position;

	ColDefine::ColBaseParam colParam;
	colParam.trs.scale = Vector3::Ones * radius;
	colParam.tag = ColDefine::Tag::tWindArea;
	colParam.targetTags = { ColDefine::Tag::tChara };
	colParam.onlyOnce = false;

	m_pColSphere = AddComponent<ColliderSphere>();
	m_pColSphere->BaseInit(colParam);
	m_pColSphere->SetDraw(true);

	m_Duration_Sec = duration_sec;
}

void AbilityFire::CollisionEvent(const CollisionData& colData)
{
	if (colData.Other()->Tag() == ColDefine::Tag::tChara)
	{
		Chara* other = colData.Other()->Parent<Chara>();
		other->Damage(1);
	}
}

void AbilityFire::Detalis()
{
	EffectManager::Play3D_Loop("Fire.efk", *transform, "AbilityFire");
}

