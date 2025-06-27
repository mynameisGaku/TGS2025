#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/common/component/collider/CollisionDefine.h"
#include <memory>

class ForceBase;
class ColliderBase;

/// <summary>
/// �͈͓��̃I�u�W�F�N�g�Ƀ��[���ɏ]���ė͂�������
/// </summary>
/// <author>�����h�l</author>
class ForceFieldBase : public Object3D
{
public:
	ForceFieldBase();
	virtual ~ForceFieldBase();

	void SetColTag(const ColDefine::Tag& tag);
	void SetColTargetTags(const std::list<ColDefine::Tag>& targetTags);
	void SetForce(std::unique_ptr<ForceBase> force);

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	virtual void CollisionEvent(const CollisionData& colData) override;

protected:
	std::unique_ptr<ForceBase> m_Force;	// �͏�̏���
	ColliderBase* m_pCollider;	// �����蔻��
};
