#include "CollisionData.h"

CollisionData::CollisionData() : 
	CollisionData(false) {}

CollisionData::CollisionData(bool _isCollision) : 
	CollisionData(_isCollision, nullptr, nullptr, V3::ZERO)	{}

CollisionData::CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const Vector3& _collisionPoint) : 
	CollisionData(_isCollision, _me, _other, _collisionPoint, V3::ZERO)	{}

CollisionData::CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const Vector3& _collisionPoint, const Vector3& _normal) {

	isCollision = _isCollision;
	me = _me;
	other = _other;

	HitPolyData data;
	data.collisionPoint = _collisionPoint;
	data.normal = _normal;

	hitPolyDatas.push_back(data);
}

CollisionData::CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const std::list<HitPolyData>& _hitPolyDatas) {

	isCollision = _isCollision;
	me = _me;
	other = _other;

	hitPolyDatas = _hitPolyDatas;
}
