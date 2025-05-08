#include "Animator.h"

// ◇汎用
#include "../../Library/time.h"
#include "../../Library/resourceLoader.h"
#include "../../Library/magic_enum.hpp"
#include "../Util/Utils.h"
#include "../Object3D.h"
#include <assert.h>

namespace {
	static const float MERGE_TIME_MAX = 0.25f;
	static const int ANIM_FRAMERATE = 60;
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
}

Animator::~Animator() {

	// アニメーションをデタッチする
	if (current != nullptr)
		delete current;
	for (AttachedAnimation* prev : prevs) {
		if (prev != nullptr) {
			delete prev;
		}
	}

	// コマンドの実行権限を復活させる
	for (auto& anim : anims) {
		for (auto& event : anim.second.event) {
			Function::DeletePointer(event);
		}
		anim.second.event.clear();
	}

	// アニメーション情報を解放する
	anims.clear();
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

#if FALSE
void Animator::Update() {

	// ①前姿勢を補正
	// ②後姿勢を補正
	// ③前姿勢と後姿勢を割合でブレンド

	int hRoot = MV1SearchFrame(parentModel, origin.c_str());

	// 一旦リセット
	MV1ResetFrameUserLocalMatrix(parentModel, hRoot);

	MATRIX currentM = MGetIdent();	// 現在のアニメーションの行列
	MATRIX prevM = MGetIdent();		// 前回のアニメーションの行列

	// ◇前回のアニメーションが再生中なら
	if (prev.attachID >= 0) {
		// ▽補間処理
		{
			mergeTime += Time::DeltaTimeLapseRate() * playSpeed;

			if (mergeTime >= mergeTimeMax) {
				MV1DetachAnim(parentModel, prev.attachID);
				MV1SetAttachAnimBlendRate(parentModel, current.attachID, 1.0f);
				prev.attachID = -1;
			}
			else {
				float rate = mergeTime / mergeTimeMax;
				MV1SetAttachAnimBlendRate(parentModel, prev.attachID, 1.0f - rate);
				MV1SetAttachAnimBlendRate(parentModel, current.attachID, rate);
			}
		}

		// 前回の行列を設定
		prevM = MV1GetFrameLocalMatrix(parentModel, hRoot);

		// 無補正時の座標を取得
		const Vector3 framePos = MV1GetAttachAnimFrameLocalPosition(parentModel, prev.attachID, hRoot);

		// 座標移動を打ち消す
		prevM *= MGetTranslate(framePos * -1.0f);

		// Yだけ維持、XZを原点
		prevM *= MGetTranslate(Vector3(0.0f, framePos.y, 0.0f));
	}

	// ◇アニメーションが再生中なら
	if (current.attachID >= 0) {

		current.beforeFrame = current.nowFrame;
		current.nowFrame += frameRate * Time::DeltaTimeLapseRate() * playSpeed;

		// アニメーションが総再生フレームまで再生したら
		if (current.nowFrame >= current.maxFrame) {

			if (!anims[playingLabel].isLoop)
				current.nowFrame = current.maxFrame;
			else {
				current.nowFrame -= current.maxFrame;

				// コマンドの実行権限を復活させる
				for (int i = 0; i < anims[playingLabel].event.size(); i++)
					anims[playingLabel].event[i]->SetUseCommand(true);
			}
		}

		// アニメーションを適応させる
		MV1SetAttachAnimTime(parentModel, current.attachID, current.nowFrame);

		// 現在の行列を取得
		currentM = MV1GetFrameLocalMatrix(parentModel, hRoot);


		// ◇ローカル座標の固定化が有効なら
		if (anims[playingLabel].isFixedRoot)
		{
			// 無補正時の座標を取得
			const Vector3 framePos = MV1GetAttachAnimFrameLocalPosition(parentModel, current.attachID, hRoot);

			// 座標移動を打ち消す
			currentM *= MGetTranslate(framePos * -1.0f);

			// Yだけ維持、XZを原点
			currentM *= MGetTranslate(Vector3(0.0f, framePos.y, 0.0f));
		}

		// ◇前回のアニメーションが再生中なら、ブレンドする
		if (prev.attachID >= 0)
		{
			// root姿勢を滑らかに遷移
			float progress = mergeTime / mergeTimeMax;

			// 現姿勢と前姿勢を合成
			// 最低値 + (最大値 - 最低値) * progress
			currentM = MAdd(prevM, MAdd(currentM, prevM * MGetScale(V3::ONE * -1.0f)) * MGetScale(V3::ONE * progress));
		}

		// テスト
		currentM *= offsetMatrix;

		// セット
		MV1SetFrameUserLocalMatrix(parentModel, hRoot, currentM);
	}


#if FALSE
	// ◇Csvデータが読み込まれていたら
	if (animCsv != nullptr) {

		for (auto& event : anims[playingLabel].event) {
			event->Update();
		}
	}
#endif // FALSE
}

#endif // FALSE

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
		mergeTime += Time::DeltaTime() * playSpeed;

		// ブレンド終了時なら
		if (mergeTime >= MERGE_TIME_MAX) {

			// 前アニメーション終了処理
			for (AttachedAnimation* prev : prevs) {
				delete prev;
				prev = nullptr;
			}
			prevs.clear();

			mergeTime = MERGE_TIME_MAX;
		}

		// 時間に応じてブレンド率を変える
		rate = mergeTime / MERGE_TIME_MAX;
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
	MATRIX prevM = MGetScale(V3::ZERO);

	// アニメーションの更新
	for (AttachedAnimation* prev : prevs) {
		prev->Update();
	}
	current->Update();

	// ルート補正の計算
	for (AttachedAnimation* prev : prevs) {
		prev->UpdateRootMatrix();
	}
	current->UpdateRootMatrix();

	// ルートの移動行列を取得
	currentM = MScale(current->RootMatrix(), current->BlendRate());
	for (AttachedAnimation* prev : prevs) {
		prevM += MScale(prev->RootMatrix(), prev->BlendRate());
	}

	// 現姿勢と前姿勢を合成
	currentM += prevM;

	// セット
	MV1SetFrameUserLocalMatrix(parentModel, hRoot, currentM);

	for (auto& item : frameMatrix)
	{
		int frame = MV1SearchFrame(parentModel, item.first.c_str());
		//MV1ResetFrameUserLocalMatrix(parentModel, frame);

		MATRIX frameM = MV1GetFrameLocalMatrix(parentModel, frame);
		frameM = item.second * frameM;

		MV1SetFrameUserLocalMatrix(parentModel, frame, frameM);
	}
}

void Animator::LoadAnim(std::string folder, std::string name, AnimOption option) {
	std::string fullPath = folder + name;
	if (name.find(".mv1") == std::string::npos)
	{
		fullPath += ".mv1";
	}
	anims[name].handle = ResourceLoader::MV1LoadModel(fullPath.c_str());
	assert(anims[name].handle >= 0);

	anims[name].animName = name;
	anims[name].option = option;

	// ToDo:start,endをどうしよう
	anims[name].startFrame = 0.0;
	anims[name].endFrame = MV1GetAnimTotalTime(anims[name].handle, 0);
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

	current = new AttachedAnimation(parentModel, anim);
	current->SetPlaySpeed(playSpeed);

	if (prevs.empty())
	{
		mergeTime = 1.0f;
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
