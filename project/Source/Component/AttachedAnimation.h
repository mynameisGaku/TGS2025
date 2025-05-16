#pragma once
#include "AnimationDefine.h"
#include <DxLib.h>
#include <list>

/// <summary>
/// アタッチ中のアニメーションの管理クラス
/// </summary>
/// <author>佐藤紘斗</author>
class AttachedAnimation
{
public:
	AttachedAnimation(int parentModel, const AnimInfo& info);
	~AttachedAnimation();

	virtual void Update() = 0;
	void RefreshDefaultBlendRate();

	inline float MaxFrame() const { return m_maxFrame; }
	inline bool IsLoop() const { return m_info.option.isLoop; }

	/*=== アクセサ ===*/
	// 現在の進行度（0..1）
	inline void SetFrameByRatio(float ratio) { m_frame = m_maxFrame * ratio; }
	// 現在の進行度（0..1）
	inline float FrameByRatio() const { return m_maxFrame > 0 ? m_frame / m_maxFrame : 1.0f; }
	// 現在のフレーム
	inline void SetFrame(float frame) { m_frame = frame; }
	// 現在のフレーム
	inline float Frame() const { return m_frame; }
	// 再生速度
	inline void SetPlaySpeed(float speed) { m_playSpeed = speed; }
	// モデルに掛けるブレンド率
	inline float BlendRate() const { return m_blendRate * m_defaultBlendRate; }
	// モデルに掛けるブレンド率
	inline void SetBlendRate(float rate) { m_blendRate = rate; }
	// モデルに掛けるブレンド率
	inline float DefaultBlendRate() const { return m_defaultBlendRate; }
	// モデルに掛けるブレンド率
	inline void SetDefaultBlendRate(float rate) { m_defaultBlendRate = rate; }
	// アニメーション情報
	inline const AnimInfo& Info() const { return m_info; }
	// アタッチ済アニメーションのハンドル
	inline int AttachID() const { return m_attachID; }
protected:
	// 再生を進める
	void updateFrame();

	AnimInfo m_info;

	int m_parentModel;	// モデルハンドル
	int m_attachID;	// アタッチ済アニメーションのハンドル

	float m_frame;	// 現在フレーム（小数もある）
	float m_maxFrame;	// 最大フレーム
	float m_playSpeed;	// 再生速度倍率
	float m_blendRate;	// ブレンド率
	float m_defaultBlendRate;	// 初期ブレンド率

	bool m_first;	// 最初のアップデート時true

};

// 全体にかけるアニメーション
class AttachedAnimation_Main : public AttachedAnimation
{
public:
	AttachedAnimation_Main(int parentModel, const AnimInfo& info);

	void Update() override;

	// ルートフレームの行列を更新
	void UpdateRootMatrix();

	inline const MATRIX& RootMatrix() const { return m_rootMatrix; }
private:
	int m_hRoot;	// モデルのルートフレームのハンドル
	MATRIX m_rootMatrix;	// ルートフレーム移動用の行列
};

// メインアニメーションを一部上書きするサブアニメーション
class AttachedAnimation_Sub : public AttachedAnimation
{
public:
	AttachedAnimation_Sub(int parentModel, const AnimInfo& info, std::string target);

	void Update() override;
private:
	int m_targetID;	// アニメーションを適用するフレームID
};
