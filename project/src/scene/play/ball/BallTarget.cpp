#include "BallTarget.h"

BallTarget::BallTarget() : BallTarget(Vector3::Zero)
{
}

BallTarget::BallTarget(const Vector3& position)
{
	m_Position = position;
}

BallTarget_WithParent::BallTarget_WithParent(const Vector3& offset, Transform* parent, bool doAutoUpdate)
{
	m_Parent = parent;
	m_Offset = offset;
	m_DoAutoUpdate = doAutoUpdate;
	UpdatePosition();
}

void BallTarget_WithParent::Update()
{
	if (m_DoAutoUpdate)
	{
		UpdatePosition();
	}
	else
	{
		// 親の座標更新を行わない場合は、手動で更新する必要がある
		// 何もしない
	}
}

void BallTarget_WithParent::UpdatePosition()
{
	if (not m_Parent) return;

	SetPosition(m_Offset * m_Parent->Matrix());
}
