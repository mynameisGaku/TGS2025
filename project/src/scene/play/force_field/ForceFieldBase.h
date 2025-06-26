#pragma once
#include "src/util/object3D/Object3D.h"
#include <memory>

class ForceBase;

class ForceFieldBase : public Object3D
{
public:
	ForceFieldBase();
	~ForceFieldBase();

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	void CollisionEvent(const CollisionData& colData) override;

protected:
	std::unique_ptr<ForceBase> m_force;	// �͏�̏���
};
