#include "src/common/setting/volume/VolumeSetting.h"

#include "src/util/sound/SoundManager.h"

VolumeSetting::VolumeSetting() {

	auto volumes = SoundManager::CategoryVolumeRate();
	if (volumes == nullptr)
		return;

	sliders.push_back(new UI_Slider("全体音量",		RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 125.0f)), &(*volumes)[SoundCategory::cMaster],	0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("BGM音量",		RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 200.0f)), &(*volumes)[SoundCategory::cBGM],		0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("SE音量",		RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 275.0f)), &(*volumes)[SoundCategory::cSE],		0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("ボイス音量",	RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 350.0f)), &(*volumes)[SoundCategory::cVoice],	0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));
	sliders.push_back(new UI_Slider("環境音量",		RectTransform(Anchor::Preset::MiddleUp, Vector2(150.0f, 425.0f)), &(*volumes)[SoundCategory::cENV],		0.0f, 1.0f, Vector2(250.0f, 25.0f), sliderColor, "%.1f", fontInfo));

	isAdjustable = false;
}

VolumeSetting::~VolumeSetting() {

	for (auto& itr : sliders) {
		// UI_Managerから切り離す
		UI_Manager::Detach(itr);

		// 実体を削除する
		Function::DeletePointer(itr);
	}

	sliders.clear();
}

void VolumeSetting::Update() {

	SettingBase::Update();

	if (isAdjustable == false)
		return;

	Apply();
}

void VolumeSetting::LoadCsv(std::string filename) {

	auto volumes = SoundManager::CategoryVolumeRate();
	if (volumes == nullptr)
		return;

	CsvReader* csv = new CsvReader(filename);
	if (csv == nullptr) {
		SaveCsv(filename);
		delete csv;
		return;
	}

	for (int i = 0; i < csv->Column(); i++) {
		std::string item = csv->GetString(i, Label);	// 設定項目の名前

		if (item != "音量設定")
			continue;

		(*volumes)[SoundCategory::cMaster]	= csv->GetFloat(i + 1, Volume_Master);
		(*volumes)[SoundCategory::cBGM]		= csv->GetFloat(i + 1, Volume_BGM);
		(*volumes)[SoundCategory::cSE]		= csv->GetFloat(i + 1, Volume_SE);
		(*volumes)[SoundCategory::cVoice]	= csv->GetFloat(i + 1, Volume_Voice);
		(*volumes)[SoundCategory::cENV]		= csv->GetFloat(i + 1, Volume_ENV);
	}

	delete csv;
}

void VolumeSetting::SaveCsv(std::string filename) {

	using namespace Function;

	auto volumes = SoundManager::CategoryVolumeRate();
	if (volumes == nullptr)
		return;

	std::ofstream file(filename, std::ios::app);

	file << "音量設定,全体音量,背景音量,効果音量,声音量,環境音量\n";	// Csvに定型文を書き込む
	std::vector<std::string> data;	// ファイルに書き出す情報を行ごとにまとめる

	data.push_back("");
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cMaster],	1)));
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cBGM],		1)));
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cSE],		1)));
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cVoice],	1)));
	data.push_back(std::to_string(Rounding((*volumes)[SoundCategory::cENV],		1)));

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
	if (!newLine)
		file << '\n';

	data.clear();	// データを解放する

	file.close();	// ファイルを閉じる

	OutputDebugString((filename + "にデータを保存しました。\n").c_str());
}

void VolumeSetting::Apply() {

	SoundManager::ApplyCategoryVolume();
}
