#include "ForceFieldCorn.h"
#include "src/common/component/collider/CollisionData.h"
#include "src/common/component/collider/ColliderBase.h"
#include <src/util/math/Vector3Util.h>

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
