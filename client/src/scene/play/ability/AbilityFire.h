#pragma once
#include "src/scene/play/ability/AbilityBase.h"

class ColliderSphere;

class AbilityFire : public AbilityBase
{
public:
	AbilityFire();
	~AbilityFire();

	void BaseInit(const Vector3& position, float radius, float duration_sec);

	void CollisionEvent(const CollisionData& colData) override;

	void Detalis() override;

private:
	ColliderSphere* m_pColSphere;
};