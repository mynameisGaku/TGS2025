#pragma once
#include "framework/gameObject.h"

class BallTarget;
template <class C>
class Pool;

/// <summary>
/// �{�[���^�[�Q�b�g���Ǘ�
/// </summary>
/// <author>�����h�l</author>
class BallTargetManager : public GameObject
{
public:
	BallTargetManager();
	~BallTargetManager();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// �{�[���^�[�Q�b�g�𐶐�
	/// </summary>
	/// <param name="position">�ʒu</param>
	/// <returns>���������{�[���^�[�Q�b�g</returns>
	BallTarget* Create(const Vector3& position);

	/// <summary>
	/// �{�[���^�[�Q�b�g���擾
	/// </summary>
	/// <param name="index">�C���f�b�N�X</param>
	/// <returns>�{�[���^�[�Q�b�g</returns>
	BallTarget* GetBallTarget(uint32_t index);
private:
	Pool<BallTarget>* m_Pool;
};
