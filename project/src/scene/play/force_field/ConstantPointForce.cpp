#include "ConstantPointForce.h"
#include "ForceFieldBase.h"
#include "src/util/math/Vector3.h"
#include "src/util/time/GameTime.h"

void ConstantPointForce::AddForce(Object3D* target)
{
	const Vector3 dif = target->transform->position - m_pForceField->transform->position;
	const Vector3 dir = dif.Normalize();

	target->transform->position += dir * m_Speed * GTime.DeltaTime();
}
