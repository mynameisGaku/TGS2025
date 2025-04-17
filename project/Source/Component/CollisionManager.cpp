#include "CollisionManager.h"

// ◇汎用
#include "../../Library/time.h"

// ◇個別で必要な物
#include "CollisionFunc.h"
#include "ColliderSphere.h"
#include "ColliderCapsule.h"
#include "ColliderModel.h"

CollisionManager::CollisionManager() {

	colliders.clear();
}

CollisionManager::~CollisionManager() {

	for (auto itr = colliders.begin(); itr != colliders.end();) {
		ColliderBase* c = *itr;
		itr = colliders.erase(itr);
		if (itr == colliders.end()) break;
	}

	colliders.clear();
}

void CollisionManager::Update() {

	if (Time::LapseRate() == 0.0f)
		return;

	for (ColliderBase* col : colliders) {
		if (col->IsActive() == false)
			continue;

		col->UpdateData();
	}

	for (auto itr1 = colliders.begin(); itr1 != colliders.end(); itr1++) {

		ColliderBase* col = *itr1;

		if (col->IsActive() == false)
			continue;

		for (auto itr2 = itr1; itr2 != colliders.end(); itr2++) {

			if (itr1 == itr2)
				continue;

			ColliderBase* col1 = *itr1;
			ColliderBase* col2 = *itr2;

			if (col2->IsActive() == false)
				continue;

			// 当たり判定の対象者かどうか
			if (col2->IsTarget(col1->Tag()) == false)
				continue;

			// 一度きりの当たり判定で二回目の場合スキップ
			if (col1->HittedDataPtr() != nullptr && col1->HittedDataPtr()->IsHitted(col2)) continue;
			if (col2->HittedDataPtr() != nullptr && col2->HittedDataPtr()->IsHitted(col1)) continue;

			CollisionData* data = ColFunction::ColCheck(col1, col2);
			if (data == nullptr)
				continue;

			if (data->IsCollision()) {

				if (col1->HittedDataPtr() != nullptr) {
					col1->HittedDataPtr()->AddHitted(col2);
				}
				if (col2->HittedDataPtr() != nullptr) {
					col2->HittedDataPtr()->AddHitted(col1);
				}

				CollisionData col1Data = *data;
				CollisionData col2Data = *data;

				col1Data.SetMe(col1);
				col1Data.SetOther(col2);

				col2Data.SetMe(col2);
				col2Data.SetOther(col1);

				col1->parent->CollisionEvent(col1Data);
				col2->parent->CollisionEvent(col2Data);
			}

			delete data;
			data = nullptr;
		}
	}
}

void CollisionManager::Draw() {


}

void CollisionManager::Add(ColliderBase* col) {

	colliders.push_back(col);
}

void CollisionManager::Remove(ColliderBase* col) {

	for (auto itr = colliders.begin(); itr != colliders.end();) {
		ColliderBase* c = *itr;

		if (c == col) {
			itr = colliders.erase(itr);
			return;
		}
		else {
			itr++;
		}
	}
}
