#pragma once
#include <list>
#include "src/util/math/Vector3.h"

/**
	@brief	一般的な数学関数と定数をまとめたモジュールです。
*/
class MathUtil
{
public:

	static const float PI;			///< 円周率	(3.14159265...)
	static const float PIOver2;		///< 円周率 / 2
	static const float EPSILON;		///< 浮動小数点の誤差
	static const float PI_TW;		///< 円周率の２倍
	static const float PI_SQ;		///< 円周率の２乗
	static const float GRAVITY_2D;	///< 重力(2D)
	static const float GRAVITY_3D;	///< 重力(3D)

public:

	/// 度をラジアンに変換します。
	static inline float ToRadians(float d) { return d * 0.017453292519943295769236907684886f; }	// 0.017… は180 の逆数 * PI

	/// ラジアンを度に変換します。
	static inline float ToDegrees(float r) { return r * 57.295779513082320876798154814105f; }	// 57.29… はPI の逆数 * 180

	/// 値を指定された範囲内に制限します。
	static inline float Clamp(float value, float minValue, float maxValue) { return (value > maxValue) ? maxValue : (value < minValue) ? minValue : value; }

	/// 値を0.0～1.0の範囲内に制限します。
	static inline float Clamp01(float value) { return Clamp(value, 0.0f, 1.0f); }

	static inline void ClampAssing(float* value, float minValue,float maxValue) { *value = Clamp(*value, minValue, maxValue); }

	/// 指定値以上の最小の 2 のべき乗数を返します。
	static unsigned int NextPow2(unsigned int value);

	/// 誤差を考慮し、浮動小数点の比較を行います。
	static bool EqualsFloat(float value1, float value2);

	/// 値が NaN(非数) かを判定します。
	static bool IsNaN(float value);

	/// 値が NaN(非数) かを判定します。
	static bool IsNaN(double value);

	/// 値が無限大かを判定します。
	static bool IsInf(float value);

	/// 値が無限大かを判定します。
	static bool IsInf(double value);

	/// 値が NaN(非数) または 無限大 かを判定します。
	static bool IsNaNOrInf(float value) { return (IsNaN(value) || IsInf(value)); }

	/// 値が NaN(非数) または 無限大 かを判定します。
	static bool IsNaNOrInf(double value) { return (IsNaN(value) || IsInf(value)); }

	/**
		@brief		等加速度運動の式で値を計算します。
		@param[in]	p	: 開始値
		@param[in]	v	: 初速度
		@param[in]	a	: 加速度
		@param[in]	t	: 時間
		@return		計算結果
	*/
	static float QuadAccel(float p, float v, float a, float t);

	/**
		@brief		2つの値の間を線形補完します。
		@param[in]	v1	: 開始値
		@param[in]	v2	: 終了値
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static float Lerp(float v1, float v2, float t);

	static float LerpAngle(float a, float b, float t);

	/**
		@brief		エルミートスプライン補間を実行します。
		@param[in]	v1	: 開始値
		@param[in]	a1	: 開始値の接線の傾き(速度)
		@param[in]	v2	: 終了値
		@param[in]	a2	: 終了値の接線の傾き(速度)
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static float Hermite(float v1, float a1, float v2, float a2, float t);

	/**
		@brief		Catmull-Rom 補間を実行します。
		@param[in]	v1	: 1番目の位置
		@param[in]	v2	: 2番目の位置 (t = 0.0 のときの値)
		@param[in]	v3	: 3番目の位置 (t = 1.0 のときの値)
		@param[in]	v4	: 4番目の位置
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static float CatmullRom(float v1, float v2, float v3, float v4, float t);

	/**
		@brief		Catmull-Rom 補間を実行します。
		@param[in]	v1	: 1番目の位置
		@param[in]	v2	: 2番目の位置 (t = 0.0 のときの値)
		@param[in]	v3	: 3番目の位置 (t = 1.0 のときの値)
		@param[in]	v4	: 4番目の位置
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static Vector3 CatmullRomVec3(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

	// 半分にする
	static float Half(float value);

	// 四半にする
	static float Quarter(float value);

	/// <summary>
	/// 確率の計算
	/// </summary>
	/// <param name="probability">確率(※百分率)</param>
	/// <returns>命中した場合、True</returns>
	static bool Probability(int probability);

	static float RateDecrease(float start, float rate);

	/// <summary>
	/// 桁数を取得する
	/// </summary>
	/// <param name="value">桁数を取得する値</param>
	static int Digit(int value);

	/// <summary>
	/// 四捨五入を行う
	/// </summary>
	/// <param name="value">四捨五入を行う値</param>
	/// <param name="decimalPoint">小数点何位まで四捨五入するか</param>
	/// <returns>四捨五入を行った値を取得する</returns>
	static float Rounding(float value, int decimalPoint);

	/// <summary>
	/// 角度を徐々に変化させる処理。
	/// 現在の値が終点角度になるまで、回転速度を加算し続ける。
	/// </summary>
	/// <param name="current">現在の値</param>
	/// <param name="terminus">終点角度</param>
	/// <param name="rotSpeed">回転速度</param>
	static float RotAngle(float current, float terminus, float rotSpeed);

	/// <summary>
	/// 回転角度に制限をかける(-PI～PI)
	/// </summary>
	static void RotLimit(float* rot);

	static int CalcList(const std::list<int>& list);
};
