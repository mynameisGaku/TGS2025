
#include <math.h>
#include <float.h>
#include <DxLib.h>

// 標準ヘッダの math.h と被るので Utils を付けている
#include "mathUtils.h"


//==============================================================================
// Math
//==============================================================================

const float MathUtil::PI = 3.1415926535897932384626433832795f;
const float MathUtil::PIOver2 = 1.5708f;
const float MathUtil::EPSILON = 0.00001f;
const float MathUtil::GRAVITY_2D = 9.8f / 20.0f;
const float MathUtil::GRAVITY_3D = -GRAVITY_2D;
const float MathUtil::PI_SQ = PI * PI;
const float MathUtil::PI_TW = PI * 2.0f;

//------------------------------------------------------------------------------
unsigned int MathUtil::NextPow2(unsigned int value)
{
	unsigned int i = 1;
	while (value > (i <<= 1)) {
		if (!i) break;
	}
	return i;
}

//------------------------------------------------------------------------------
bool MathUtil::EqualsFloat(float value1, float value2)
{
	return ((value2 - FLT_EPSILON) < value1) && (value1 < (value2 + FLT_EPSILON));
}

//------------------------------------------------------------------------------
bool MathUtil::IsNaN(float value)
{
#ifdef _MSC_VER
	return (_isnan(value) != 0);
#else
	return (isnan(value) != 0);
#endif
}

//------------------------------------------------------------------------------
bool MathUtil::IsNaN(double value)
{
#ifdef _MSC_VER
	return (_isnan(value) != 0);
#else
	return (isnan(value) != 0);
#endif
}

//------------------------------------------------------------------------------
bool MathUtil::IsInf(float value)
{
#ifdef _MSC_VER
	return (_isnan(value) == 0 && _finite(value) == 0);
#else
	return (isinf(value) != 0);
#endif
}

//------------------------------------------------------------------------------
bool MathUtil::IsInf(double value)
{
#ifdef _MSC_VER
	return (_isnan(value) == 0 && _finite(value) == 0);
#else
	return (isinf(value) != 0);
#endif
}

//------------------------------------------------------------------------------
float MathUtil::Lerp(float v1, float v2, float t)
{
	return v1 + ((v2 - v1) * t);
}

//------------------------------------------------------------------------------
float MathUtil::QuadAccel(float p, float v, float a, float t)
{
	return p + (v * t) + (0.5f * a * t * t);
}

//------------------------------------------------------------------------------
float MathUtil::Hermite(float v1, float a1, float v2, float a2, float t)
{
	float a = 2.f * (v1 - v2) + (a1 + a2);
	float b = 3.f * (v2 - v1) - (2.f * a1) - a2;
	float r = a;
	r *= t;
	r += b;
	r *= t;
	r += a1;
	r *= t;
	return r + v1;
}

//------------------------------------------------------------------------------
float MathUtil::CatmullRom(float v1, float v2, float v3, float v4, float t)
{
	float d1 = (v3 - v1) * 0.5f;
	float d2 = (v4 - v2) * 0.5f;
	return (2.0f * v2 - 2.0f * v3 + d1 + d2) * t * t * t + (-3.0f * v2 + 3.0f * v3 - 2.0f * d1 - d2) * t * t + d1 * t + v2;
}

//------------------------------------------------------------------------------
float MathUtil::Half(float value)
{
	return value / 2.0f;
}

//------------------------------------------------------------------------------
float MathUtil::Quarter(float value)
{
	return value / 4.0f;
}

//------------------------------------------------------------------------------
bool MathUtil::Probability(int probability)
{
	return GetRand(99) < probability;
}

//------------------------------------------------------------------------------
float MathUtil::RateDecrease(float start, float rate)
{
	if (rate >= 1.0f || rate <= 0.0f)
		return start;

	float value = start;
	float ans = 0.0f;
	bool isLoop = true;

	while (isLoop) {
		value *= rate;
		ans += value;

		if (value <= EPSILON)
			isLoop = false;
	}

	return ans;
}

int MathUtil::Digit(int value)
{

	int digit = 0;

	if (value == 0)
		return 1;

	while (value != 0) {
		value /= 10;
		digit++;
	}

	return digit;
}

float MathUtil::Rounding(float value, int decimalPoint)
{
	float result = 0.0f;

	result = value * static_cast<float>(pow(10, decimalPoint - 1)); //四捨五入したい値を10の(n-1)乗倍する。
	result = roundf(result); //小数点以下を四捨五入する。
	result /= static_cast<float>(pow(10, decimalPoint - 1)); //10の(n-1)乗で割る。
	return result;
}

float MathUtil::RotAngle(float current, float terminus, float rotSpeed)
{

	float ans = current;	// 最終的な結果を保持する
	float diffAngle = terminus - current;// 角度の差異

	// マイナスなら一周回してプラスの符号にする
	if (diffAngle < 0)
		diffAngle += MathUtil::PI_TW;

	float angleReverseDif = -diffAngle + MathUtil::PI_TW;	// 逆回転の大きさ(値はプラス)

	// 時計回りの方が近い場合
	if (diffAngle < angleReverseDif) {
		if (diffAngle < rotSpeed)		// 角度の差異と回転速度を比較
			ans = terminus;
		else
			ans += rotSpeed;
	}
	// 反時計回りの方が近い場合
	else {
		if (angleReverseDif < rotSpeed)// 角度の差異と回転速度を比較
			ans = terminus;
		else
			ans -= rotSpeed;
	}

	return ans;
}

void MathUtil::RotLimit(float* rot)
{
	if (*rot > MathUtil::PI)
		*rot -= MathUtil::PI_TW;
	else if (*rot < -MathUtil::PI)
		*rot += MathUtil::PI_TW;
}

int MathUtil::CalcList(const std::list<int>& list)
{
	int result = 0;
	for (auto& it : list)
	{
		result += it;
	}
	return result;
}
