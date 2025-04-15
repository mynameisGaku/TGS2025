#pragma once
#include "../Library/myDxLib.h"
#include "../Source/Util/Vector2.h"
#include <vector>

/// <summary>
/// モーションブラーを再生する
/// </summary>
namespace BlurScreen {

	//==========================================================================================
	// ▼各種関数

	// 初期化
	void Init(float _alpha);

	// 再初期化
	void ReplayInit(float _duration, float fadeDuration = 0.25f);

	void Update();
	void Draw();

	// ブラー用スクリーンを削除する
	void Release();

	void Reset();

	// レンダリング前のスクリーンの描画
	void PreRenderBlurScreen();

	// レンダリング後のスクリーンの描画
	void PostRenderBlurScreen();

	// ブラーをフェード処理を行って、終了する
	void FadeOut(float duration);

	//==========================================================================================
	// ▼ゲッター

	// ブラー用スクリーンの情報を取得する
	int ScreenInfo(int i);

	// ブラーの効果時間を取得する
	float Duration();

	// フェード処理を行うか
	bool IsFadeOut();

	// ブラーを使用しているか
	bool IsUse();

};
