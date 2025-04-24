#pragma once
#include "Object3D.h"
#include "CharaDefine.h"

class ColliderCapsule;
class CharaBase;

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

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;

	void SetColliderActive(bool isActive);
	void SetParent(CharaBase* parent) { m_Parent = parent; }
private:
	ColliderCapsule* m_Collider;	// �����蔻��
	CharaBase* m_Parent;			// �e�L�����N�^�[
};
