#pragma once

// ���p����
#include "framework/gameObject.h"

class ColliderBase;

/// <summary>
/// �����蔻��̊Ǘ����s��
/// </summary>
class CollisionManager : public GameObject {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	CollisionManager();
	~CollisionManager();

	//==========================================================================================
	// ���e��֐�

	void Update() override;
	void Draw() override;

	/// <summary>
	/// �Ǘ��҂ւ̓o�^���s��
	/// </summary>
	/// <param name="col">�����蔻��̏��</param>
	void Add(ColliderBase* col);
	
	/// <summary>
	/// �Ǘ��҂ւ̓o�^�������s��
	/// </summary>
	/// <param name="col">�����蔻��̏��</param>
	void Remove(ColliderBase* col);

private:
	std::list<ColliderBase*> colliders;

};