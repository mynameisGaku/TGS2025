#pragma once
#include "src/util/transform/Transform.h"

/// <summary>
/// �{�[���𓊂���Ƃ����b�N�I���ł���^�[�Q�b�g
/// </summary>
/// <author>�����h�l</author>
class BallTarget
{
public:
	BallTarget();
	BallTarget(const Vector3& position);

	virtual void Update() {}
	void Draw() {}

	void SetPosition(const Vector3& position) { m_Position = position; }
	const Vector3& Position() const { return m_Position; }

	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool IsActive() const { return m_IsActive; }
private:
	Vector3 m_Position;	// �^�[�Q�b�g�̈ʒu
	bool m_IsActive;	// �^�[�Q�b�g���A�N�e�B�u���ǂ���
};

/// <summary>
/// �e�ƃI�t�Z�b�g�ō��W���X�V�ł���^�[�Q�b�g
/// </summary>
/// <author>�����h�l</author>
class BallTarget_WithParent : public BallTarget
{
public:
	BallTarget_WithParent(const Vector3& offset, Transform* parent, bool doAutoUpdate = false);

	void Update() override;
	void UpdatePosition();
private:
	Vector3 m_Offset;		// �e����̃I�t�Z�b�g���W
	Transform* m_Parent;	// �e��Transform
	bool m_DoAutoUpdate;	// �e�̍��W�X�V�������ōs�����ǂ���
};
