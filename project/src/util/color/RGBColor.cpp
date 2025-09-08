#include "RGBColor.h"
#include "src/util/math/MathUtil.h"
#include <DxLib.h>

RGBColor::RGBColor() : RGBColor(0, 0, 0)
{
}

RGBColor::RGBColor(int _r, int _g, int _b)
{
	SetColorRGB(_r, _g, _b);
}

RGBColor::RGBColor(int whiteness) : RGBColor(whiteness, whiteness, whiteness)
{
}

RGBColor::RGBColor(const COLOR_F& colorF)
{
	SetColorRGB(MathUtil::RateToByte(colorF.r), MathUtil::RateToByte(colorF.g), MathUtil::RateToByte(colorF.b));
}

RGBColor::operator const COLOR_F() const
{
	return GetColorF(MathUtil::ByteToRate(r), MathUtil::ByteToRate(g), MathUtil::ByteToRate(b), 1.0f);
}

const RGBColor RGBColor::operator+(const RGBColor& c) const
{
	RGBColor ans;
	ans.SetRed(this->r + c.r);
	ans.SetGreen(this->g + c.g);
	ans.SetBlue(this->b + c.b);

	return ans;
}

const RGBColor RGBColor::operator-(const RGBColor& c) const
{
	RGBColor ans;
	ans.SetRed(this->r - c.r);
	ans.SetGreen(this->g - c.g);
	ans.SetBlue(this->b - c.b);

	return ans;
}

const RGBColor RGBColor::operator*(const float& f) const
{
	RGBColor ans;
	ans.SetRed(static_cast<int>(this->r * f));
	ans.SetGreen(static_cast<int>(this->g * f));
	ans.SetBlue(static_cast<int>(this->b * f));

	return ans;
}

void RGBColor::SetDrawBright() const
{
	DxLib::SetDrawBright(r, g, b);
}

void RGBColor::SetColorHSV(float H, float S, float V)
{
	int hi;
	float f, p, q, t;
	float red = 0.0f, green = 0.0f, blue = 0.0f;
	int ir, ig, ib;

	// 色相を60で割る。小数点以下切り捨て (0 <= H <= 360 だったら 0 ～ 6 の値になる)
	// 0   < 60  => 0
	// 60  < 120 => 1
	// 120 < 180 => 2
	// 180 < 240 => 3
	// 240 < 300 => 4
	// 300 < 360 => 5
	// 360       => 6
	hi = (int)(H / 60.0f);

	// ? : 三項演算子
	// 代入をするとき、条件分岐を一緒に書くことができる
	// 書き方 : (条件式) ? (真だったときの処理) : (偽だったときの処理)

	// hi が 6 だったとき(H が 360 だったとき)は 5 に合わせる、360 より大きいとき対策として %= 6 を入れる
	hi = hi == 6 ? 5 : hi %= 6;

	// なんかHSVをRGBに変換する公式
	f = H / 60.0f - (float)hi;
	p = V * (1.0f - S);
	q = V * (1.0f - f * S);
	t = V * (1.0f - (1.0f - f) * S);

	// 色相によって6パターンに場合分けする必要がある
	switch (hi)
	{
	case 0: red = V; green = t; blue = p; break;
	case 1: red = q; green = V; blue = p; break;
	case 2: red = p; green = V; blue = t; break;
	case 3: red = p; green = q; blue = V; break;
	case 4: red = t; green = p; blue = V; break;
	case 5: red = V; green = p; blue = q; break;
	}

	// 0.0 ～ 1.0 の値を 255.0 でスケーリング
	// ifを使い不正な値が入っていたら直す
	ir = (int)(red * 255.0f);
	if (ir > 255) ir = 255;
	else if (ir < 0) ir = 0;

	ig = (int)(green * 255.0f);
	if (ig > 255) ig = 255;
	else if (ig < 0) ig = 0;

	ib = (int)(blue * 255.0f);
	if (ib > 255) ib = 255;
	else if (ib < 0) ib = 0;

	// 取得できたRGBをメンバーに代入
	SetColorRGB(ir, ig, ib);
}

void RGBColor::SetColorRGB(int _r, int _g, int _b)
{
	SetRed(_r);
	SetGreen(_g);
	SetBlue(_b);
}

int RGBColor::Get(int i) const
{
	switch (i)
	{
	case (int)RGBParam::RED:
		return Red;
		break;
	case (int)RGBParam::GREEN:
		return Green;
		break;
	case (int)RGBParam::BLUE:
		return Blue;
		break;
	}

	return -1;
}

void RGBColor::Set(int i, int value)
{
	switch (i)
	{
	case (int)RGBParam::RED:
		SetRed(value);
		break;
	case (int)RGBParam::GREEN:
		SetGreen(value);
		break;
	case (int)RGBParam::BLUE:
		SetBlue(value);
		break;
	}
}

void RGBColor::SetRed(int value)
{
	r = value;
	if (r > 255) r = 255;
	else if (r < 0) r = 0;
}

void RGBColor::SetGreen(int value)
{
	g = value;
	if (g > 255) g = 255;
	else if (g < 0) g = 0;
}

void RGBColor::SetBlue(int value)
{
	b = value;
	if (b > 255) b = 255;
	else if (b < 0) b = 0;
}

void RGBColor::fixRange()
{
	SetColorRGB(r, g, b);
}
