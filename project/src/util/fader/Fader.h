#pragma once
#include "framework/gameObject.h"
#include "framework/myDxLib.h"
#include "src/util/time/GameTime.h"
#include "src/util/easing/Easing.h"

/// <summary>
/// フェードを管理する
/// </summary>
namespace Fader {

	//================================================================================
	// ▼各種関数

	void Init();
	void Update();
	void Draw();
	void Release();

	//================================================================================
	// ▼フェード関連

	/// <summary>
	/// フェードを開始する
	/// </summary>
	/// <param name="totalTime">フェード時間</param>
	/// <param name="easingType">補間種類</param>
	/// <param name="begin">補間前の不透明度</param>
	/// <param name="end">補間後の不透明度</param>
	/// <param name="rate">時間経過率を適応させるか</param>
	void FadeStart(float totalTime, EasingType easingType, float begin, float end, GameTime::AdditionMethod timeAddition);

	/// <summary>
	/// フェードインする
	/// </summary>
	/// <param name="time">フェード時間</param>
	/// <param name="easingType">補間種類</param>
	/// <param name="rate">時間経過率を適応させるか</param>
	inline void FadeIn(float time, EasingType easingType = EasingType::Linear, GameTime::AdditionMethod timeAddition = GameTime::AdditionMethod::Rate) { FadeStart(time, easingType, 255.0f, 0.0f, timeAddition); }

	/// <summary>
	/// フェードアウトする
	/// </summary>
	/// <param name="time">フェード時間</param>
	/// <param name="easingType">補間種類</param>
	/// <param name="rate">時間経過率を適応させるか</param>
	inline void FadeOut(float time, EasingType easingType = EasingType::Linear, GameTime::AdditionMethod timeAddition = GameTime::AdditionMethod::Rate) { FadeStart(time, easingType, 0.0f, 255.0f, timeAddition); }

	//================================================================================
	// ▼レターボックス関連

	/// <summary>
	/// レターボックスの描画を開始する
	/// </summary>
	/// <param name="totalTime">表示するまでの時間</param>
	/// <param name="size">幅の大きさ</param>
	/// <param name="easingType">補間種類</param>
	/// <param name="rate">時間経過率を適応させるか</param>
	/// <param name="fadeIn">フェード方法。Trueならイン、Falseならアウト</param>
	void LetterBoxStart(float totalTime, float size, EasingType easingType, GameTime::AdditionMethod timeAddition, bool fadeIn);

	/// <summary>
	/// レターボックスの描画を終了する
	/// </summary>
	/// <param name="totalTime">表示するまでの時間</param>
	/// <param name="size">幅の大きさ</param>
	/// <param name="easingType">補間種類</param>
	/// <param name="rate">時間経過率を適応させるか</param>
	inline void LetterBoxFadeIn(float totalTime, float size, EasingType easingType = EasingType::Linear, GameTime::AdditionMethod timeAddition = GameTime::AdditionMethod::Rate) { LetterBoxStart(totalTime, size, easingType, timeAddition, true); }
	inline void LetterBoxFadeOut(float totalTime, float size, EasingType easingType = EasingType::Linear, GameTime::AdditionMethod timeAddition = GameTime::AdditionMethod::Rate) { LetterBoxStart(totalTime, size, easingType, timeAddition, false); }

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// フェードが再生中か
	/// </summary>
	/// <returns>再生中の場合はTrue</returns>
	bool IsPlaying();

	/// <summary>
	/// レターボックスのフェードが再生中か
	/// </summary>
	/// <returns>再生中の場合はTrue</returns>
	bool IsPlayingLetterBox();

	/// <summary>
	/// フェードが表示中か
	/// </summary>
	/// <returns>表示中の場合はTrue</returns>
	bool IsDisplay();
};
