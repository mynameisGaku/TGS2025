#pragma once
#include "framework/myDxLib.h"
#include "src/util/getset/GetSet.h"

// このクラスはDxLibに依存しているので注意

/// <summary>
/// 色をRGBでひとまとめに保持するデータクラス
/// </summary>
class RGBColor
{
public:
	RGBColor();

	/// <summary>
	/// RGBから色をセット
	/// </summary>
	/// <param name="_r">赤　有効範囲 0 ～ 255</param>
	/// <param name="_g">緑　有効範囲 0 ～ 255</param>
	/// <param name="_b">青　有効範囲 0 ～ 255</param>
	RGBColor(int _r, int _g, int _b);

	/// <summary>
	/// RGB全てに同じ値をセット(=グレースケール)
	/// </summary>
	/// <param name="whiteness">セットする値(=白さ)　有効範囲 0 ～ 255</param>
	RGBColor(int whiteness);

	RGBColor(const COLOR_F& colorF);
	operator const COLOR_F() const;
	const RGBColor operator+(const RGBColor& c) const;
	const RGBColor operator-(const RGBColor& c) const;
	const RGBColor operator*(const float& f) const;

	/// <summary>
	/// DxlibのSetDrawBrightをこのインスタンスの色で設定する
	/// </summary>
	void SetDrawBright() const;

	//dxlib置き場から
	// より理解したい場合：https://www.peko-step.com/tool/hsvrgb.html
	/// <summary>
	/// HSVから色をセット
	/// </summary>
	/// <param name="H">色相(Hue)　有効範囲 0.0f ～ 360.0f</param>
	/// <param name="S">彩度(Saturation)　有効範囲 0.0f ～ 1.0f</param>
	/// <param name="V">明度(Value)　有効範囲 0.0f ～ 1.0f</param>
	void SetColorHSV(float H, float S, float V);

	/// <summary>
	/// RGBから色をセット
	/// </summary>
	/// <param name="_r">赤　有効範囲 0 ～ 255</param>
	/// <param name="_g">緑　有効範囲 0 ～ 255</param>
	/// <param name="_b">青　有効範囲 0 ～ 255</param>
	void SetColorRGB(int _r, int _g, int _b);

	/// <summary>
	/// インデックスで要素を取得
	/// </summary>
	/// <param name="i">インデックス</param>
	/// <returns>インデックスが 0なら「R」、1なら「G」、2なら「B」</returns>
	int Get(int i) const;
	 
	/// <summary>
	/// インデックスで要素を選びセット
	/// インデックスが 0なら「R」、1なら「G」、2なら「B」にセットする
	/// </summary>
	/// <param name="i">インデックス</param>
	/// <param name="value">セットする値</param>
	void Set(int i, int value);

	/*▼=== アクセサ ===▼*/
	inline int GetRed() const { return r; }
	inline int GetGreen() const { return g; }
	inline int GetBlue() const { return b; }
	// 16進数6ケタで表すRGB色をint型で取得
	inline int GetColorInt() const { return DxLib::GetColor(r, g, b); }
	void SetRed(int value);
	void SetGreen(int value);
	void SetBlue(int value);

	GetSet<int> Red = (&r);
	GetSet<int> Green = (&g);
	GetSet<int> Blue = (&b);
private:
	enum class RGBParam
	{
		RED,
		GREEN,
		BLUE,
	};

	int r;	// 赤　有効範囲 0 ～ 255
	int g;	// 緑　有効範囲 0 ～ 255
	int b;	// 青　有効範囲 0 ～ 255

	// 各要素の値を0～255に納める
	void fixRange();
};

namespace RGB
{
	static const RGBColor WHITE = RGBColor(255, 255, 255);
	static const RGBColor BLACK = RGBColor(0, 0, 0);
	static const RGBColor RED = RGBColor(255, 0, 0);
	static const RGBColor MAGENTA = RGBColor(255, 0, 255);
	static const RGBColor GREEN = RGBColor(0, 255, 0);
	static const RGBColor BLUE = RGBColor(0, 0, 255);
}
