#pragma once

// ���ėp
#include "../Util/Vector3.h"
#include <list>

class ColliderBase;

/// <summary>
/// �Փ˂ɂ��Ẵf�[�^������
/// </summary>
class CollisionData {
public:
	//==========================================================================================
	// ���\����
	
	struct HitPolyData {
		Vector3 collisionPoint;	// �|���S���̍��W
		Vector3 normal;	// �|���S���̖@��
	};

	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	CollisionData();
	CollisionData(bool _isCollision);

	// �����Ń����o��������
	CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const Vector3& _collisionPoint);
	CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const Vector3& _collisionPoint, const Vector3& _normal);
	CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const std::list<HitPolyData>& _hitPolyDatas);

	//==========================================================================================
	// ���Z�b�^�[

	void SetIsCollision(bool value) { isCollision = value; }
	void SetMe(ColliderBase* value) { me = value; }
	void SetOther(ColliderBase* value) { other = value; }
	void SetCollisionPoint(const Vector3& value) { hitPolyDatas.front().collisionPoint = value; }
	void SetNormal(const Vector3& value) { hitPolyDatas.front().normal = value; }
	
	//==========================================================================================
	// ���Q�b�^�[

	bool IsCollision() const { return isCollision; }
	ColliderBase* Me() const { return me; }
	ColliderBase* Other() const { return other; }
	Vector3 CollisionPoint() const { return hitPolyDatas.front().collisionPoint; }
	Vector3 Normal() const { return hitPolyDatas.front().normal; }

	std::list<HitPolyData> HitPolyDatas() const { return hitPolyDatas; }

private:
	bool isCollision;	// �Փ˂��Ă����true
	ColliderBase* me;	// �����̃R���C�_�[�̃|�C���^
	ColliderBase* other;	// ����̃R���C�_�[�̃|�C���^
	std::list<HitPolyData> hitPolyDatas;	// ���������|���S���̏��
};
