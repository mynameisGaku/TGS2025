#include "stageObject.h"

// ◇汎用
#include "../Library/resourceLoader.h"

StageObject::StageObject(StageObjInfo _info, const Transform& _transform, int _id, bool _collider) : 
	Object3D(_info.hModel, _transform) {

	info = _info;

	id = _id;
	isCollider = _collider;
	isHitModelDraw = true;

	// モデルのアルファ値が128以上の場合のみ描画する
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

	// 当たり判定モデルの可視化
	if (isCollider && isHitModelDraw) {
		MV1SetMatrix(info.hHitModel, transform->Global().Matrix());	// MATRIXをモデルに適応
		MV1DrawModel(info.hHitModel);		// モデルの描画
	}
}
