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
		// �e�̍��W�X�V���s��Ȃ��ꍇ�́A�蓮�ōX�V����K�v������
		// �������Ȃ�
	}
}

void BallTarget_WithParent::UpdatePosition()
{
	if (not m_Parent) return;

	SetPosition(m_Offset * m_Parent->Matrix());
}
