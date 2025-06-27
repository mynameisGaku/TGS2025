#include "ForceFieldCorn.h"
#include "src/common/component/collider/CollisionData.h"
#include "src/common/component/collider/ColliderBase.h"
#include <src/util/math/Vector3Util.h>

void ForceFieldCorn::Draw()
{
	DrawCone3D(transform->position, transform->position + transform->Forward() * m_pCollider->transform->scale.x, 30.0f, 8, 0x00FFFF, 0x000000, true);
}

void ForceFieldCorn::CollisionEvent(const CollisionData& colData)
{
	Object3D* target = colData.Other()->Parent<Object3D>();
	const Vector3 forward = transform->Forward();
	const Vector3 toOther = target->transform->position - transform->position;

	if (Vector3Util::AngleBetween(forward, toOther) <= m_CornAngle)
	{
		ForceFieldBase::CollisionEvent(colData);
	}
}
