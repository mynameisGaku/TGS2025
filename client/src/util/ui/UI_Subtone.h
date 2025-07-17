#pragma once
// ◇継承元
#include "src/util/object2D/Object2D.h"

// ◇汎用
#include "src/util/math/Vector2.h"

/// <summary>
/// サブトーンの情報
/// </summary>
class UI_Subtone : public Object2D {
public:
	//================================================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// サブトーンを設定する
	/// </summary>
	UI_Subtone();

	/// <summary>
	/// サブトーンを設定する
	/// </summary>
	/// <param name="image">画像</param>
	/// <param name="color">画像が指定されていない場合、表示する色</param>
	/// <param name="alpha">不透明度</param>
	/// <param name="active">機能しているか</param>
	/// <param name="begin">描画始点座標(左上)</param>
	/// <param name="end">描画終点座標(右下)</param>
	UI_Subtone(const int& image, const int& color, const int& alpha, const bool& active, const Vector2& begin, const Vector2& end);

	~UI_Subtone();

	//================================================================================
	// ▼各種関数

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const Vector2& begin, const Vector2& end);

	/// <summary>
	/// 始点・終点から左上、右上、左下、右下の座標を設定する
	/// </summary>
	void SetPoint(const Vector2& begin, const Vector2& end);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// 画像を取得する
	/// </summary>
	/// <returns></returns>
	inline int Image() const { return hImage; }

	/// <summary>
	/// 画像が設定されていない場合に表示する色を取得する
	/// </summary>
	inline int Color() const { return color; }

	/// <summary>
	/// 不透明度を取得する
	/// </summary>
	inline int Alpha() const { return alpha; }

	/// <summary>
	/// 描画する始点座標を取得する(左上)
	/// </summary>
	inline Vector2 BeginPos() const { return begin; }

	/// <summary>
	/// 描画する終点座標を取得する(右下)
	/// </summary>
	inline Vector2 EndPos() const { return end; }

private:
	int color;		// 色
	int alpha;		// 不透明度

	Vector2 begin;	// 始点座標
	Vector2 end;	// 終点座標

	static const int POINT_DIR_NUM = 4;	// 描画する際の向き

	// 四角形の点の色
	const unsigned int PointColor[4] = {
		GetColor(255, 255, 255),// 左上
		GetColor(255,   0,   0),// 右上
		GetColor(  0, 255,   0),// 右下
		GetColor(  0,   0, 255)	// 左下
	};

	Vector2 point[4];	// 画像描画位置(0 = 左上、1 = 右上、2 = 左下、3 = 右下)
};