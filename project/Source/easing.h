#pragma once
#include <math.h>
#include <limits>
#include <algorithm>
#include <cmath>
#include <assert.h>

/*

イージング関数

チートシート:
https://easings.net/ja  //ctrl + クリックでリンク先に飛ぶ

*/

/// <summary>
/// イージングの種類
/// </summary>
enum class EasingType
{
	None = -1,
	InQuad,
	OutQuad,
	InOutQuad,
	InCubic,
	OutCubic,
	InOutCubic,
	InQuart,
	OutQuart,
	InOutQuart,
	InQuint,
	OutQuint,
	InOutQuint,
	InSine,
	OutSine,
	InOutSine,
	//InExpo,
	//OutExpo,
	//InOutExpo,
	InCirc,
	OutCirc,
	InOutCirc,
	InBack,
	OutBack,
	InOutBack,
	InBounce,
	OutBounce,
	InOutBounce,
	Linear,
};

namespace EasingFunc
{
	static constexpr float Pai = 3.141592653589793f;

	template<typename Ty>
	static constexpr Ty _0 = static_cast<Ty>(0);
	template<typename Ty>
	static constexpr Ty _0_5 = static_cast<Ty>(0.5);
	template<typename Ty>
	static constexpr Ty _0_75 = static_cast<Ty>(0.75);
	template<typename Ty>
	static constexpr Ty _0_9375 = static_cast<Ty>(0.9375);
	template<typename Ty>
	static constexpr Ty _0_984375 = static_cast<Ty>(0.984375);

	template<typename Ty>
	static constexpr Ty _1 = static_cast<Ty>(1);
	template<typename Ty>
	static constexpr Ty _1_5 = static_cast<Ty>(1.5);
	template<typename Ty>
	static constexpr Ty _1_525 = static_cast<Ty>(1.525);

	template<typename Ty>
	static constexpr Ty _2 = static_cast<Ty>(2);
	template<typename Ty>
	static constexpr Ty _2_25 = static_cast<Ty>(2.25);
	template<typename Ty>
	static constexpr Ty _2_5 = static_cast<Ty>(2.5);
	template<typename Ty>
	static constexpr Ty _2_625 = static_cast<Ty>(2.625);
	template<typename Ty>
	static constexpr Ty _2_75 = static_cast<Ty>(2.75);

	template<typename Ty>
	static constexpr Ty _7_5625 = static_cast<Ty>(7.5625);

	template<typename Ty>
	static constexpr Ty _10 = static_cast<Ty>(10);

	template<typename Ty>
	static constexpr Ty _180 = static_cast<Ty>(180);

	template<typename Ty>
	static constexpr Ty _90 = static_cast<Ty>(90);

	template<class Ty>
	[[nodiscard]] static inline Ty ToRadian(const Ty angle)
	{
		return static_cast<Ty>(angle * Pai / 180.0f);
	}

	// 二つの浮動小数点数がほぼ等しいかどうかを比較する
	template<typename Ty>
	[[nodiscard]] static inline bool AdjEqual(const float epsilon_num, const float num)
	{
		// 絶対値を計算するラムダ関数
		constexpr auto Fabs{ [](const Ty num) constexpr {
			// num が 0 より大きければそのまま返す。そうでなければ -num を返す。
			if (num > 0.0f) return num; else return -num;
		} };

		// 型 Ty の最小の機械的な値を取得
		static constexpr auto Epsilon{ std::numeric_limits<Ty>::epsilon() };

		// 二つの数値の差の絶対値を計算
		auto dis{ Fabs(epsilon_num - num) };

		// 差の絶対値が Epsilon 以下であれば、二つの数はほぼ等しいと判断
		return (dis <= Epsilon);
	}


	template<typename Ty = float>
	[[nodiscard]] static inline Ty InQuad(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		return static_cast<Ty>(_max * time * time + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutQuad(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		return static_cast<Ty>(-_max * time * (time - 2.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutQuad(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		if (time / 2.0f < 1.0f)
			return static_cast<Ty>(_max / 2.0f * time * time + _min);

		--time;

		return static_cast<Ty>(-_max * (time * (time - 2.0f) - 1.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InCubic(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		return static_cast<Ty>(_max * time * time * time + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutCubic(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time = time / totaltime - 1.0f;

		return static_cast<Ty>(_max * (time * time * time + 1.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutCubic(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		if (time / 2.0f < 1.0f)
			return static_cast<Ty>(_max / 2.0f * time * time * time + _min);

		time -= 2.0f;

		return static_cast<Ty>(_max / 2.0f * (time * time * time + 2.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InQuart(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		return static_cast<Ty>(_max * time * time * time * time + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutQuart(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time = time / totaltime - 1.0f;

		return static_cast<Ty>(_max * -1.0f * (time * time * time * time - 1.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutQuart(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		if (time / 2.0f < 1.0f)
			return static_cast<Ty>(_max / 2.0f * time * time * time * time + _min);

		time -= 2.0f;

		return static_cast<Ty>(-_max / 2.0f * (time * time * time * time - 2.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InQuint(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		return static_cast<Ty>(_max * time * time * time * time * time + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutQuint(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time = time / totaltime - 1.0f;

		return static_cast<Ty>(_max * (time * time * time * time * time + 1.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutQuint(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		if (time / 2.0f < 1.0f)
			return static_cast<Ty>(_max / 2.0f * time * time * time * time * time + _min);

		time -= 2.0f;

		return static_cast<Ty>(_max / 2.0f * (time * time * time * time * time + 2.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InSine(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;

		return static_cast<Ty>(-_max * cos(time * ToRadian(90.0f) / totaltime) + _max + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutSine(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;

		return static_cast<Ty>(_max * sin(time * ToRadian(90.0f) / totaltime) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutSine(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;

		return static_cast<Ty>(-_max / 2.0f * (cos(time * Pai / totaltime) - 1) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InExp(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;

		return AdjEqual<Ty>(time, 0.0f) ?
			static_cast<Ty>(_min) :
			static_cast<Ty>(_max * std::pow(2.0f, 10.0f * (time / totaltime - 1.0f)) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutExp(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;

		return AdjEqual<Ty>(time, totaltime) ?
			static_cast<Ty>(_max + _min) :
			static_cast<Ty>(_max * (-std::pow(2.0f, -10.0f * time / totaltime) + 1.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutExp(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		if (AdjEqual<Ty>(time, 0.0f))
			return static_cast<Ty>(_min);

		if (AdjEqual<Ty>(time, totaltime))
			return static_cast<Ty>(_max);

		_max -= _min;
		time /= totaltime;

		if (time / 2.0f < 1.0f)
			return static_cast<Ty>(_max / 2.0f * std::pow(2.0f, 10.0f * (time - 1.0f)) + _min);

		--time;

		return static_cast<Ty>(_max / 2.0f * (-std::pow(2.0f, -10.0f * time) + 2.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InCirc(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;
		time = (std::max)((std::min)(time, 1.0f), -1.0f);


		return static_cast<Ty>(-_max * (std::pow(1.0f, -time * time) - 1.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutCirc(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= (totaltime - 1.0f);
		time = (std::max)((std::min)(time, 1.0f), -1.0f);


		return static_cast<Ty>(_max * std::pow(1.0f, -time * time) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutCirc(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;
		time = (std::max)((std::min)(time, 1.0f), -1.0f);


		if (time / 2.0f < 1.0f)
			return static_cast<Ty>(-_max / 2.0f * (std::pow(1.0f, -time * time) - 1.0f) + _min);

		time -= 2.0f;

		return static_cast<Ty>(_max / 2.0f * (std::pow(1.0f, -time * time) + 1.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InBack(float time, float totaltime, Ty back, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		return static_cast<Ty>(_max * time * time * ((back + 1.0f) * time - back) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutBack(float time, float totaltime, Ty back, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time = time / totaltime - 1.0f;

		return static_cast<Ty>(_max * (time * time * ((back + 1.0f) * time * back) + 1.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutBack(float time, float totaltime, Ty back, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		back *= static_cast<Ty>(1.525f);

		if (time / 2.0f < 1.0f)
			return static_cast<Ty>(_max * (time * time * ((back + 1.0f) * time - back)) + _min);

		time -= 2.0f;

		return static_cast<Ty>(_max / 2.0f * (time * time * ((back + 1.0f) * time + back) + 2.0f) + _min);
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty OutBounce(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		_max -= _min;
		time /= totaltime;

		if (time < 1.0f / 2.75f)
			return static_cast<Ty>(_max * (7.5625f * time * time) + _min);

		else if (time < 2.0f / 2.75f)
		{
			time -= 1.5f / 2.75f;

			return static_cast<Ty>(_max * (7.5625f * time * time + 0.75f) + _min);
		}
		else if (time < 2.5f / 2.75f)
		{
			time -= 2.25f / 2.75f;

			return static_cast<Ty>(_max * (7.5625f * time * time + 0.9375f) + _min);
		}
		else
		{
			time -= 2.625f / 2.75f;

			return static_cast<Ty>(_max * (7.5625f * time * time + 0.984375f) + _min);
		}
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InBounce(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		return _max - OutBounce<Ty>(totaltime - time, totaltime, _max - _min, static_cast<Ty>(0.0f)) + _min;
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty InOutBounce(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		if (time < totaltime / 2.0f)
		{
			return static_cast<Ty>(InBounce<Ty>(time * 2.0f, totaltime, _max - _min, _max) * 0.5f + _min);
		}
		else
		{
			return static_cast<Ty>(OutBounce<Ty>(time * 2.0f - totaltime, totaltime, _max - _min, static_cast<Ty>(0.0f)) * 0.5f + _min + (_max - _min) * 0.5f);
		}
	}

	template<typename Ty = float>
	[[nodiscard]] static inline Ty Linear(float time, float totaltime, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		return static_cast<Ty>((_max - _min) * time / totaltime + _min);
	}
};
