#include "stageObject.h"

// ���ėp
#include "../Library/resourceLoader.h"

StageObject::StageObject(StageObjInfo _info, const Transform& _transform, int _id, bool _collider) : 
	Object3D(_info.hModel, _transform) {

	info = _info;

	id = _id;
	isCollider = _collider;
	isHitModelDraw = true;

	// ���f���̃A���t�@�l��128�ȏ�̏ꍇ�̂ݕ`�悷��
	//MV1SetMaterialDrawAlphaTestAll(hModel, true, DX_CMP_GREATER, 128);
}

StageObject::~StageObject() {

	if (info.hHitModel > 0) {
		ResourceLoader::MV1DeleteModel(info.hHitModel);
		info.hHitModel = -1;
	}
}

void StageObject::Draw() {

	Object3D::Draw();

	// �����蔻�胂�f���̉���
	if (isCollider && isHitModelDraw) {
		MV1SetMatrix(info.hHitModel, transform->Global().Matrix());	// MATRIX�����f���ɓK��
		MV1DrawModel(info.hHitModel);		// ���f���̕`��
	}
}
