#include "src/util/sound/SoundManager.h"

#include "src/util/math/MathUtil.h"
#include "src/util/ptr/PtrUtil.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/util/file/csv/CsvReader.h"
#include "src/util/file/json/settings_json.h"
#include <vendor/magic_enum/magic_enum.hpp>

namespace {

	std::unordered_map<std::string, SoundInfo>* soundInfoDatas;	// サウンドの情報を持つ
	std::unordered_map<SoundCategory, float>* categoryVolumeRate;	// カテゴリ―別の音量倍率
	std::list<SoundBase*>* sounds;	// 再生されているサウンド
	std::string* csvFilePath;		// 読み込み用Csvファイルのパス

	bool initialize = false;	// 初期化処理が行われたか
}

void SoundManager::Init() {

	if (soundInfoDatas == nullptr) {
		soundInfoDatas = new std::unordered_map<std::string, SoundInfo>();
	}

	if (categoryVolumeRate == nullptr) {
		categoryVolumeRate = new std::unordered_map<SoundCategory, float>();

		for (int i = 0; i < static_cast<int>(SoundCategory::cMax); i++) {
			SoundCategory category = static_cast<SoundCategory>(i);	// カテゴリー
			(*categoryVolumeRate)[category] = 1.0f;
		}
	}

	if (sounds == nullptr) {
		sounds = new std::list<SoundBase*>();
	}

	initialize = false;
}

void SoundManager::Start() {

	if (initialize)
		return;

#ifdef IMGUI
		InitImGui();
#endif // IMGUI

	initialize = true;
}

void SoundManager::Update() {

	// 初期化処理がまだ行われていない場合
	if (initialize == false)
		Start();

	if (categoryVolumeRate != nullptr) {
		for (auto& itr : *categoryVolumeRate) {
			itr.second = MathUtil::Rounding(itr.second, 2);
		}
	}

	// 再生中のサウンドがない場合
	if (sounds == nullptr)
		return;

	// 再生中のサウンドのUpdateを呼び出す
	for (auto itr = sounds->begin(); itr != sounds->end();) {
		if ((*itr)->IsPlaying() && (*itr)->IsDestroy() == false) {
			(*itr)->Update();
			itr++;
			continue;
		}

		delete* itr;
		itr = sounds->erase(itr);
		if (itr == sounds->end()) break;
	}
}

void SoundManager::Release() {

	StopAll();

	// 再生中のサウンドの実体を削除する
	if (sounds != nullptr) {
		for (auto itr = sounds->begin(); itr != sounds->end();) {
			if (*itr == nullptr) {
				itr++;
				continue;
			}

			delete* itr;
			itr = sounds->erase(itr);
			if (itr == sounds->end()) break;
		}

		sounds->clear();
		PtrUtil::SafeDelete(sounds);
	}

	// カテゴリー別の音量倍率を削除する
	PtrUtil::SafeDelete(categoryVolumeRate);

	// 保持していた読み込み用Csvファイルのパスを削除する
	PtrUtil::SafeDelete(csvFilePath);

	// 読み込んだ全てのサウンドデータを解放する
	AllReleaseInfo();
}

void SoundManager::ApplyCategoryVolume() {

	// サウンドデータの保存が可能か
	if (soundInfoDatas == nullptr)
		return;

	for (auto& itr : *soundInfoDatas) {
		itr.second.curVolume = static_cast<int>(itr.second.defVolume * (*categoryVolumeRate)[itr.second.category]);
	}
}

void SoundManager::Load(const SoundInfo& info) {

	// サウンドデータの保存が可能か
	if (soundInfoDatas == nullptr)
		return;

	// 同じデータが既に存在している場合、追加しない
	if ((*soundInfoDatas).contains(info.typeName))
		return;

	// サウンド情報
	SoundInfo newInfo = SoundInfo(info);

	// ハンドルが存在しない場合
	if (newInfo.handle < 0)
		newInfo.handle = ResourceLoader::LoadSoundMem(info.fileName + info.typeName);

	// データの登録
	(*soundInfoDatas)[info.typeName] = newInfo;
}

void SoundManager::LoadToCsv(const std::string& filename) {

	// サウンドデータの保存が可能か
	if (soundInfoDatas == nullptr)
		return;

	// Csvデータを取得する
	CsvReader* csv = new CsvReader(filename);

	// Csvデータが存在しない場合
	if (csv == nullptr)
		return;

	// 再読み込み用にsvファイルのパスを保存しておく
	if (csvFilePath == nullptr)
		csvFilePath = new std::string();

	*csvFilePath = filename;

	for (int i = 0; i < csv->Column(); i++) {
		
		std::string beginning = csv->GetString(i, 0);	// 最初の行

		// コメントなら次の要素へ
		if (beginning.substr(0, 2) == "//")
			continue;

		// サウンドの情報
		SoundInfo info;

		//==================================================
		// ◇データ名、ファイルパスの設定

		info.typeName = beginning;
		info.fileName = csv->GetString(i, 1);
		
		//==================================================
		// ◇音源の分類設定

		std::string category = csv->GetString(i, 2);

		if (category == "BGM")
			info.category = SoundCategory::cBGM;
		else if (category == "ENV")
			info.category = SoundCategory::cENV;
		else if (category == "SE")
			info.category = SoundCategory::cSE;
		else if (category == "Voice")
			info.category = SoundCategory::cVoice;
		else
			assert(0 && "音源の分類データが間違っています。");

		//==================================================
		// ◇既定音量の設定

		info.defVolume = csv->GetInt(i, 3);
		info.curVolume = info.defVolume;

		//==================================================
		// ◇再生形式の設定

		std::string playType = csv->GetString(i, 4);

		if (playType == "LOOP")
			info.playType = DX_PLAYTYPE_LOOP;
		else if (playType == "BACK")
			info.playType = DX_PLAYTYPE_BACK;
		else if (playType == "NORMAL")
			info.playType = DX_PLAYTYPE_NORMAL;
		else
			assert(0 && "再生形式のデータが間違っています。");

		//==================================================
		// ◇読み込み処理

		Load(info);
	}

	delete csv;
}

void SoundManager::LoadFromJson(const std::string& filename)
{
	auto jsonSetting = Settings_json::Inst();
	jsonSetting->LoadSettingJson(filename, filename);

	auto json = jsonSetting->GetJson(filename);

	for (const auto& [type, value] : json["Params"].items())
	{
		const std::string& path = value["Path"];
		const auto& datas = value["Datas"];
		std::string cat = value["Category"];

		for (const auto& data : datas)
		{
			std::string fileName = data["FileName"];
			std::string name = data["Name"];
			int volume = data["Volume"];
			bool isLoop = data["IsLoop"];

			std::string fullPath = path + fileName;

			SoundInfo info{};
			info.typeName = type + "_" + name;
			info.fileName = fullPath;

			if (cat == "BGM")
				info.category = SoundCategory::cBGM;
			else if (cat == "ENV")
				info.category = SoundCategory::cENV;
			else if (cat == "SE")
				info.category = SoundCategory::cSE;
			else if (cat == "Voice")
				info.category = SoundCategory::cVoice;
			else
				info.category = SoundCategory::cMaster; // デフォルトはマスター

			info.defVolume = volume;
			info.curVolume = volume;
			info.playType = isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_NORMAL;
			info.handle = ResourceLoader::LoadSoundMem(fullPath);

			Load(info);
		}
	}
}

SoundBase* SoundManager::Play(const std::string& typeName, const std::string& label) {

	// データが存在しているかを確認する
	if (soundInfoDatas == nullptr || sounds == nullptr || CheckLoadDate(typeName) == false)
		return nullptr;

	// 既に再生中なら、停止してから再生する
	if (IsPlaying(typeName, label))
		Stop(typeName, label);

	// 実体を生成
	SoundBase* sound = new SoundBase();
	
	// 再生処理
	sound->Play(&(*soundInfoDatas)[typeName], label);

	// 再生中サウンドのリストに追加
	sounds->push_back(sound);

	return sound;
}

SoundBase* SoundManager::PlayRandom(const std::vector<std::string>& typeName, const std::string& label) {

	// データが存在しているかを確認する
	if (typeName.empty())
		return nullptr;

	// 最大数を求める
	int size = static_cast<int>(typeName.size()) - 1;

	// 乱数を取得
	int random = GetRand(size);

	return Play(typeName[random], label);
}

SoundBase* SoundManager::PlaySetPan(const std::string& typeName, const std::string& label, Vector3* playPos) {

	// データが存在しているかを確認する
	if (soundInfoDatas == nullptr || sounds == nullptr || CheckLoadDate(typeName) == false)
		return nullptr;

	// 既に再生中なら、停止してから再生する
	if (IsPlaying(typeName, label))
		Stop(typeName, label);

	// 実体を生成
	SoundBase* sound = new SoundBase();
	
	// 再生処理
	sound->PlaySetPan(&(*soundInfoDatas)[typeName], label, playPos);

	// 再生中サウンドのリストに追加
	sounds->push_back(sound);
	
	return sound;
}

void SoundManager::SetPan(const std::string& typeName, const std::string& label, Vector3* playPos) {

	// 再生中か
	SoundBase* sound = IsPlaying(typeName, label);
	if (sound == nullptr)
		return;

	// パンの設定を適応させる
	sound->SetPan(playPos);
}

SoundBase* SoundManager::PlaySetFrequency(const std::string& typeName, const std::string& label, const float& frequency) {

	// データが存在しているかを確認する
	if (soundInfoDatas == nullptr || sounds == nullptr || CheckLoadDate(typeName) == false)
		return nullptr;

	// 既に再生中なら、停止してから再生する
	if (IsPlaying(typeName, label))
		Stop(typeName, label);

	// 実体を生成
	SoundBase* sound = new SoundBase();
	
	// 再生処理
	sound->PlaySetFrequency(&(*soundInfoDatas)[typeName], label, frequency);

	// 再生中サウンドのリストに追加
	sounds->push_back(sound);

	return sound;
}

void SoundManager::SetFrequency(const std::string& typeName, const std::string& label, const float& frequency) {

	// 再生中か
	SoundBase* sound = IsPlaying(typeName, label);
	if (sound == nullptr)
		return;

	// 周波数の設定を適応させる
	sound->SetFrequency(frequency);
}

SoundBase* SoundManager::FadeIn(const std::string& typeName, const std::string& label, const float& sec, const EasingType& easing) {

	// データが存在しているかを確認する
	if (soundInfoDatas == nullptr || sounds == nullptr || CheckLoadDate(typeName) == false)
		return nullptr;

	// 既に再生中なら、停止してから再生する
	if (IsPlaying(typeName, label))
		Stop(typeName, label);

	// 実体を生成
	SoundBase* sound = new SoundBase();

	// フェード情報
	SoundFade fade;
	float begin = 0.0f;	// 始点
	float end = static_cast<float>((*soundInfoDatas)[typeName].defVolume);	// 終点
	fade.SetEasing(begin, end, sec, easing, true);

	// 再生処理
	sound->FadeIn(&(*soundInfoDatas)[typeName], label, fade);

	// 再生中サウンドのリストに追加
	sounds->push_back(sound);

	return sound;
}

SoundBase* SoundManager::FadeOut(const std::string& typeName, const std::string& label, const float& sec, const EasingType& easing, const bool& isFadeOutEnd) {

	// 再生中か
	SoundBase* sound = IsPlaying(typeName, label);
	if (sound == nullptr)
		return nullptr;

	// フェード情報
	SoundFade fade;
	float begin = static_cast<float>(sound->Info()->curVolume);
	float end = 0.0f;
	fade.SetEasing(begin, end, sec, easing, true);

	sound->FadeOut(fade, isFadeOutEnd);

	return sound;
}

SoundBase* SoundManager::IsPlaying(const std::string& typeName, const std::string& label) {

	// 再生中サウンドの管理が出来てない場合
	if (sounds == nullptr)
		return nullptr;

	for (const auto& itr : *sounds) {
		// 引数の情報と一致しているか、整合性チェックを行う
		if (itr->CheckConsistency(typeName, label))
			return itr;
	}

	return nullptr;
}

void SoundManager::Stop(const std::string& typeName, const std::string& label) {

	// 再生中か
	SoundBase* sound = IsPlaying(typeName, label);
	if (sound == nullptr)
		return;

	sound->Stop();
}

void SoundManager::StopCategory(const SoundCategory& category) {

	// 再生中サウンドの管理が出来てない場合
	if (sounds == nullptr)
		return;

	// 再生されているサウンドの中から、分類が一致した物の再生を止める
	for (const auto& itr : *sounds) {
		if (itr->Info()->category == category)
			itr->Stop();
	}
}

void SoundManager::StopAll() {

	// 再生中サウンドの管理が出来てない場合
	if (sounds == nullptr)
		return;

	for (const auto& itr : *sounds)
		itr->Stop();
}

void SoundManager::SetVolumeRate(const SoundCategory& category, const float& rate) {

	if (categoryVolumeRate == nullptr)
		return;

	(*categoryVolumeRate)[category] = rate;
}

std::unordered_map<SoundCategory, float>* SoundManager::CategoryVolumeRate() {

	return categoryVolumeRate;
}

bool SoundManager::CheckLoadDate(const std::string& typeName) {

	if (soundInfoDatas == nullptr) {
		OutputDebugString("============================================================\n");
		OutputDebugString("=   !!!!! Warning !!!!!\n");
		OutputDebugString(("=   "+ typeName + "は再生できません。\n").c_str());
		OutputDebugString("=   サウンドの情報を持つ実体が生成されていないため、実体が生成されてから再生をしてください。\n");
		OutputDebugString("============================================================\n");
		return false;
	}

	if (soundInfoDatas->contains(typeName) == false) {
		OutputDebugString("============================================================\n");
		OutputDebugString("=   !!!!! Warning !!!!!\n");
		OutputDebugString(("=   " + typeName + "は再生できません。\n").c_str());
		OutputDebugString("=   以下の場合が考えられます。\n");
		OutputDebugString("=   データが読み込めていない。データの名前が一致していない。\n");
		OutputDebugString("============================================================\n");
		return false;
	}

	return true;
}

void SoundManager::Reload() {

	// 読み込み用Csvファイルのパスが存在しない場合
	if (csvFilePath == nullptr)
		return;

	// 読み込んだ全てのエフェクトデータを解放する
	AllReleaseInfo();

	// 再初期化処理
	Init();

	// Csvからエフェクト情報を読み込む
	LoadToCsv(*csvFilePath);
}

void SoundManager::AllReleaseInfo() {

	// サウンドデータが存在していない場合
	if (soundInfoDatas == nullptr)
		return;

	for (auto itr = soundInfoDatas->begin(); itr != soundInfoDatas->end();) {
		ResourceLoader::DeleteSoundMem((*itr).second.handle);
		itr = soundInfoDatas->erase(itr);
	}

	PtrUtil::SafeDelete(soundInfoDatas);
}

#ifdef IMGUI

#include "src/util/debug/imgui/imGuiManager.h"

void SoundManager::InitImGui() {

	if (soundInfoDatas == nullptr)
		return;

	// リソース情報を持つRoot
	ImGuiRoot* resourceTree = ImGuiManager::FindRoot("Resource");
	if (resourceTree == nullptr) {
		resourceTree = ImGuiManager::AddRoot(new ImGuiRoot("Resource"));
	}

	// サウンド情報を持つRoot
	ImGuiRoot* soundTree = resourceTree->AddChild(new ImGuiRoot("Sound"));

	// ▼サウンドのリソースを管理を行う
	{
		ImGuiRoot* soundResourceTree = soundTree->AddChild(new ImGuiRoot("Resource"));

		soundResourceTree->SetChild(200.0f, 150.0f);

		for (auto& itr : *soundInfoDatas) {

			// ロードされたサウンド一つ一つに対してRootを生成する
			ImGuiRoot* item = soundResourceTree->AddChild(new ImGuiRoot(itr.first));

			std::function<void()> playFunc = std::bind(&Play, itr.first, "Debug");	// 再生する関数
			std::function<void()> stopFunc = std::bind(&Stop, itr.first, "Debug");	// 停止する関数

			item->Add(new ImGuiNode_SliderInt("Volume", &itr.second.curVolume, 0, 255));
			item->Add(new ImGuiNode_Button("Play", playFunc));
			item->Add(new ImGuiNode_Button("Stop", stopFunc));
		}
	}

	// ▼サウンドの設定を行う
	{
		ImGuiRoot* soundSettingTree = soundTree->AddChild(new ImGuiRoot("Setting"));

		// カテゴリ―別の音量倍率を設定する
		for (int i = 0; i < static_cast<int>(SoundCategory::cMax); i++) {

			// カテゴリー
			SoundCategory category = static_cast<SoundCategory>(i);

			// カテゴリー名
			std::string categoryStr = magic_enum::enum_name(category).data();
			categoryStr.erase(categoryStr.begin());

			// 音量倍率を変化させる処理
			soundSettingTree->Add(new ImGuiNode_SliderFloat(categoryStr + " Volume", &(*categoryVolumeRate)[category], 0.0f, 1.0f, "%.1f"));
		}
	}
}

#endif // IMGUI