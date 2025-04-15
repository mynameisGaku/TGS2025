#include "ShadowMap.h"

// ◇汎用
#include "../Library/myDxLib.h"
#include "../Library/resourceLoader.h"

namespace {

	static const Vector3 SHADOW_MAP_DRAW_AREA = Vector3(-1200.0f);
	static const Vector3 SHADOW_MAP_DRAW_OFFSET = Vector3(0.0f, 0.0f, 500.0f);

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

void ShadowMap::DrawBegin() {

	if (isActive == false)
		return;

	// カメラの座標
	VECTOR camPos = GetCameraPosition();

	// Direct3Dで自動適用されるビューポート行列を取得する
	MATRIX camM = GetCameraAPIViewportMatrix();

	// 相対座標とビューボート行列で影計算範囲を求める
	VECTOR offset = VTransform(SHADOW_MAP_DRAW_OFFSET, camM);

	// 影計算をするエリア範囲
	SetShadowMapDrawArea(hShadowMap, camPos + offset, camPos + SHADOW_MAP_DRAW_AREA);

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
