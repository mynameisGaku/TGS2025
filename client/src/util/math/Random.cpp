
#include <ctime>
#include "random.h"

//==============================================================================
// Randomizer
//==============================================================================

Randomizer Random;

#define DEFAULT_X 123456789
#define DEFAULT_Y 362436069
#define DEFAULT_Z 521288629
#define DEFAULT_W 88675123

Randomizer::Randomizer()
{
	SetSeed((int)std::time(nullptr));
}

//------------------------------------------------------------------------------
Randomizer::Randomizer(int seed)
{
	SetSeed(seed);
}

//------------------------------------------------------------------------------
void Randomizer::SetSeed(int seed)
{
	x = DEFAULT_X;
	y = DEFAULT_Y;
	z = DEFAULT_Z;
	w = (seed <= 0) ? 88675123 : seed;
}

//------------------------------------------------------------------------------
int Randomizer::GetInt()
{
	unsigned t = x ^ (x << 11);
	x = y;
	y = z;
	z = w;
	w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	return static_cast<int>(w & 0x7fffffff);	// ����������
}

//------------------------------------------------------------------------------
int Randomizer::GetInt(int maxValue)
{
	if (maxValue == 0) { return 0; }
	int r = GetInt();
	r %= maxValue;
	return r;
}

//------------------------------------------------------------------------------
int Randomizer::GetIntRange(int minValue, int maxValue)
{
	if (maxValue - minValue == 0) { return 0; }
	int r = GetInt();
	r %= maxValue - minValue;
	r += minValue;
	return r;
}

//------------------------------------------------------------------------------
int Randomizer::GetIntWidth(int median, int width)
{
	return GetIntRange(median - width, median + width);
}

//------------------------------------------------------------------------------
float Randomizer::GetFloat()
{
	int r = GetInt();
	r = (r & 0x007fffff) | 0x3f800000;			// 0x3f800000 �͎w������ 1111111�ŁA����͎w��1�������Br & 0x007fffff �ŉ�������K���Ɍ��߂Ă���B
	float f = *reinterpret_cast<float*>(&r);
	return f - 1.f;
}

//------------------------------------------------------------------------------
float Randomizer::GetFloat(float maxValue)
{
	float r = GetFloat();
	r *= maxValue;
	return r;
}

//------------------------------------------------------------------------------
float Randomizer::GetFloatRange(float minValue, float maxValue)
{
	float r = GetFloat();
	r *= (maxValue - minValue);
	r += minValue;
	return r;
}

//------------------------------------------------------------------------------
float Randomizer::GetFloatWidth(float median, float width)
{
	return GetFloatRange(median - width, median + width);
}