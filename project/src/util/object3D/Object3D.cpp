#include "src/util/object3D/Object3D.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/util/ptr/PtrUtil.h"

Object3D::Object3D() : Object3D(-1, Transform()) {}

Object3D::Object3D(const Transform& _transform) : Object3D(-1, _transform) {}

Object3D::Object3D(const int& model, const Transform& _transform) {

	SetTransform(_transform);
	SetModel(model);

	localMatrix = MGetIdent();
	isModelDraw = true;
	isAxisDraw = false;
}

Object3D::~Object3D() {

	if (hModel != -1) {
		ResourceLoader::MV1DeleteModel(hModel);
		hModel = -1;
	}

	PtrUtil::SafeDelete(transform);
}

void Object3D::Draw() {

	GameObject::Draw();

	if (IsActive() == false)
		return;

#ifdef _DEBUG
	if (isAxisDraw)
		DrawAixs();
#endif // _DEBUG

	if (hModel < 0 || isModelDraw == false)
		return;

	MV1SetMatrix(hModel, localMatrix * transform->Global().Matrix());	// MATRIXをモデルに適応
	MV1DrawModel(hModel);		// モデルの描画
}

void Object3D::DrawAixs() {

	Vector3 pos = transform->Global().position;
	MATRIX mRot = transform->RotationMatrix();

	Vector3 xAxis = pos + (Vector3::UnitX * 50.0f);	// X軸
	Vector3 yAxis = pos + (Vector3::UnitY * 50.0f);		// Y軸
	Vector3 zAxis = pos + (Vector3::UnitZ * 50.0f);// Z軸

	int xColor = GetColor(255, 0, 0);	// 各軸の色(赤)
	int yColor = GetColor(0, 255, 0);	// 各軸の色(緑)
	int zColor = GetColor(0, 0, 255);	// 各軸の色(青)

	DrawLine3D(pos, xAxis * mRot, xColor);	// 各軸の線を描画
	DrawLine3D(pos, yAxis * mRot, yColor);	// 各軸の線を描画
	DrawLine3D(pos, zAxis * mRot, zColor);	// 各軸の線を描画

	DrawSphere3D(pos + xAxis * mRot, 4, 4, xColor, xColor, false);
	DrawSphere3D(pos + yAxis * mRot, 4, 4, yColor, yColor, false);
	DrawSphere3D(pos + zAxis * mRot, 4, 4, zColor, zColor, false);
}

void Object3D::SetTransform(const Transform& trs) {

	PtrUtil::SafeDelete(transform);
	transform = new Transform(trs);
}

void Object3D::SetModel(const int& handle) {

	if (hModel != -1) {
		ResourceLoader::MV1DeleteModel(hModel);
		hModel = -1;
	}

	hModel = handle;

	if (hModel != -1 && transform != nullptr)
		MV1SetMatrix(hModel, transform->Global().Matrix());	// MATRIXをモデルに適応
}
