#pragma once
#include "src/util/object3D/Object3D.h"
#include <string>

class ColliderCapsule;
class Chara;
class Ball;
class ForceFieldManager;
class ForceFieldCorn;

/// <summary>
/// �L�����N�^�[�̋z���L���b�`�̓����蔻��̃N���X
/// </summary>
/// <author>�����h�l</author>
class Catcher : public Object3D
{
public:
	Catcher();
	~Catcher();

	void Init(std::string tag);
	void Update() override;
	void Draw() override;

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;

	void SetColliderActive(bool isActive);
	void SetParent(Chara* parent) { m_pParent = parent; }

	bool IsColliderActive() const;
	// �L���b�`�p�x���Ƀ{�[�������邩�ǂ���
	bool CanCatch(Ball* ball) const;

	// �L���b�`��������
	void CatchSuccese(Ball* ball);

private:
	ColliderCapsule* m_pCollider;	// �����蔻��
	Chara* m_pParent;			// �e�L�����N�^�[
	ForceFieldManager* m_pForceFieldManager;	// �z���ݗp�A�͏�}�l�[�W���[
	ForceFieldCorn* m_pWindArea;	// �z���݃G���A

	void createWindArea();
	void deleteWindArea();
};
