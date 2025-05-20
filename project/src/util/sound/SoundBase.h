#pragma once

// ◇継承元
#include "../Library/gameObject.h"

// ◇汎用
#include "easing.h"
#include "../Source/Util/Vector3.h"

// ◇個別で必要な物
#include "soundDefine.h"

/// <summary>
/// サウンドを再生するクラス
/// </summary>
class SoundBase : public GameObject {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	SoundBase();
	~SoundBase();

	//================================================================================
	// ▼各種関数

	void Update() override;

	/// <summary>
	/// サウンドを再生する
	/// </summary>
	/// <param name="info">再生するサウンドの情報</param>
	/// <param name="label">判別ラベル</param>
	void Play(SoundDefine::SoundInfo* info, const std::string& label);

	/// <summary>
	/// パンを設定して再生する
	/// </summary>
	/// <param name="info">再生するサウンドの情報</param>
	/// <param name="label">判別ラベル</param>
	/// <param name="pos">再生する座標</param>
	void PlaySetPan(SoundDefine::SoundInfo* info, const std::string& label, Vector3* pos);

	/// <summary>
	/// 周波数を設定して再生する
	/// </summary>
	/// <param name="id">サウンドの種類</param>
	/// <param name="frequency">周波数</param>
	/// <param name="label">判別ラベル</param>
	void PlaySetFrequency(SoundDefine::SoundInfo* info, const std::string& label, const float& frequency = -1.0f);

	/// <summary>
	/// サウンドの周波数を設定する
	/// </summary>
	/// <param name="frequency">周波数</param>
	void SetFrequency(const float& frequency);

	/// <summary>
	/// 再生 フェードイン版
	/// </summary>
	/// <param name="info">サウンドの情報</param>
	/// <param name="label">判別ラベル</param>
	/// <param name="duration">再生までの時間(秒)</param>
	/// <param name="easingType">イージングの種類</param>
	/// <param name="beginVolume">フェード開始時の音量</param>
	/// <param name="endVolume">フェード終了時の音量</param>
	void FadeIn(SoundDefine::SoundInfo* info, const std::string& label, const SoundDefine::SoundFade& fade);

	/// <summary>
	/// 停止 フェードアウト版
	/// </summary>
	/// <param name="duration">停止までの時間(秒)</param>
	/// <param name="easingtype">イージングの種類</param>
	/// <param name="beginVolume">フェード開始時の音量</param>
	/// <param name="endVolume">フェード終了時の音量</param>
	/// <param name="isFadeOutEnd">フェード終了時にサウンドを停止させるか</param>
	void FadeOut(const SoundDefine::SoundFade& fade, const bool& isFadeOutEnd = true);

	/// <summary>
	/// サウンドを止める
	/// </summary>
	void Stop();

	/// <summary>
	/// パンの設定を行う
	/// </summary>
	/// <param name="pos">再生座標</param>
	void SetPan(Vector3* pos);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// 自身の持つエフェクト情報を取得する
	/// </summary>
	inline const SoundDefine::SoundInfo* Info() const { return info; }

	/// <summary>
	/// 判別番号を取得する
	/// </summary>
	inline const std::string Label() const { return label; }

	/// <summary>
	/// フェード中かを取得する
	/// </summary>
	inline const bool IsFade() const { return fade.info.isActive; }

	/// <summary>
	/// 再生されているかを取得する
	/// </summary>
	/// <returns>再生されていたらtrue</returns>
	bool IsPlaying() const;

	/// <summary>
	/// 整合性チェックを行う
	/// </summary>
	/// <param name="typeName">名前</param>
	/// <param name="label">判別ラベル</param>
	/// <returns>合っていればtrue</returns>
	bool CheckConsistency(const std::string& typeName, const std::string& label) const;

private:

	/// <summary>
	/// フェードによる音量変化処理
	/// </summary>
	void applyFadeVolume();

	//================================================================================
	// ▼メンバ変数

	SoundDefine::SoundInfo* info;	// 自身のサウンド情報
	SoundDefine::SoundFade fade;	// フェード関連

	std::string label;	// 判別ラベル
	bool isFadeOutEnd;	// フェードアウト終了後にサウンドを停止するか
	bool isSetPan;		// パン設定を行うか
	Vector3* playPos;	// 再生座標
};