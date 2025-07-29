#pragma once
#include "framework/gameObject.h"

class BallTarget;
class BallTarget_WithParent;
template <class C>
class Pool;
class CharaManager;

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
	BallTarget* Get(uint32_t index) const;


	/// <summary>
	/// �w�肵���L�����ɑ΂��āA�ł��߂��A�����ł͂Ȃ��{�[���^�[�Q�b�g���擾����
	/// </summary>
	/// <param name="index">�L����ID</param>
	/// <param name="distance">���b�N�I����������</param>
	BallTarget* GetNearest(int index, float distance) const;
private:
	Pool<BallTarget>* m_Pool;
	CharaManager* m_pCharaManager;
};
