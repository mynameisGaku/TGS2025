//#include "collisionObserver.h"
//
//// ◇汎用
//#include <list>
//#include <unordered_map>
//
//namespace {
//
//	std::list<const ColliderBase*> colliders;	// 観測する当たり判定
//	std::unordered_map<ColDefine::Tag, bool> isDisplay;	// 当たり判定の可視化をするか
//}
//
//void CollisionObserver::AddCollider(const ColliderBase* collider) {
//
//	colliders.push_back(collider);
//}
//
//bool CollisionObserver::DetachCollider(const ColliderBase* collider) {
//
//	for (auto itr = colliders.begin(); itr != colliders.end(); itr++) {
//		if ((*itr) == collider) {
//			colliders.erase(itr);
//			return true;
//		}
//	}
//
//	return false;
//}
//
//void CollisionObserver::SetIsDisplay(ColDefine::Tag tag, bool value) {
//
//	isDisplay[tag] = value;
//}
//
//bool CollisionObserver::IsDisplay(ColDefine::Tag tag) {
//
//	return isDisplay[tag];
//}
