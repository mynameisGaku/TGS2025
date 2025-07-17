#pragma once
// ◇継承元
#include "src/util/object2D/Object2D.h"

// ◇演出・機能
#include "src/util/easing/Easing.h"

// ◇個別で必要な物
#include "src/util/ui/UI_Subtone.h"
#include "src/util/ui/UI_Define.h"

/// <summary>
/// UIに関する基底クラス
/// </summary>
/// <author> ミッチ </author>
class UI_Canvas : public Object2D {
public:
	//================================================================================
	// ▼列挙型

	/// <summary>
	/// イージング挙動の種類
	/// </summary>
	enum EasingMovement {
		eMove,	// 移動
		eRot,	// 回転
		eScale,	// 拡縮
		eAlpha,	// 不透明度
		eMax
	};

	//================================================================================
	// ▼コンストラクタ

	UI_Canvas();
	virtual ~UI_Canvas();

	//================================================================================
	// ▼各種関数

	virtual void Update() override;
	virtual void Draw() override;

	/// <summary>
	/// 残像描画処理
	/// </summary>
	virtual void DrawAfterImage() {};

	/// <summary>
	/// 点滅処理
	/// </summary>
	void BlinkUpdate();

	/// <summary>
	/// 補間処理
	/// </summary>
	void EasingUpdate();

	/// <summary>
	/// 残像描画処理
	/// </summary>
	void AfterImageUpdate();

	/// <summary>
	/// 画面サイズに応じて拡縮倍率を変化させる
	/// </summary>
	void ScaleWithScreenSize();

	/// <summary>
	/// サブトーンを画像に合わせて調整する
	/// </summary>
	void SubtoneAdaptation(const UI_Subtone& sub);

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 不透明度を設定する
	/// </summary>
	inline void SetAlpha(int _alpha) { alpha = _alpha; }

	/// <summary>
	/// 描画優先度を設定する
	/// </summary>
	void SetPriority(int value);

	/// <summary>
	/// サブトーンを設定する
	/// </summary>
	virtual void SetSubtone(const UI_Subtone& sub);

	/// <summary>
	/// 更新処理を行うかを設定する
	/// </summary>
	inline void SetIsUpdate(bool value) { isUpdate = value; }

	/// <summary>
	/// 描画処理を行うかを設定する
	/// </summary>
	inline void SetIsDraw(bool value) { isDraw = value; }

	/// <summary>
	/// 点滅処理を行うかを設定する
	/// </summary>
	inline void SetNeedBlink(bool value) { needBlink = value; }

	/// <summary>
	/// 残像処理を行うかを設定する
	/// </summary>
	inline void SetAfterImage(bool value) { needAfterImage = value; }

	/// <summary>
	/// 点滅する時間を設定する(引数の秒数で点滅の一連の流れを行う)
	/// </summary>
	inline void SetBlinkTime(float sec) { blinkSpeed = sec; }

	/// <summary>
	/// イージング挙動を設定する
	/// </summary>
	/// <param name="useType">挙動の種類</param>
	/// <param name="begin">始点</param>
	/// <param name="end">終点</param>
	/// <param name="totalTime">効果時間</param>
	/// <param name="type">イージングの種類</param>
	template<typename Ty>
	void SetEasing(EasingMovement move, const Ty& begin, const Ty& end, float totalTime, EasingType type = EasingType::Linear);

	/// <summary>
	/// イージング挙動を設定する
	/// </summary>
	/// <param name="useType">挙動の種類</param>
	/// <param name="begin">始点</param>
	/// <param name="end">終点</param>
	/// <param name="totalTime">効果時間</param>
	/// <param name="type">イージングの種類</param>
	template<typename Ty>
	void SetAfterImageEasing(EasingMovement move, const Ty& begin, const Ty& end, float totalTime, EasingType type = EasingType::Linear);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// 画像を取得する
	/// </summary>
	inline int Image() const { return hImage; }

	/// <summary>
	/// 不透明度を取得する
	/// </summary>
	inline int Alpha() const { return alpha; }

	/// <summary>
	/// 描画優先度を取得する
	/// </summary>
	inline int Priority() const { return priority; }

	/// <summary>
	/// 更新処理を行うかを取得する
	/// </summary>
	inline bool IsUpdate() const { return (IsActive() && isUpdate); }

	/// <summary>
	/// 描画処理を行うかを取得する
	/// </summary>
	inline bool IsDraw() const { return (IsActive() && isDraw); }

	/// <summary>
	/// 点滅処理を行うかを取得する
	/// </summary>
	inline bool NeedBlink() const { return needBlink; }

	/// <summary>
	/// サブトーンを取得する
	/// </summary>
	inline UI_Subtone* Subtone() const { return subtone; }

	/// <summary>
	/// イージング挙動が稼働しているか
	/// </summary>
	/// <returns>イージング挙動が一つでも稼働していればTrue</returns>
	bool IsAllEasingRun() const;

	/// <summary>
	/// 残像イージング挙動が稼働しているか
	/// </summary>
	/// <returns>残像イージング挙動が一つでも稼働していればTrue</returns>
	bool IsAllEasingRunAfterImage() const;

protected:
	//================================================================================
	// ▼変数's

	int alpha;			// 不透明度
	int priority;		// 描画優先度
	bool isUpdate;		// 更新処理を行うか
	bool isDraw;		// 描画処理を行うか
	UI_Subtone* subtone;// サブトーンの情報

	//================================================================================
	// ▼画面情報

	Vector2 scaleWithScreenSize;// 画面サイズに応じて拡縮倍率を変化させる
	int screenX, screenY;		// 画面サイズ
	int colorBitDepth;			// カラービット

	//================================================================================
	// ▼イージング挙動

	EasingVec2 easingMove;	// 移動用イージング情報
	EasingVec2 easingScale;	// 拡縮用イージング情報
	EasingFloat easingRot;	// 回転用イージング情報
	EasingInt easingAlpha;	// 不透明度用イージング情報

	//================================================================================
	// ▼点滅挙動

	float blinkTime;	// 点滅時間
	float blinkTimeMax;	// 最大点滅時間

	float blinkCounter;	// 点滅用
	float blinkSpeed;	// 点滅速度

	bool needBlink;		// 点滅させるか

	//================================================================================
	// ▼残像挙動

	EasingVec2 easingMoveAftImg;	// 移動用イージング情報
	EasingVec2 easingScaleAftImg;	// 拡縮用イージング情報
	EasingFloat easingRotAftImg;	// 回転用イージング情報
	EasingInt easingAlphaAftImg;	// 不透明度用イージング情報

	bool needAfterImage;	// 残像を表示するか
};

/// <summary>
/// イージング挙動を設定する
/// </summary>
/// <param name="useType">挙動の種類</param>
/// <param name="begin">始点</param>
/// <param name="end">終点</param>
/// <param name="totalTime">効果時間</param>
/// <param name="type">イージングの種類</param>
template<typename Ty>
inline void UI_Canvas::SetEasing(EasingMovement move, const Ty& begin, const Ty& end, float totalTime, EasingType type) {

	switch (move) {
	case EasingMovement::eMove:
		easingMove.begin = begin;
		easingMove.end = end;
		easingMove.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingMove.info, &rectTransform->position, easingMove.end, easingMove.begin);
		break;

	case EasingMovement::eRot:
		easingRot.begin = begin;
		easingRot.end = end;
		easingRot.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingRot.info, &rectTransform->rotation, easingRot.end, easingRot.begin);
		break;

	case EasingMovement::eScale:
		easingScale.begin = begin;
		easingScale.end = end;
		easingScale.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingScale.info, &rectTransform->scale, easingScale.end, easingScale.begin);
		break;

	case EasingMovement::eAlpha:
		easingAlpha.begin = begin;
		easingAlpha.end = end;
		easingAlpha.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingAlpha.info, &alpha, easingAlpha.end, easingAlpha.begin);
		break;
	}
}

/// <summary>
/// イージング挙動を設定する
/// </summary>
/// <param name="useType">挙動の種類</param>
/// <param name="begin">始点</param>
/// <param name="end">終点</param>
/// <param name="totalTime">効果時間</param>
/// <param name="type">イージングの種類</param>
template<typename Ty>
inline void UI_Canvas::SetAfterImageEasing(EasingMovement move, const Ty& begin, const Ty& end, float totalTime, EasingType type) {

	switch (move) {
	case EasingMovement::eMove:
		easingMoveAftImg.begin = begin;
		easingMoveAftImg.end = end;
		easingMoveAftImg.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingMoveAftImg.info, &easingMoveAftImg.current, easingMoveAftImg.end, easingMoveAftImg.begin);
		break;

	case EasingMovement::eRot:
		easingRotAftImg.begin = begin;
		easingRotAftImg.end = end;
		easingRotAftImg.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingRotAftImg.info, &easingRotAftImg.current, easingRotAftImg.end, easingRotAftImg.begin);
		break;

	case EasingMovement::eScale:
		easingScaleAftImg.begin = begin;
		easingScaleAftImg.end = end;
		easingScaleAftImg.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingScaleAftImg.info, &easingScaleAftImg.current, easingScaleAftImg.end, easingScaleAftImg.begin);
		break;

	case EasingMovement::eAlpha:
		easingAlphaAftImg.begin = begin;
		easingAlphaAftImg.end = end;
		easingAlphaAftImg.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingAlphaAftImg.info, &easingAlphaAftImg.current, easingAlphaAftImg.end, easingAlphaAftImg.begin);
		break;
	}
}