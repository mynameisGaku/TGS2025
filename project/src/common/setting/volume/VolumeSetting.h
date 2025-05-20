#pragma once

// ◇継承元
#include "SettingBase.h"

/// <summary>
/// 音量設定画面
/// </summary>
class VolumeSetting : public SettingBase {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ
	
	VolumeSetting();
	~VolumeSetting();

	//==========================================================================================
	// ▼各種関数

	void Update() override;

	void LoadCsv(std::string filename) override;
	void SaveCsv(std::string filename) override;
	void Apply() override;

private:
	enum CsvElement {
		Label = 0,		// 設定項目名
		Volume_Master,	// 全体音量
		Volume_BGM,		// 背景音量
		Volume_SE,		// 効果音量
		Volume_Voice,	// 声音量
		Volume_ENV,		// 環境音量
	};
};