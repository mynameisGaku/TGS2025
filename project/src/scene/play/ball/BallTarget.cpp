#include "BallTarget.h"

BallTarget::BallTarget()
{
	m_Index = -1;	// プールのインデックスは-1
	Reset();
}

void BallTarget::Reset()
{
	m_Position = Vector3::Zero;
	m_IsActive = true;	// 初期状態はアクティブ
	m_CanRockOn = true;	// 初期状態はロックオン可能
	m_DoDeactivateOnNoRockOn = false;
	m_RockOnData.clear();	// ロックオンデータは空にする
}

void BallTarget::Update()
{
	if (m_DoDeactivateOnNoRockOn)
	{
		if (m_RockOnData.empty())
		{
			m_IsActive = false; // ロックオンデータがない場合は非アクティブにする
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
