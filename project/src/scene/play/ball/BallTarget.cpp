#include "BallTarget.h"

BallTarget::BallTarget()
{
	m_Index = -1;	// �v�[���̃C���f�b�N�X��-1
	Reset();
}

void BallTarget::Reset()
{
	m_Position = Vector3::Zero;
	m_IsActive = true;	// ������Ԃ̓A�N�e�B�u
	m_CanRockOn = true;	// ������Ԃ̓��b�N�I���\
	m_DoDeactivateOnNoRockOn = false;
	m_RockOnData.clear();	// ���b�N�I���f�[�^�͋�ɂ���
}

void BallTarget::Update()
{
	if (m_DoDeactivateOnNoRockOn)
	{
		if (m_RockOnData.empty())
		{
			m_IsActive = false; // ���b�N�I���f�[�^���Ȃ��ꍇ�͔�A�N�e�B�u�ɂ���
		}
	}
}

void BallTarget::SetPositionWithParent(const Vector3& offset, Transform* parent)
{
	if (parent)
	{
		m_Position = offset * parent->Global().Matrix();
	}
}
