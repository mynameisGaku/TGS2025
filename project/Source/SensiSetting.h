#pragma once

// ◇継承元
#include "SettingBase.h"

/// <summary>
/// 感度設定画面
/// </summary>
class SensiSetting : public SettingBase {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ
	
	SensiSetting();
	~SensiSetting();

	//==========================================================================================
	// ▼各種関数

	void Update() override;

	void LoadCsv(std::string filename) override;
	void SaveCsv(std::string filename) override;
	void Apply() override;

private:
	Vector2 mouseSensitivity;	// マウス感度
	Vector2 stickSensitivity;	// スティック感度

	enum CsvElement {
		Label = 0,		// 設定項目名
		MouseSensiX,	// マウス感度 X軸
		MouseSensiY,	// マウス感度 Y軸
		StickSensiX,	// スティック感度 X軸
		StickSensiY,	// スティック感度 Y軸
	};
};