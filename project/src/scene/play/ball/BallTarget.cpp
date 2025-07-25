#include "BallTarget.h"

BallTarget::BallTarget()
{
	Reset();
}

void BallTarget::Reset()
{
	m_Position = Vector3::Zero;
	m_IsActive = true;	// 初期状態はアクティブ
	m_Index = -1;	// プールのインデックスは-1
}

void BallTarget::SetPositionWithParent(const Vector3& offset, Transform* parent)
{
	if (parent)
	{
		m_Position = offset * parent->Global().Matrix();
	}
}
