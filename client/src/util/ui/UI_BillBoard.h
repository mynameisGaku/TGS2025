#pragma once

// ◇継承元
#include "src/util/ui/UI_Canvas.h"

#include "src/util/transform/Transform.h"

/// <summary>
/// ビルボードの処理を行うクラス
/// </summary>
class UI_BillBoard : public UI_Canvas {
public:

	Transform* transform;

	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// ビルボードを生成する
	/// </summary>
	/// <param name="graph">使用する画像ハンドル</param>
	/// <param name="trs">描画座標</param>
	/// <param name="center"></param>
	/// <param name="size"></param>
	UI_BillBoard(const int& image, const Transform& trs, const Vector2& center, const float& clipSize);

	~UI_BillBoard();

	//==========================================================================================
	// ▼各種関数

	void Update() override;
	void Draw() override;

};
