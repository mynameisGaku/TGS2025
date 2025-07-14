#pragma once
#include "src/util/object3D/Object3D.h"
#include <string>

class ColliderCapsule;
class Chara;

class Tackler : public Object3D
{
public:
    Tackler();
    ~Tackler();

	void Init(std::string tag);
	void Update() override;
	void Draw() override;

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;

	void SetColliderActive(bool isActive);
	void SetParent(Chara* parent) { m_Parent = parent; }

	bool IsColliderActive() const;
private:
	ColliderCapsule* m_Collider;	// �����蔻��
	Chara* m_Parent;			// �e�L�����N�^�[
};