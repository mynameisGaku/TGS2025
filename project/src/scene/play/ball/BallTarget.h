#pragma once
#include "src/util/transform/Transform.h"
#include "src/scene/play/ball/Ball.h"

/// <summary>
/// �{�[���𓊂���Ƃ����b�N�I���ł���^�[�Q�b�g
/// </summary>
/// <author>�����h�l</author>
class BallTarget
{
public:
	BallTarget();

	void Reset();

	void Update() {}
	void Draw() {}

	void SetPosition(const Vector3& position) { m_Position = position; }
	void SetPositionWithParent(const Vector3& offset, Transform* parent);
	const Vector3& Position() const { return m_Position; }

	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool IsActive() const { return m_IsActive; }

	void SetIndex(int index) { m_Index = index; }
	int Index() const { return m_Index; }

	void SetCanRockOn(bool canRockOn) { m_CanRockOn = canRockOn; }
	bool CanRockOn() const { return m_CanRockOn; }
private:
	Vector3 m_Position;	// �^�[�Q�b�g�̈ʒu
	bool m_IsActive;	// �^�[�Q�b�g���A�N�e�B�u���ǂ���
	bool m_CanRockOn;	// ���b�N�I���\���ǂ���
	int m_Index;		// �v�[���̃C���f�b�N�X
};
