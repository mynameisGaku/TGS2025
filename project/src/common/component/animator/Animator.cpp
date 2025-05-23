#include "Animator.h"

// ◇汎用
#include "src/util/time/GameTime.h"
#include "src/util/file/resource_loader/resourceLoader.h"
#include <vendor/magic_enum/magic_enum.hpp>
#include "src/util/object3D/Object3D.h"
#include "src/util/file/json/settings_json.h"
#include <assert.h>
#include <src/util/ptr/PtrUtil.h>

//ToDo:外部化
namespace {
	static const int BLEND_COUNT_MAX = 5;
};

Animator::Animator() {
	current = nullptr;

	parentModel = -1;
	mergeTime = 0.0f;
	mergeTimeMax = 1.0f;
	playSpeed = 1.0f;
	frameRate = 1.0f;

	origin = "";
	playingLabel = "";

	offsetMatrix = MGetIdent();
}

Animator::~Animator() {

	// アニメーションをデタッチする
	if (current != nullptr)
		delete current;
	for (auto item : currentSubs) {
		AttachedAnimation_Sub* currentSub = item.second;
		if (currentSub != nullptr) {
			delete currentSub;
		}
	}
	for (AttachedAnimation* prev : prevs) {
		if (prev != nullptr) {
			delete prev;
		}
	}

	DeleteAnimInfos();
}

void Animator::Init(std::string _origin, float _frameRate, float _mergeTimeMax) {

	Object3D* parentObj = dynamic_cast<Object3D*>(parent);

	// 親のモデルを設定
	if (parentObj != nullptr)
		parentModel = parentObj->Model();

	// モデルの原点の名前
	origin = _origin;

	// アニメーションの経過速度
	frameRate = _frameRate;

	// 何秒で補完しきるか
	mergeTimeMax = _mergeTimeMax;
}

void Animator::Update() {
	if (current == nullptr) return;

	// ルートフレーム取得
	int hRoot = MV1SearchFrame(parentModel, origin.c_str());

	// 一旦リセット
	MV1ResetFrameUserLocalMatrix(parentModel, hRoot);

	// 時間に応じてブレンド率を変える
	float rate = 1.0f;

	// ブレンド進行処理
	if (prevs.size() > 0) {
		// ブレンド時間を進める
		mergeTime += GTime.deltaTime * playSpeed;
		const float animMergeTimeMax = GetAnimInfo().mergeTimeMax;

		// ブレンド終了時なら
		if (mergeTime >= animMergeTimeMax) {

			// 前アニメーション終了処理
			for (AttachedAnimation* prev : prevs) {
				delete prev;
				prev = nullptr;
			}
			prevs.clear();

			mergeTime = animMergeTimeMax;
		}

		// 時間に応じてブレンド率を変える
		if (animMergeTimeMax > 0)
			rate = mergeTime / animMergeTimeMax;
		else
			rate = 1.0f;
		// 全体が1になるようにブレンド
		current->SetBlendRate(rate);

		if (prevs.size() > 0) {
			float prevRate = 1.0f - rate;
			for (AttachedAnimation* prev : prevs) {
				prev->SetBlendRate(prevRate);
			}
		}
	}

	// ルートの移動行列
	MATRIX currentM = MGetIdent();
	MATRIX prevM = MGetScale(Vector3::Zero);

	// アニメーションの更新
	for (AttachedAnimation_Main* prev : prevs) {
		prev->Update();
	}
	current->Update();

	// ルート補正の計算
	for (AttachedAnimation_Main* prev : prevs) {
		prev->UpdateRootMatrix();
	}
	current->UpdateRootMatrix();

	// ルートの移動行列を取得
	currentM = MScale(current->RootMatrix(), current->BlendRate());
	for (AttachedAnimation_Main* prev : prevs) {
		prevM += MScale(prev->RootMatrix(), prev->BlendRate());
	}

	// 現姿勢と前姿勢を合成
	currentM += prevM;

	// オフセットを適用
	currentM *= offsetMatrix;

	// セット
	MV1SetFrameUserLocalMatrix(parentModel, hRoot, currentM);

#if TRUE

	// 時間に応じてブレンド率を変える
	float rateSub = 1.0f;

	// ブレンド進行処理
	for (auto& item : prevSubs) {
		const std::string& frameName = item.first;
		std::list<AttachedAnimation_Sub*>& prevSubList = item.second;
		float& mergeTimeSub = mergeTimeSubs.at(frameName);
		AttachedAnimation_Sub* currentSub = currentSubs.at(frameName);

		if (not prevSubList.empty()) {
			// ブレンド時間を進める
			mergeTimeSub += GTime.deltaTime * playSpeed;
			const float animMergeTimeMax = currentSub->Info().mergeTimeMax;

			// ブレンド終了時なら
			if (mergeTimeSub >= animMergeTimeMax) {

				// 前アニメーション終了処理
				for (AttachedAnimation_Sub* prevSub : prevSubList) {
					delete prevSub;
					prevSub = nullptr;
				}
				prevSubList.clear();

				mergeTimeSub = animMergeTimeMax;
			}

			// 時間に応じてブレンド率を変える
			if (animMergeTimeMax > 0)
				rateSub = mergeTimeSub / animMergeTimeMax;
			else
				rateSub = 1.0f;
			// 全体が1になるようにブレンド
			currentSub->SetBlendRate(rateSub);

			float prevRate = 1.0f - rateSub;
			for (AttachedAnimation_Sub* prevSub : prevSubList) {
				prevSub->SetBlendRate(prevRate);
			}
		}

	}

#endif 

	// サブアニメーション更新
	for (auto& item : currentSubs)
	{
		std::string frameName = item.first;
		AttachedAnimation_Sub* currentSub = item.second;

		currentSub->UpdateBrendRate(current->AttachID());

		currentSub->Update();
	}
	// サブアニメーション更新
	for (auto& item : prevSubs)
	{
		std::string frameName = item.first;
		std::list<AttachedAnimation_Sub*> prevSubList = item.second;
		for (AttachedAnimation_Sub* prevSub : prevSubList)
		{
			//prevSub->UpdateBrendRate(current->AttachID());

			prevSub->Update();
		}
	}

	for (auto& item : frameMatrix)
	{
		int frame = MV1SearchFrame(parentModel, item.first.c_str());
		MV1ResetFrameUserLocalMatrix(parentModel, frame);

		MATRIX frameM = MV1GetFrameLocalMatrix(parentModel, frame);
		frameM = item.second * frameM;

		MV1SetFrameUserLocalMatrix(parentModel, frame, frameM);
	}
}

void Animator::LoadAnim(std::string folder, std::string name, AnimOption option) {
	std::string fullPath = folder + name;
	// 拡張子を補完
	if (name.find(".mv1") == std::string::npos)
	{
		fullPath += ".mv1";
	}
	anims[name].handle = ResourceLoader::MV1LoadModel(fullPath.c_str());
	assert(anims[name].handle >= 0);

	anims[name].animName = name;
	anims[name].option = option;

	anims[name].startFrame = 0.0;
	anims[name].endFrame = MV1GetAnimTotalTime(anims[name].handle, 0);
}

void Animator::LoadAnimsFromJson(std::string path) {
	// 拡張子を補完
	if (path.find(".json") == std::string::npos) {
		path += ".json";
	}

	std::string key = path;

	// JSON 読み込み
	auto jsonLoader = Settings_json::Inst();
	jsonLoader->LoadSettingJson(path, key);

	std::string folder = jsonLoader->GetOrDefault<std::string>("Folder", "", key);

	std::unordered_map<std::string, nlohmann::json> files;
	files = jsonLoader->GetOrDefault<std::unordered_map<std::string, nlohmann::json>>("Files", files, key);

	for (auto itr = files.begin(); itr != files.end(); itr++) {
		std::string name = (*itr).first;
		AnimOption option;

		option.defaultAnimSpeed = jsonLoader->GetOrDefault<float>("Files." + name + ".DefaultAnimSpeed", 1.0f, key);
		option.isFixedRoot[0] = jsonLoader->GetOrDefault<bool>("Files." + name + ".IsFixedRoot.X", false, key);
		option.isFixedRoot[1] = jsonLoader->GetOrDefault<bool>("Files." + name + ".IsFixedRoot.Y", false, key);
		option.isFixedRoot[2] = jsonLoader->GetOrDefault<bool>("Files." + name + ".IsFixedRoot.Z", false, key);
		option.isLoop = jsonLoader->GetOrDefault<bool>("Files." + name + ".IsLoop", false, key);
		option.offset.x = jsonLoader->GetOrDefault<float>("Files." + name + ".Offset.X", 0.0f, key);
		option.offset.y = jsonLoader->GetOrDefault<float>("Files." + name + ".Offset.Y", 0.0f, key);
		option.offset.z = jsonLoader->GetOrDefault<float>("Files." + name + ".Offset.Z", 0.0f, key);

		LoadAnim(folder, name, option);

		anims.at(name).startFrame = jsonLoader->GetOrDefault<float>("Files." + name + ".StartFrame", anims.at(name).startFrame, key);
		anims.at(name).endFrame = jsonLoader->GetOrDefault<float>("Files." + name + ".EndFrame", anims.at(name).endFrame, key);
		anims.at(name).mergeTimeMax = jsonLoader->GetOrDefault<float>("Files." + name + ".MergeTime", mergeTimeMax, key);
	}
}

void Animator::Play(std::string label, float speed) {
	if (label == playingLabel) // 同じアニメーション名なら無視する
		return;
	if (not anims.contains(label))	// 存在しないアニメーション名なら無視する
		return;

	AnimInfo anim = anims.at(label);

	// 追加前に消すのか？
	bool over = false;
	float rate = 0.0f;
	if (prevs.size() >= BLEND_COUNT_MAX) {
		rate = prevs.front()->BlendRate();
		delete prevs.front();
		prevs.pop_front();
		over = true;
	}

	if (current != nullptr) {
		prevs.push_back(current);
		mergeTime = 0.0f;
	}

	for (AttachedAnimation* prev : prevs) {
		prev->RefreshDefaultBlendRate();
	}

	if (over)
	{
		for (AttachedAnimation* prev : prevs) {
			prev->SetDefaultBlendRate(prev->DefaultBlendRate() + rate / prevs.size());
		}
	}

	playingLabel = label;

	current = new AttachedAnimation_Main(parentModel, anim);
	current->SetPlaySpeed(playSpeed);

	if (prevs.empty())
	{
		mergeTime = anim.mergeTimeMax;
		current->SetBlendRate(1.0f);
	}

#if FALSE
	// 再生開始地点を設定する
	if (anim.startFrame < 0) {
		current->SetFrame(0.0f);
	}
	else {
		current->SetFrame(anim.startFrame);
	}

	// 再生終了地点を設定する
	if (anim.endFrame <= 0)
		current.maxFrame = MV1GetAttachAnimTotalTime(parentModel, current.attachID);
	else
		current.maxFrame = anim.endFrame;
#endif // FALSE
}

void Animator::PlaySub(std::string frameName, std::string label, float speed) {
	if (not anims.contains(label))	// 存在しないアニメーション名なら無視する
		return;

	// 同じアニメーションなら無視する
	if (currentSubs.contains(frameName)) {
		if (currentSubs.at(frameName)->Info().animName == label)
			return;
	}

	AnimInfo anim = anims.at(label);

	// 現在のサブアニメーションをブレンド元にする
	if (currentSubs.contains(frameName)) {

		AttachedAnimation_Sub* prevSub = currentSubs.at(frameName);
		currentSubs.erase(frameName);
		prevSubs.at(frameName).push_back(prevSub);

		for (AttachedAnimation_Sub* prevSub : prevSubs[frameName]) {
			prevSub->RefreshDefaultBlendRate();
		}
	}
	// 現在のメインアニメーションをサブアニメーションにしてブレンド元にする
	else if (current != nullptr)	{
		AttachedAnimation_Sub* prevSub = new AttachedAnimation_Sub(parentModel, anims.at(playingLabel), frameName);
		if (not prevSubs.contains(frameName)) {
			prevSubs.emplace(frameName, std::list<AttachedAnimation_Sub*>());
		}
		prevSub->SetBlendRate(1.0f);

		prevSubs.at(frameName).push_back(prevSub);
	}

	AttachedAnimation_Sub* newSub = new AttachedAnimation_Sub(parentModel, anim, frameName);

	newSub->SetPlaySpeed(playSpeed);
	currentSubs.emplace(frameName, newSub);

	if (prevSubs.empty()) {
		mergeTimeSubs[frameName] = anim.mergeTimeMax;
		newSub->SetBlendRate(1.0f);
	}
	else {
		mergeTimeSubs[frameName] = 0.0f;
		newSub->SetBlendRate(0.0f);
	}
}

void Animator::StopSub(std::string frameName)
{
	// ブレンドを使う
	if (not currentSubs.contains(frameName)) return;

	PlaySub(frameName, anims.at(playingLabel).animName, playSpeed);
}

void Animator::DeleteAnimInfos() {
	// アニメーション情報を解放する
	for (auto& anim : anims) {
		for (auto& event : anim.second.event) {
			PtrUtil::SafeDelete(event);
		}
		anim.second.event.clear();
		MV1DeleteModel(anim.second.handle);
	}

	anims.clear();
}

void Animator::SetPlaySpeed(float speed) {
	playSpeed = speed;
	current->SetPlaySpeed(playSpeed);
}
