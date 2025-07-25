#pragma once
#include "framework/gameObject.h"

class BallTarget;
class BallTarget_WithParent;
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

	// �{�[���^�[�Q�b�g�𐶐�
	BallTarget* Create();

	/// <summary>
	/// �{�[���^�[�Q�b�g���擾
	/// </summary>
	/// <param name="index">�v�[���̃C���f�b�N�X</param>
	/// <returns>�{�[���^�[�Q�b�g</returns>
	BallTarget* Get(uint32_t index);
private:
	Pool<BallTarget>* m_Pool;
};
