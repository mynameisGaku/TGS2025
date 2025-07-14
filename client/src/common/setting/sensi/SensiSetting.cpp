#include "src/common/setting/sensi/SensiSetting.h"

#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"
#include <src/util/ptr/PtrUtil.h>
#include "src/util/math/MathUtil.h"

using namespace KeyDefine;

SensiSetting::SensiSetting() {

	stickSensitivity = PadController::StickSensitivity();
	mouseSensitivity = MouseController::Info().sensitivity;

	sliders.push_back(new UI_Slider("マウス感度 X軸", RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 125.0f)), &mouseSensitivity.x, 0.1f, MOUSE_SENSI_MAX.x, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("マウス感度 Y軸", RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 200.0f)), &mouseSensitivity.y, 0.1f, MOUSE_SENSI_MAX.y, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	
	sliders.push_back(new UI_Slider("スティック感度 X軸", RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 275.0f)), &stickSensitivity.x, 0.1f, STICK_SENSI_MAX.x, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("スティック感度 Y軸", RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 350.0f)), &stickSensitivity.y, 0.1f, STICK_SENSI_MAX.y, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));

	isAdjustable = false;
}

SensiSetting::~SensiSetting() {

	for (auto& itr : sliders) {
		// UI_Managerから切り離す
		UI_Manager::Detach(itr);

		// 実体を削除する
		PtrUtil::SafeDelete(itr);
	}

	sliders.clear();
}

void SensiSetting::Update() {

	SettingBase::Update();

	if (isAdjustable == false)
		return;

	Apply();
}

void SensiSetting::LoadCsv(std::string filename) {

	CsvReader* csv = new CsvReader(filename);
	if (csv == nullptr) {
		SaveCsv(filename);
		return;
	}

	for (int i = 0; i < csv->Column(); i++) {
		std::string item = csv->GetString(i, Label);	// 設定項目の名前

		if (item != "感度設定")
			continue;

		mouseSensitivity.x = csv->GetFloat(i + 1, MouseSensiX);
		mouseSensitivity.y = csv->GetFloat(i + 1, MouseSensiY);

		stickSensitivity.x = csv->GetFloat(i + 1, StickSensiX);
		stickSensitivity.y = csv->GetFloat(i + 1, StickSensiY);
	}

	delete csv;
}

void SensiSetting::SaveCsv(std::string filename) {

	std::ofstream file(filename, std::ios::app);

	file << "感度設定,マウス感度 X軸,マウス感度 Y軸,スティック感度 X軸,スティック感度 Y軸\n";	// Csvに定型文を書き込む
	std::vector<std::string> data;	// ファイルに書き出す情報を行ごとにまとめる

	data.push_back("");
	data.push_back(std::to_string(MathUtil::Rounding(mouseSensitivity.x, 1)));
	data.push_back(std::to_string(MathUtil::Rounding(mouseSensitivity.y, 1)));
	data.push_back(std::to_string(MathUtil::Rounding(stickSensitivity.x, 1)));
	data.push_back(std::to_string(MathUtil::Rounding(stickSensitivity.y, 1)));

	bool newLine = false;	// 行の最後の文字に改行が付いているか

	for (auto dat = data.begin(); dat != data.end(); dat++) {
		// 改行が無く、カンマも無い場合、カンマを追記する
		if ((*dat).find('\n') == std::string::npos) {
			if ((*dat).find(',') == std::string::npos)
				(*dat) += ',';
		}
		// 改行がある場合、newLineをTrueにする
		else
			newLine = true;

		file << (*dat).c_str();	// ファイルに書き出し
	}

	// 改行が無ければ改行する
	if (newLine == false)
		file << '\n';

	data.clear();	// データを解放する

	file.close();	// ファイルを閉じる

	OutputDebugString((filename + "にデータを保存しました。\n").c_str());
}

void SensiSetting::Apply() {

	mouseSensitivity.x = MathUtil::Rounding(mouseSensitivity.x, 2);
	mouseSensitivity.y = MathUtil::Rounding(mouseSensitivity.y, 2);
	stickSensitivity.x = MathUtil::Rounding(stickSensitivity.x, 2);
	stickSensitivity.y = MathUtil::Rounding(stickSensitivity.y, 2);

	MouseController::SetMouseSensitivity(mouseSensitivity);
	PadController::SetStickSensitivity(stickSensitivity);
}
