#include "ForceFieldSphere.h"
#include "src/common/component/collider/ColliderSphere.h"

ForceFieldSphere::ForceFieldSphere(float radius)
{
	m_pCollider = AddComponent<ColliderSphere>();

	ColDefine::ColBaseParam param;
	param.trs.scale = Vector3(radius, radius, radius);

	m_pCollider->BaseInit(param);
	m_pCollider->SetDraw(false);
}
