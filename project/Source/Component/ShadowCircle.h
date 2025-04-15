#pragma once

// ◇継承元
#include "Component.h"

/// <summary>
/// 丸影の描画を行う
/// </summary>
class ShadowCircle : public Component {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	ShadowCircle();
	~ShadowCircle();

	//==========================================================================================
	// ▼各種関数

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:
	int handle;

};