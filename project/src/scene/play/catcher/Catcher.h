#pragma once
#include "src/util/object3D/Object3D.h"
#include <string>

class ColliderCapsule;
class CharaBase;
class Ball;

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
	void SetParent(CharaBase* parent) { m_Parent = parent; }

	bool IsColliderActive() const;
	// �L���b�`�p�x���Ƀ{�[�������邩�ǂ���
	bool CanCatch(Ball* ball) const;
private:
	ColliderCapsule* m_Collider;	// �����蔻��
	CharaBase* m_Parent;			// �e�L�����N�^�[
};
