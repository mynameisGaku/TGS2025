#include "BallTarget.h"

BallTarget::BallTarget()
{
	Reset();
}

void BallTarget::Reset()
{
	m_Position = Vector3::Zero;
	m_IsActive = true;	// ������Ԃ̓A�N�e�B�u
	m_Index = -1;	// �v�[���̃C���f�b�N�X��-1
}

void BallTarget::SetPositionWithParent(const Vector3& offset, Transform* parent)
{
	if (parent)
	{
		m_Position = offset * parent->Global().Matrix();
	}
}
