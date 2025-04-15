#pragma once

// ◇汎用
#include <string>

// ◇個別で必要な物
#include "KeyDefine.h"

//==========================================================================================
// ▼定義値

namespace {

	static constexpr int KEY_BUFFER = 256;					// キーボードのバッファ
}

//==========================================================================================

namespace KeyController {

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	//==========================================================================================
	// ▼入力情報の取得

	/// <summary>
	/// 現在のキーボード入力を確認
	/// </summary>
	/// <param name="_keyInput">キーボードの入力情報</param>
	/// <param name="keyCode">入力の種類</param>
	/// <returns>押されていたら:True</returns>
	bool CheckPushStatusCurrent(KeyDefine::KeyCode keyCode);

	/// <summary>
	/// 1フレーム前のキーボード入力を確認
	/// </summary>
	/// <param name="_keyInput">キーボードの入力情報</param>
	/// <param name="keyCode">入力の種類</param>
	/// <returns>押されていたら:True</returns>
	bool CheckPushStatusBefore(KeyDefine::KeyCode keyCode);
}