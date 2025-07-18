#include "BallTarget.h"

BallTarget::BallTarget() : BallTarget(Vector3::Zero)
{
}

BallTarget::BallTarget(const Vector3& position)
{
	m_Position = position;
}

BallTarget_WithParent::BallTarget_WithParent(const Vector3& offset, Transform* parent)
{
	m_Parent = parent;
	m_Offset = offset;
	UpdatePosition();
}

void BallTarget_WithParent::UpdatePosition()
{
	if (not m_Parent) return;

	SetPosition(m_Offset * m_Parent->Matrix());
}
