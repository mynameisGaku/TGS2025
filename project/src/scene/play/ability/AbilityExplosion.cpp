#include "AbilityExplosion.h"

#include "src/scene/play/chara/Chara.h"
#include "src/common/component/collider/ColliderSphere.h"

#include "src/util/fx/effect/EffectManager.h"

AbilityExplosion::AbilityExplosion()
{
	m_pColSphere = nullptr;
}

AbilityExplosion::~AbilityExplosion()
{
}

void AbilityExplosion::BaseInit(const Vector3& position, float radius, float duration_sec)
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

void AbilityExplosion::CollisionEvent(const CollisionData& colData)
{
	if (colData.Other()->Tag() == ColDefine::Tag::tChara)
	{
		Chara* other = colData.Other()->Parent<Chara>();
		other->Damage(1);
	}
}

void AbilityExplosion::Detalis()
{
	EffectManager::Play3D_Loop("Explosion.efk", *transform, "AbilityExplosion");
}
