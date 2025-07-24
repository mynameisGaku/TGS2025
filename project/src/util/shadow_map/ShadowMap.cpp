#include "src/util/shadow_map/ShadowMap.h"

// ◇汎用
#include "framework/myDxLib.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/common/camera/CameraManager.h"

namespace {

	static const Vector3 SHADOW_MAP_DRAW_AREA = Vector3(1500.0f);	// 影描画を行う範囲

	int hShadowMap = -1;		// シャドウマップのハンドラ
	bool isActive = false;		// 稼働しているか
}

void ShadowMap::Init(int sizeX, int sizeY) {

	// シャドウマップを生成する
	if (hShadowMap == -1)
		hShadowMap = MakeShadowMap(sizeX, sizeY);

	// 影を出すライトを決める(真下に向ける)
	SetLightDirection(Vector3(0.5f, -1.0f, 0.0f));
	SetLightAmbColor(COLOR_F(0.4f, 0.4f, 0.8f, 1.0f));
	SetLightDifColor(COLOR_F(0.6f, 0.6f, 0.6f, 1.0f));
	SetShadowMapLightDirection(hShadowMap, GetLightDirection());

	// 稼働中に設定
	SetIsActive(true);
}

void ShadowMap::DrawBegin(int cameraIndex) {

	if (isActive == false)
		return;

	Camera* camera = CameraManager::GetCamera(cameraIndex);
	if (not camera)
		return;

	Vector3 camPos = camera->transform->Global().position;
	Vector3 offset = VTransform(SHADOW_MAP_DRAW_AREA, MGetRotY(camera->transform->Global().rotation.y));

	Vector3 minPosition = camPos - offset;	// 影描画開始地点
	Vector3 maxPosition = camPos + offset;	// 影描画終了地点

	// 影計算をするエリア範囲
	SetShadowMapDrawArea(hShadowMap, minPosition, maxPosition);

	ShadowMap_DrawSetup(hShadowMap);
}

void ShadowMap::DrawEnd() {

	if (isActive == false)
		return;

	ShadowMap_DrawEnd();
	SetUseShadowMap(0, hShadowMap);
}

void ShadowMap::CleanUp() {

	SetUseShadowMap(0, -1);
}

void ShadowMap::Draw(void (*func)(), int cameraIndex) {

	ShadowMap::DrawBegin(cameraIndex);
	func();
	ShadowMap::DrawEnd();
	func();
}

void ShadowMap::Release() {

	if (hShadowMap != -1) {
		DeleteShadowMap(hShadowMap);
		hShadowMap = -1;
	}
}

void ShadowMap::SetIsActive(bool value) {

	isActive = value;
}

bool ShadowMap::IsActive() {

	return isActive;
}
