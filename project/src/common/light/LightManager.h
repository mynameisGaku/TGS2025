#pragma once

// ◇個別で必要な物
#include "LightDefine.h"
#include "Light.h"

/// <summary>
/// ライトを管理するnamespcae
/// </summary>
namespace LightManager {

	//==========================================================================================
	// ▼各種関数

	void Init();
	void Update();
	void Draw();
	void Release();

	void CreateLightDir();
	void CreateLightPoint();
	void CreateLightSpot();

	//==========================================================================================
	// ▼セッター

	void SetIsActive(const bool& value, const LightDefine::LightType& type);

	//==========================================================================================
	// ▼ゲッター

	Light* GetLight(const LightDefine::LightType& type);

}