#pragma once

// ◇継承元
#include "Component.h"

// ◇汎用
#include "../../Library/myDxLib.h"
#include "../../Library/csvReader.h"
#include <vector>
#include <string>
#include <unordered_map>

// ◇個別で必要な物
#include "AnimationEvent.h"

class Object3D;

/// <summary>
/// アニメーションの再生を行うコンポーネント
/// </summary>
class Animator : public Component {
public:
	//==========================================================================================
	// ▼構造体

	/// <summary>
	/// アニメーションが持つ情報
	/// </summary>
	struct AnimInfo {
		int handle;			// アニメーションのハンドル
		bool isLoop;		// ループ再生の有無
		bool isFixedRoot;	// ローカル座標の固定化の有無
		float startFrame;	// 再生開始フレーム
		float endFrame;		// 再生終了フレーム
		float defAnimSpeed;	// 既定再生速度
		std::vector<AnimationEvent*> event;	// アニメーションのコマンド
		std::string animName;

		AnimInfo() : 
			handle(-1),
			isLoop(false),
			isFixedRoot(false),
			startFrame(0.0f),
			endFrame(0.0f),
			defAnimSpeed(1.0f),
			animName("")
		{
		}
	};

	/// <summary>
	/// アタッチ(再生)しているアニメーションの情報 
	/// </summary>
	struct AttachInfo {
		int attachID;		// アタッチID
		std::string attachName;
		float nowFrame;		// 現在の再生フレーム
		float beforeFrame;	// 1フレーム前の再生フレーム
		float maxFrame;		// 総再生フレーム

		AttachInfo() :
			attachID(0),
			attachName(""),
			nowFrame(0.0f),
			beforeFrame(0.0f),
			maxFrame(0.0f)
		{
		}
	};

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

	// 常に掛ける行列をセット
	void SetOffsetMatrix(const MATRIX& matrix) { offsetMatrix = matrix; }

	void Update() override;
	void Draw() override {};

	/// <summary>
	/// アニメーションファイルをロードする
	/// </summary>
	/// <param name="csvFilePath">CSVデータがあるファイル名</param>
	/// <param name="animFilePath">アニメーションデータがあるファイル名</param>
	void LoadAnimCsv(std::string csvFilePath, std::string animFilePath);

	/// <summary>
	/// アニメーションファイルをロードする
	/// </summary>
	/// <param name="filename">アニメーションデータがあるファイル名</param>
	/// <param name="label">アニメーションのラベル名</param>
	/// <param name="loop">ループアニメーションの場合はtrue</param>
	/// <param name="fixedRoot">rootをその場固定する場合はtrue</param>
	/// <param name="extension">拡張子</param>
	void LoadAnim(std::string animFilePath, std::string label, bool loop = false, bool fixedRoot = false, std::string extension = ".mv1");

	/// <summary>
	/// アニメーションを再生する
	/// 同じアニメーションの場合は、無視する
	/// </summary>
	/// <param name="label">アニメーションのラベル名</param>
	/// <param name="speed">再生速度</param>
	void Play(std::string label, float speed = -1.0f);

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// アニメーションの再生速度（倍率）を変更する
	/// 標準速度は1.0f
	/// </summary>
	/// <param name="speed">再生速度</param>
	inline void SetPlaySpeed(float speed) { playSpeed = speed; }

	/// <summary>
	/// 今のアニメーションの進行度をセット
	/// </summary>
	/// <param name="ratio">進行度（0..1）</param>
	inline void SetCurrentRatio(float ratio) { current.nowFrame = current.maxFrame * max(min(ratio, 1.0f), 0.0f); }

	/// <summary>
	/// 現在のアニメーションのフレームを設定する
	/// </summary>
	/// <param name="frame">アニメーションのフレーム</param>
	inline void SetCurrentFrame(float frame) { current.nowFrame = max(min(frame, current.maxFrame), 0.0f); }

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// ループしないアニメーションが最後まで再生したか調べる
	/// </summary>
	/// <returns>最後にtrue</returns>
	inline bool IsFinished() {

		if (anims[playingLabel].isLoop)
			return false;

		return (current.nowFrame >= current.maxFrame);
	};

	inline const std::unordered_map<std::string, AnimInfo> GetAllAnimInfo() const { return anims; }

	inline AnimInfo GetAnimInfo(std::string type) {

		if (anims.contains(type) == false)
			return AnimInfo();

		return anims[type];
	}

	/// <summary>
	/// 今のアニメーションの情報を取得する
	/// </summary>
	inline const AttachInfo& CurrentAttach() const { return current; }

	/// <summary>
	/// 今のアニメーションの進行度を0..1で返す
	/// </summary>
	/// <returns>今の進行度</returns>
	inline float CurrentRatio() const { return current.nowFrame / current.maxFrame; }

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

	enum AnimInfoElement {
		None = -1,
		MotionDescription,	// モーションの説明
		ResourceName,		// リソース名
		LabelName,			// ラベル名
		IsLoop,				// ループ再生の有無
		IsFixedRoot,		// 座標固定化の有無
		StartFrame,			// 再生開始フレーム
		EndFrame,			// 再生終了フレーム
		DefAnimSpeed,		// 既定再生速度
		Command,			// コマンド
		Details,			// 実行内容
		RunFrame,			// 実行フレーム
		Max
	};

	Object3D* parentObj;
	CsvReader* animCsv;	// アニメーションのCSVデータ
	std::unordered_map<std::string, AnimInfo> anims;	// アニメーションの情報

	AttachInfo current;	// 現在アタッチ中のアニメーション情報
	AttachInfo prev;	// 一つ前にアタッチしていたアニメーション情報

	int parentModel;	// 親のモデル
	float mergeTime;	// ブレンドの経過速度(1秒間で何フレーム進むか)
	float mergeTimeMax;	// 何秒で補完しきるか
	float playSpeed;	// アニメーションの再生速度
	float frameRate;	// アニメーションのフレーム経過速度

	std::string origin;	// モデルの原点の名前
	std::string playingLabel;	// 再生中のアニメーションの名札

	MATRIX offsetMatrix;	// ルートに常に掛ける行列
};