#pragma once

// ◇汎用
#include "../Util/Vector3.h"
#include <list>

class ColliderBase;

/// <summary>
/// 衝突についてのデータを持つ
/// </summary>
class CollisionData {
public:
	//==========================================================================================
	// ▼構造体
	
	struct HitPolyData {
		Vector3 collisionPoint;	// ポリゴンの座標
		Vector3 normal;	// ポリゴンの法線
	};

	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	CollisionData();
	CollisionData(bool _isCollision);

	// 引数でメンバを初期化
	CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const Vector3& _collisionPoint);
	CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const Vector3& _collisionPoint, const Vector3& _normal);
	CollisionData(bool _isCollision, ColliderBase* _me, ColliderBase* _other, const std::list<HitPolyData>& _hitPolyDatas);

	//==========================================================================================
	// ▼セッター

	void SetIsCollision(bool value) { isCollision = value; }
	void SetMe(ColliderBase* value) { me = value; }
	void SetOther(ColliderBase* value) { other = value; }
	void SetCollisionPoint(const Vector3& value) { hitPolyDatas.front().collisionPoint = value; }
	void SetNormal(const Vector3& value) { hitPolyDatas.front().normal = value; }
	
	//==========================================================================================
	// ▼ゲッター

	bool IsCollision() const { return isCollision; }
	ColliderBase* Me() const { return me; }
	ColliderBase* Other() const { return other; }
	Vector3 CollisionPoint() const { return hitPolyDatas.front().collisionPoint; }
	Vector3 Normal() const { return hitPolyDatas.front().normal; }

	std::list<HitPolyData> HitPolyDatas() const { return hitPolyDatas; }

private:
	bool isCollision;	// 衝突していればtrue
	ColliderBase* me;	// 自分のコライダーのポインタ
	ColliderBase* other;	// 相手のコライダーのポインタ
	std::list<HitPolyData> hitPolyDatas;	// 当たったポリゴンの情報
};
