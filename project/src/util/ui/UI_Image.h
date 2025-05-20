#pragma once
// ◇継承元
#include "src/util/ui/UI_Canvas.h"

/// <summary>
/// 画像表示を行うクラス
/// </summary>
/// <author> ミッチ </author>
class UI_Image : public UI_Canvas {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// 
	/// </summary>
	/// <param name="image">描画画像</param>
	/// <param name="trs">座標・回転・拡縮の情報</param>
	UI_Image(const int& image, const RectTransform& trs);
	~UI_Image();

	//==========================================================================================
	// ▼各種関数

	void Draw() override;

	/// <summary>
	/// 残像描画処理
	/// </summary>
	void DrawAfterImage() override;

};