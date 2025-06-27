#include "ForceFieldBase.h"
#include "ForceBase.h"
#include "src/common/component/collider/CollisionData.h"
#include "src/common/component/collider/ColliderBase.h"
#include <list>

ForceFieldBase::ForceFieldBase()
{
	m_Force = nullptr;
	m_pCollider = nullptr;
}

ForceFieldBase::~ForceFieldBase()
{
	m_Force.release();
}

void ForceFieldBase::SetColTag(const ColDefine::Tag& tag)
{
	m_pCollider->SetTag(tag);
}

void ForceFieldBase::SetColTargetTags(const std::list<ColDefine::Tag>& targetTags)
{
	m_pCollider->SetTargetTags(targetTags);
}

void ForceFieldBase::SetForce(std::unique_ptr<ForceBase> force)
{
	m_Force = std::move(force);
	m_Force->SetForceField(this);
}

void ForceFieldBase::CollisionEvent(const CollisionData& colData)
{
	Object3D* target = colData.Other()->Parent<Object3D>();
	
	m_Force->AddForce(target);
}
