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

	void SetPosition(const Vector3& position) { m_Position = position; }
	const Vector3& Position() const { return m_Position; }
private:
	Vector3 m_Position;
};

/// <summary>
/// �e�ƃI�t�Z�b�g�ō��W���X�V�ł���^�[�Q�b�g
/// </summary>
/// <author>�����h�l</author>
class BallTarget_WithParent : public BallTarget
{
public:
	BallTarget_WithParent(const Vector3& offset, Transform* parent);

	void UpdatePosition();
private:
	Vector3 m_Offset;
	Transform* m_Parent;
};
