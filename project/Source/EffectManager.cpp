#include "EffectManager.h"

#include <EffekseerForDXLib.h>
#include <unordered_map>
#include <list>

#include "../Library/csvReader.h"
#include "Util/Utils.h"
#include "settings_json.h"

namespace {

	std::unordered_map<std::string, EffectInfo>* effectInfoDatas;	// 読み込んだエフェクトデータを保持する
	std::list<EffectBase*>* effects;	// 再生中のエフェクト
	std::string* csvFilePath;			// 読み込み用Csvファイルのパス

	bool initialize = false;	// 初期化処理が行われたか
}

void EffectManager::Init() {

	if (effectInfoDatas == nullptr)
		effectInfoDatas = new std::unordered_map<std::string, EffectInfo>();

	if (effects == nullptr)
		effects = new std::list<EffectBase*>();

	if (csvFilePath == nullptr)
		csvFilePath = new std::string();

	initialize = false;
}

void EffectManager::Start() {

	if (initialize)
		return;

#ifdef IMGUI
		InitImGui();
#endif // IMGUI

	initialize = true;
}

void EffectManager::Update() {

	// 初期化処理がまだ行われていない場合
	if (initialize == false)
		Start();

	UpdateEffekseer2D();
	UpdateEffekseer3D();

	// 再生中のエフェクトがない場合
	if (effects == nullptr)
		return;

	// 再生中のエフェクトのUpdateを呼び出す
	for (auto itr = effects->begin(); itr != effects->end();) {
		if ((*itr)->IsActive() && (*itr)->IsDestroy() == false) {
			(*itr)->Update();
			itr++;
			continue;
		}

		delete* itr;
		itr = effects->erase(itr);
		if (itr == effects->end()) break;
	}
}

void EffectManager::Draw() {

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

	DrawEffekseer2D();
	DrawEffekseer3D();

	// 再生中のエフェクトがない場合、処理を抜ける
	if (effects == nullptr)
		return;

	// 再生中のエフェクトのDrawを呼び出す
	for (const auto& itr : *effects) {
		if (itr->IsActive())
			itr->Draw();
	}
}

void EffectManager::Release() {

	// 再生中のエフェクトの実体を削除する
	if (effects != nullptr) {

		for (auto itr = effects->begin(); itr != effects->end();) {
			if (*itr == nullptr) {
				itr++;
				continue;
			}

			delete* itr;
			itr = effects->erase(itr);
			if (itr == effects->end()) break;
		}

		effects->clear();
		Function::DeletePointer(effects);
	}

	// 保持していた読み込み用Csvファイルのパスを削除する
	Function::DeletePointer(csvFilePath);

	// 読み込んだ全てのエフェクトデータを解放する
	AllReleaseInfo();
}

void EffectManager::Load(const EffectInfo& info) {

	// エフェクトデータの保存が可能か
	if (effectInfoDatas == nullptr)
		return;

	// 同じデータが既に存在している場合
	if ((*effectInfoDatas).contains(info.typeName))
		return;

	// エフェクト情報
	EffectInfo newInfo = EffectInfo(info);

	// ハンドルが存在しない場合
	if (newInfo.handle < 0)
		newInfo.handle = LoadEffekseerEffect((info.fileName + info.typeName).c_str(), newInfo.defMagnification);

	// データの登録
	(*effectInfoDatas)[info.typeName] = newInfo;
}

void EffectManager::LoadToCsv(const std::string& filename) {

	// エフェクトデータの保存が可能か
	if (effectInfoDatas == nullptr)
		return;

	// Csvデータを取得する
	CsvReader* csv = new CsvReader(filename);

	// Csvデータが存在しない場合
	if (csv == nullptr)
		return;

	if (csvFilePath == nullptr)
		csvFilePath = new std::string();

	// 再読み込み用にCsvファイルのパスを保持する
	*csvFilePath = filename;

	for (int i = 0; i < csv->Column(); i++) {

		std::string beginning = csv->GetString(i, 0);	// 最初の行

		// コメントなら次の要素へ
		if (beginning.substr(0, 2) == "//")
			continue;

		// エフェクトの情報
		EffectInfo info;

		//==================================================
		// ◇データ名、ファイルパスの設定

		info.typeName = beginning;
		info.fileName = csv->GetString(i, 1);

		//==================================================
		// ◇既定拡大率

		info.defMagnification = csv->GetFloat(i, 2);
		info.magnification = info.defMagnification;

		//==================================================
		// ◇既定再生速度

		info.defPlaySpeed = csv->GetFloat(i, 3);
		info.playSpeed = info.defPlaySpeed;

		//==================================================
		// ◇読み込み処理

		Load(info);
	}

	delete csv;
}

void EffectManager::LoadFromJson(const std::string& filename)
{
	auto setting = Settings_json::Inst();
	setting->LoadSettingJson(filename, filename, true);

	EffectInfo desc = {};
	desc.magnification		= setting->GetOrDefault<float>(		 "Param.magnification",		0.0f,		filename);
	desc.defMagnification	= setting->GetOrDefault<float>(		 "Param.defMagnification",	0.0f,		filename);
	desc.playingHandle		= setting->GetOrDefault<float>(		 "Param.playSpeed",			0.0f,		filename);
	desc.defPlaySpeed		= setting->GetOrDefault<float>(		 "Param.defPlaySpeed",		0.0f,		filename);
	desc.isLoop				= setting->GetOrDefault<bool>(		 "Param.isLoop",			false,		filename);
	desc.fileName			= setting->GetOrDefault<std::string>("Param.fileName",			"NO INFO",	filename);
	desc.typeName			= setting->GetOrDefault<std::string>("Param.typeName",			"NO INFO",	filename);
	std::string dim			= setting->GetOrDefault<std::string>("Param.dimension",			"2D",		filename);
	if (dim == "2D")
	{
		desc.dimension = Dimensional::_2D;
	}
	else
	{
		desc.dimension = Dimensional::_3D;
	}

	Load(desc);
}

EffectBase* EffectManager::Play3D(const std::string& typeName, const Transform& trs, const std::string& label, const bool& isLoop) {

	// エフェクトデータや再生中エフェクトの管理が出来ていない場合
	if ((effectInfoDatas != nullptr && effects != nullptr) == false)
		return nullptr;

	// 実体を生成
	EffectBase* effect = new EffectBase();

	// 再生処理
	effect->Play3D((*effectInfoDatas)[typeName], trs, label, isLoop);

	// 再生中エフェクトのリストに追加
	effects->push_back(effect);

	return effect;
}

EffectBase* EffectManager::Play3D_Loop(const std::string& typeName, const Transform& trs, const std::string& label) {

	EffectBase* effect = IsPlaying(typeName, label);

	if (effect != nullptr)
		effect->SetTransform3D(trs);
	else
		effect = Play3D(typeName, trs, label);

	return effect;
}

EffectBase* EffectManager::Play2D(const std::string& typeName, const RectTransform& trs, const std::string& label, const bool& isLoop) {

	// エフェクトデータが存在していないもしくは、再生中エフェクトの管理が出来ていない場合
	if ((effectInfoDatas != nullptr && effects != nullptr) == false)
		return nullptr;

	// 実体を生成
	EffectBase* effect = new EffectBase();
	
	// 再生処理
	effect->Play2D((*effectInfoDatas)[typeName], trs, label, isLoop);
	
	// 再生中エフェクトのリストに追加
	effects->push_back(effect);

	return effect;
}

EffectBase* EffectManager::Play2D_Loop(const std::string& typeName, const RectTransform& trs, const std::string& label) {

	EffectBase* effect = IsPlaying(typeName, label);

	if (effect != nullptr)
		effect->SetTransform2D(trs);
	else
		effect = Play2D(typeName, trs, label);

	return effect;
}

EffectBase* EffectManager::IsPlaying(const std::string& typeName, const std::string& label) {

	// 再生中エフェクトの管理が出来てない場合
	if (effects == nullptr)
		return nullptr;

	for (const auto& itr : *effects) {
		// 引数の情報と一致しているか、整合性チェックを行う
		if (itr->CheckConsistency(typeName, label) && itr->IsActive())
			return itr;
	}

	return nullptr;
}

void EffectManager::Stop(const std::string& typeName, const std::string& label) {

	// 再生中か
	EffectBase* effect = IsPlaying(typeName, label);
	if (effect == nullptr)
		return;

	effect->Stop();
}

void EffectManager::StopAll() {

	// 再生中エフェクトの管理が出来てない場合
	if (effects == nullptr)
		return;

	for (const auto& itr : *effects)
		itr->Stop();
}

void EffectManager::Reload() {

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

void EffectManager::AllReleaseInfo() {

	// エフェクトデータが存在していない場合
	if (effectInfoDatas == nullptr)
		return;

	for (auto& itr : *effectInfoDatas) {
		itr.second.playingHandle = -1;
		DeleteEffekseerEffect(itr.second.handle);
	}

	Function::DeletePointer(effectInfoDatas);
}
#ifdef IMGUI

#include "ImGuiTree/imGuiManager.h"

void EffectManager::InitImGui() {

	if (effectInfoDatas == nullptr)
		return;

	// リソース情報を持つRoot
	ImGuiRoot* resourceTree = ImGuiManager::FindRoot("Resource");
	if (resourceTree == nullptr) {
		resourceTree = ImGuiManager::AddRoot(new ImGuiRoot("Resource"));
	}

	// エフェクト情報を持つRoot
	ImGuiRoot* effectTree = resourceTree->AddChild(new ImGuiRoot("Effect"));
	effectTree->SetChild(200.0f, 150.0f);

	for (const auto& itr : *effectInfoDatas) {

		// ロードされたエフェクト一つ一つに対してRootを生成する
		ImGuiRoot* item = effectTree->AddChild(new ImGuiRoot(itr.first));

		// 再生する関数
		std::function<void()> playFunc = std::bind(&Play2D, itr.first, RectTransform(Anchor::Preset::Middle), "Debug", false);
		
		// 停止する関数
		std::function<void()> stopFunc = std::bind(&Stop, itr.first, "Debug");

		item->Add(new ImGuiNode_Button("Play", playFunc));
		item->Add(new ImGuiNode_Button("Stop", stopFunc));
	}
}

#endif // IMGUI