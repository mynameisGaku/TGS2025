#pragma once

// ���ʂŕK�v�ȕ�
#include "LightDefine.h"
#include "Light.h"

/// <summary>
/// ���C�g���Ǘ�����namespcae
/// </summary>
namespace LightManager {

	//==========================================================================================
	// ���e��֐�

	void Init();
	void Update();
	void Draw();
	void Release();

	void CreateLightDir();
	void CreateLightPoint();
	void CreateLightSpot();

	//==========================================================================================
	// ���Z�b�^�[

	void SetIsActive(const bool& value, const LightDefine::LightType& type);

	//==========================================================================================
	// ���Q�b�^�[

	Light* GetLight(const LightDefine::LightType& type);

}