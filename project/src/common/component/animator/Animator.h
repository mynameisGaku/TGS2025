#pragma once

// ◇継承元
#include "Component.h"

// ◇汎用
#include <unordered_map>
#include <assert.h>

// ◇個別に必要なもの
#include "AnimationDefine.h"
#include "AttachedAnimation.h"

class Object3D;

/// <summary>
/// アニメーションの再生を行うコンポーネント
/// </summary>
/// <author>ミッチ、佐藤紘斗</author>
class Animator : public Component {
public:
	//==========================================================================================
	// ▼構造体

	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	Animator();
	~Animator();

	//==========================================================================================
	// ▼各種関数

	/// <summary>
	/// 初期化を行う
	/// </summary>
	/// <param name="origin">モデルの原点の名前</param>
	/// <param name="frameRate">フレームの経過速度(1秒間で何フレーム進むか)</param>
	/// <param name="mergeTimeMax">何秒で補完しきるか</param>
	void Init(std::string origin, float frameRate, float mergeTimeMax);

	void Update() override;
	void Draw() override {};

	/// <summary>
	/// アニメーションをロード
	/// </summary>
	/// <param name="folder">アニメーションがあるフォルダのパス</param>
	/// <param name="name">アニメーション名</param>
	/// <param name="option">設定項目</param>
	void LoadAnim(std::string folder, std::string name, AnimOption option);

	/// <summary>
	/// アニメーションをJsonを元に一括ロード
	/// </summary>
	/// <param name="path">Jsonファイルのパス</param>
	void LoadAnimsFromJson(std::string path);

	/// <summary>
	/// アニメーションを再生する
	/// 同じアニメーションの場合は、無視する
	/// </summary>
	/// <param name="label">アニメーションのラベル名</param>
	/// <param name="speed">再生速度</param>
	void Play(std::string label, float speed = -1.0f);

	void PlaySub(std::string frameName, std::string label, float speed = -1.0f);
	void StopSub(std::string frameName);

	void DeleteAnimInfos();

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// アニメーションの再生速度（倍率）を変更する
	/// 標準速度は1.0f
	/// </summary>
	/// <param name="speed">再生速度</param>
	void SetPlaySpeed(float speed);

	/// <summary>
	/// 今のアニメーションの進行度をセット
	/// </summary>
	/// <param name="ratio">進行度（0..1）</param>
	inline void SetCurrentRatio(float ratio) { current->SetFrameByRatio(ratio); }

	/// <summary>
	/// 現在のアニメーションのフレームを設定する
	/// </summary>
	/// <param name="frame">アニメーションのフレーム</param>
	inline void SetCurrentFrame(float frame) { current->SetFrame(frame); }

	/// <summary>
	/// 指定フレームに行列をセット
	/// </summary>
	/// <param name="frameName">対象のフレームの名前</param>
	/// <param name="matrix"></param>
	void SetFrameMatrix(std::string frameName, const MATRIX& matrix) {
		assert(MV1SearchFrame(parentModel, frameName.c_str()) >= 0);
		frameMatrix[frameName] = matrix;
	}

	void SetOffsetMatrix(const MATRIX& matrix) {
		offsetMatrix = matrix;
	}

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// ループしないアニメーションが最後まで再生したか調べる
	/// </summary>
	/// <returns>最後にtrue</returns>
	inline bool IsFinished() {

		if (anims[playingLabel].option.isLoop)
			return false;

		return (current->Frame() >= current->MaxFrame());
	};

	/// <summary>
	/// ループしないアニメーションが最後まで再生したか調べる
	/// </summary>
	/// <returns>最後にtrue</returns>
	inline bool IsFinishedSub(std::string frame) {
		if (not currentSubs.contains(frame))
			return false;

		AttachedAnimation_Sub* currentSub = currentSubs.at(frame);

		if (currentSub->Info().option.isLoop)
			return false;

		return (currentSub->Frame() >= currentSub->MaxFrame());
	};

	inline const std::unordered_map<std::string, AnimInfo> GetAllAnimInfo() const { return anims; }

	inline AnimInfo GetAnimInfo() const {
		return GetAnimInfo(playingLabel);
	}

	inline AnimInfo GetAnimInfo(std::string type) const {

		if (anims.contains(type) == false)
			return AnimInfo();

		return anims.at(type);
	}

	/// <summary>
	/// 今のアニメーションの進行度をフレームで返す
	/// </summary>
	/// <returns>現在のフレーム</returns>
	inline float CurrentFrame() const { return current->Frame(); }

	/// <summary>
	/// 今のアニメーションの進行度を0..1で返す
	/// </summary>
	/// <returns>現在の進行度</returns>
	inline float CurrentRatio() const { return current->FrameByRatio(); }

	/// <summary>
	/// 現在の再生速度を取得する
	/// </summary>
	inline float CurrentPlaySpeed() const { return playSpeed; }

	/// <summary>
	/// ブレンドの経過速度を取得する
	/// </summary>
	inline float MargeTime() const { return mergeTime; }

private:
	//==========================================================================================
	// ▼メンバ変数
	std::unordered_map<std::string, AnimInfo> anims;	// アニメーションの情報

	std::list<AttachedAnimation_Main*> prevs;
	AttachedAnimation_Main* current;

	std::unordered_map<std::string, AttachedAnimation_Sub*> currentSubs;

	int parentModel;	// 親のモデル
	float mergeTime;	// ブレンドの経過速度(1秒間で何フレーム進むか)
	float mergeTimeMax;	// 何秒で補完しきるか
	float playSpeed;	// アニメーションの再生速度
	float frameRate;	// アニメーションのフレーム経過速度

	std::string origin;	// モデルの原点の名前
	std::string playingLabel;	// 再生中のアニメーションの名札

	std::unordered_map<std::string, MATRIX> frameMatrix;
	MATRIX offsetMatrix;
};