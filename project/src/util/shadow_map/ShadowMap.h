#pragma once

// ◇汎用
#include "src/util/math/vector3.h"
#include "src/util/math/vector2.h"

namespace ShadowMap {

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(int sizeX, int sizeY);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// カメラの描画範囲で影計算を行う
	/// </summary>
	void DrawBegin(int cameraIndex = 0);

	/// <summary>
	/// 影の描画を終了する
	/// </summary>
	void DrawEnd();

	/// <summary>
	/// 描画に使用する影の設定を解除する
	/// </summary>
	void CleanUp();

	/// <summary>
	/// 影描画を適応する
	/// </summary>
	/// <param name="">描画関数</param>
	void Draw(void (*func)(), int cameraIndex = 0);

	/// <summary>
	/// 稼働するかを設定する
	/// </summary>
	/// <param name="value">trueならUpdateやDrawの処理を行うことができる</param>
	void SetIsActive(bool value);

	/// <summary>
	/// 稼働しているかを取得する
	/// </summary>
	bool IsActive();
}