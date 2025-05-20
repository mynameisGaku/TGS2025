#include "colliderModel.h"

// ���ʂŕK�v�ȕ�
#include "../Object3D.h"

ColliderModel::ColliderModel() {

	shape = ColDefine::Shape::sModel;
	hModel = -1;
	refresh = false;

	Object3D* obj = Parent<Object3D>();
	if (obj != nullptr)
		hModel = obj->Model();

	if (hModel >= 0) {
		MV1SetupCollInfo(hModel, -1, 8, 8, 8);	//�R���W���������\�z����
		MV1RefreshCollInfo(hModel, -1);	//�R���W���������X�V����
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
