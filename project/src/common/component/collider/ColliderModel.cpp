#include "colliderModel.h"

// ◇個別で必要な物
#include "../Object3D.h"

ColliderModel::ColliderModel() {

	shape = ColDefine::Shape::sModel;
	hModel = -1;
	refresh = false;

	Object3D* obj = Parent<Object3D>();
	if (obj != nullptr)
		hModel = obj->Model();

	if (hModel >= 0) {
		MV1SetupCollInfo(hModel, -1, 8, 8, 8);	//コリジョン情報を構築する
		MV1RefreshCollInfo(hModel, -1);	//コリジョン情報を更新する
	}
}

void ColliderModel::UpdateData() {

	ColliderBase::UpdateData();

	if (hModel < 0)
		return;

	if (refresh) {
		MV1SetMatrix(hModel, transform->Global().Matrix());
		MV1RefreshCollInfo(hModel, -1);
	}
}
