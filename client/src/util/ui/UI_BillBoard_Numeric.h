#pragma once

// ◇継承元
#include "src/util/ui/UI_Canvas.h"

// ◇汎用
#include <vector>

// ◇個別で必要な物
#include "src/util/ui/UI_BillBoard.h"

/// <summary>
/// 数値を描画するビルボード。0～9が書かれた画像を10分割した物を使用する。
/// </summary>
class UI_BillBorad_Numeric : public UI_Canvas {
public:
	UI_BillBorad_Numeric();
	~UI_BillBorad_Numeric();

	void Update() override;
	void Draw() override;

	void Add(const int numberGraph[10], const Transform& trs, const Vector2& center, const float& clipSize, const int& value);

private:
	std::vector<UI_BillBoard*> numbers;	// 数値を描画するビルボード
	std::vector<Vector3> numbersPos;	// ビルボードの既定座標
	float clipSize;		// 画像の切り抜きの大きさ
};