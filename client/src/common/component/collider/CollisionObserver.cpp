//#include "collisionObserver.h"
//
//// ���ėp
//#include <list>
//#include <unordered_map>
//
//namespace {
//
//	std::list<const ColliderBase*> colliders;	// �ϑ����铖���蔻��
//	std::unordered_map<ColDefine::Tag, bool> isDisplay;	// �����蔻��̉��������邩
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
