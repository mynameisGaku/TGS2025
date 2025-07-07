#include "src/util/shadow_map/ShadowMap.h"

// ◇汎用
#include "framework/myDxLib.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/common/camera/CameraManager.h"

namespace {

	static const Vector3 SHADOW_MAP_DRAW_AREA = Vector3(-2000.0f);
	static const Vector3 SHADOW_MAP_DRAW_OFFSET = Vector3(.0f, 0.0f, 1000.0f);

	int hShadowMap = -1;		// シャドウマップのハンドラ
	bool isActive = false;		// 稼働しているか
}

void ShadowMap::Init() {

	// シャドウマップを生成する
	if (hShadowMap == -1)
		hShadowMap = MakeShadowMap(4096, 4096);

	// 影を出すライトを決める(真下に向ける)
	SetShadowMapLightDirection(hShadowMap, VGet(0.5, -1.0f, 0.0f));

	SetIsActive(true);
}

void ShadowMap::DrawBegin(int cameraIndex) {

	if (isActive == false)
		return;

	// カメラの座標
	Camera* camera = CameraManager::GetCamera(cameraIndex);
	if (camera == nullptr) {
		return; // カメラが存在しない場合は何もしない
	}

	Vector3 camPos = camera->transform->Global().position;

	VECTOR offset = VTransform(SHADOW_MAP_DRAW_OFFSET, GetCameraAPIViewportMatrix());

	// 影計算をするエリア範囲
	SetShadowMapDrawArea(hShadowMap, camPos - VGet(500, 500, 500), camPos + offset);

	// 影計算をするエリア範囲
	SetShadowMapDrawArea(hShadowMap, camPos, camPos - offset);

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
