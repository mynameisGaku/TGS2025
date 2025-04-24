#include "Animator.h"

// ◇汎用
#include "../../Library/time.h"
#include "../../Library/resourceLoader.h"
#include "../../Library/magic_enum.hpp"
#include "../Util/Utils.h"
#include "../Object3D.h"
#include <assert.h>

Animator::Animator() {

	parentObj = nullptr;
	animCsv = nullptr;

	current.attachID = -1;
	prev.attachID = -1;

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

	if (current.attachID >= 1.0f)
		MV1DetachAnim(parentModel, current.attachID);

	if (prev.attachID >= 1.0f)
		MV1DetachAnim(parentModel, prev.attachID);

	// Csvデータを削除する
	Function::DeletePointer(animCsv);

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

	parentObj = dynamic_cast<Object3D*>(parent);

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

void Animator::LoadAnim(std::string animFilePath, std::string label, bool loop, bool fixedRoot, std::string extension) {

	anims[label].handle = ResourceLoader::MV1LoadModel((animFilePath + extension).c_str());
	assert(anims[label].handle >= 0);

	anims[label].animName = label;
	anims[label].isLoop = loop;
	anims[label].isFixedRoot = fixedRoot;
}

void Animator::LoadAnimCsv(std::string csvFilePath, std::string animFilePath) {

	if (csvFilePath == "")
		return;

	Function::DeletePointer(animCsv);

	animCsv = new CsvReader(csvFilePath);

	for (int i = 1; i < animCsv->Column(); i++) {

		std::string labelName = animCsv->GetString(i, LabelName);	// ラベル名

		// ラベル名が設定されていないなら、次の要素へ
		if (labelName == "")
			continue;

		std::string fileName = animFilePath + animCsv->GetString(i, ResourceName);	// データが実在する場所		
		bool loop = animCsv->GetBool(i, IsLoop);			// ループ再生の有無
		bool fixedRoot = animCsv->GetBool(i, IsFixedRoot);	// ローカル座標の固定化の有無

		// データの読み込み
		LoadAnim(fileName, labelName, loop, fixedRoot);

		anims[labelName].startFrame		= animCsv->GetFloat(i, StartFrame);
		anims[labelName].endFrame		= animCsv->GetFloat(i, EndFrame);
		anims[labelName].defAnimSpeed	= animCsv->GetFloat(i, DefAnimSpeed);

		// コマンドが設定されていないなら、次の要素へ
		if (animCsv->GetString(i, Command) == "")
			continue;

		// 各コマンドを取得する
		for (int j = i; j < animCsv->Column(); j++) {

			std::string nextLabelName = animCsv->GetString(j, LabelName);	// 次の列のラベル名

			// 次のラベル名が今のラベル名と一致していないかつ、
			// 次のラベル名に名前が入っていたらforから抜ける
			if (nextLabelName != labelName && nextLabelName != "")
				break;

			AnimationEvent* event = new AnimationEvent(this);

			std::string  commandType = animCsv->GetString(j, Command);		// コマンド
			std::string commandContents = animCsv->GetString(j, Details);	// 実行内容
			float runFrame = animCsv->GetFloat(j, RunFrame);				// 実行フレーム

			event->SetCommand(commandType, commandContents, runFrame);

			anims[labelName].event.push_back(event);
		}
	}
}

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

	// ◇Csvデータが読み込まれていたら
	if (animCsv != nullptr) {

		for (auto& event : anims[playingLabel].event) {
			event->Update();
		}
	}
}

void Animator::Play(std::string label, float speed)
{
	if (label == playingLabel) // 同じIDなら無視する
		return;

	for (const auto& anim : anims) {
		// 引数のラベル名と一致していない場合、次の要素へ
		if (anim.first != label)
			continue;

		// 前回のアニメーションを切り離す
		if (prev.attachID >= 0)
			MV1DetachAnim(parentModel, prev.attachID);

		playingLabel = label;
		prev = current;
		mergeTime = 0.0f;

		// 再生速度を設定する
		if (speed < 0)
			playSpeed = anims[label].defAnimSpeed;
		else
			playSpeed = speed;

		// 再生開始地点を設定する
		if (anims[label].startFrame < 0) {
			current.nowFrame	= 0.0f;
			current.beforeFrame = 0.0f;
		}
		else {
			current.nowFrame	= anims[label].startFrame;
			current.beforeFrame = anims[label].startFrame;
		}

		// コマンドの実行権限を復活させる
		for (int i = 0; i < anims[playingLabel].event.size(); i++)
			anims[playingLabel].event[i]->SetUseCommand(true);

		// アニメーションを適応
		current.attachID = MV1AttachAnim(parentModel, 0, anim.second.handle);
		current.attachName = label;

		// 再生終了地点を設定する
		if (anims[label].endFrame <= 0)
			current.maxFrame = MV1GetAttachAnimTotalTime(parentModel, current.attachID);
		else
			current.maxFrame = anims[label].endFrame;

		break;
	}
}
