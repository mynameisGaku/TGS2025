#pragma once
#include "Object3D.h"
#include "CharaDefine.h"

class ColliderCapsule;

/// <summary>
/// �L�����N�^�[�̋z���L���b�`�̓����蔻��̃N���X
/// </summary>
/// <author>�����h�l</author>
class Catcher : public Object3D
{
public:
	Catcher();
	~Catcher();

	void Init(CharaDefine::CharaTag tag);
	void Update() override;
	void Draw() override;

	void SetColliderActive(bool isActive);
private:
	ColliderCapsule* m_Collider;	// �����蔻��
};
