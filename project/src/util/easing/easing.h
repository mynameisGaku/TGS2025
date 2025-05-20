// easingFunctions.h
#pragma once
#include <cmath>
#include "src/util/easing/EasingType.h"
#include "src/util/easing/EasingTypeMapper.h"
// ◇汎用
#include "src/util/time/GameTime.h"
#include "src/util/math/vector2.h"
#include "src/util/math/vector3.h"

namespace EasingFunc 
{

    inline float Apply(float x, float start, float end) 
    {
        return start + (end - start) * x;
    }

#define DEFINE_EASING(funcName, body) \
inline float funcName##Raw(float x) body \
inline float funcName(float t, float d, float end, float start) { \
    return Apply(funcName##Raw(t / d), start, end); \
}

    DEFINE_EASING(EaseInSine, { return 1.0f - cosf((x * 3.14159265f) / 2.0f); })
        DEFINE_EASING(EaseOutSine, { return sinf((x * 3.14159265f) / 2.0f); })
        DEFINE_EASING(EaseInOutSine, { return -0.5f * (cosf(3.14159265f * x) - 1.0f); })
        DEFINE_EASING(EaseInQuad, { return x * x; })
        DEFINE_EASING(EaseOutQuad, { return 1.0f - (1.0f - x) * (1.0f - x); })
        DEFINE_EASING(EaseInOutQuad, { return x < 0.5f ? 2.0f * x * x : 1.0f - powf(-2.0f * x + 2.0f, 2.0f) / 2.0f; })
        DEFINE_EASING(EaseInCubic, { return x * x * x; })
        DEFINE_EASING(EaseOutCubic, { return 1.0f - powf(1.0f - x, 3.0f); })
        DEFINE_EASING(EaseInOutCubic, { return x < 0.5f ? 4.0f * x * x * x : 1.0f - powf(-2.0f * x + 2.0f, 3.0f) / 2.0f; })
        DEFINE_EASING(EaseInQuart, { return x * x * x * x; })
        DEFINE_EASING(EaseOutQuart, { return 1.0f - powf(1.0f - x, 4.0f); })
        DEFINE_EASING(EaseInOutQuart, { return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - powf(-2.0f * x + 2.0f, 4.0f) / 2.0f; })
        DEFINE_EASING(EaseInQuint, { return x * x * x * x * x; })
        DEFINE_EASING(EaseOutQuint, { return 1.0f - powf(1.0f - x, 5.0f); })
        DEFINE_EASING(EaseInOutQuint, { return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - powf(-2.0f * x + 2.0f, 5.0f) / 2.0f; })
        DEFINE_EASING(EaseInExpo, { return x == 0.0f ? 0.0f : powf(2.0f, 10.0f * x - 10.0f); })
        DEFINE_EASING(EaseOutExpo, { return x == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * x); })
        DEFINE_EASING(EaseInOutExpo, {
            if (x == 0.0f) return 0.0f;
            if (x == 1.0f) return 1.0f;
            return x < 0.5f ? powf(2.0f, 20.0f * x - 10.0f) / 2.0f : (2.0f - powf(2.0f, -20.0f * x + 10.0f)) / 2.0f;
            })
        DEFINE_EASING(EaseInCirc, { return 1.0f - sqrtf(1.0f - x * x); })
        DEFINE_EASING(EaseOutCirc, { return sqrtf(1.0f - powf(x - 1.0f, 2.0f)); })
        DEFINE_EASING(EaseInOutCirc, {
            return x < 0.5f
                ? (1.0f - sqrtf(1.0f - 4.0f * x * x)) / 2.0f
                : (sqrtf(1.0f - powf(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
            })
        DEFINE_EASING(EaseInBack, {
            float c1 = 1.70158f;
            return c1 * x * x * x - c1 * x * x;
            })
        DEFINE_EASING(EaseOutBack, {
            float c1 = 1.70158f;
            float c3 = c1 + 1.0f;
            return 1.0f + c3 * powf(x - 1.0f, 3.0f) + c1 * powf(x - 1.0f, 2.0f);
            })
        DEFINE_EASING(EaseInOutBack, {
            float c1 = 1.70158f;
            float c2 = c1 * 1.525f;
            return x < 0.5f
                ? (powf(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f
                : (powf(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
            })
        DEFINE_EASING(EaseInElastic, {
            if (x == 0.0f) return 0.0f;
            if (x == 1.0f) return 1.0f;
            float c4 = (2.0f * 3.14159265f) / 3.0f;
            return -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * c4);
            })
        DEFINE_EASING(EaseOutElastic, {
            if (x == 0.0f) return 0.0f;
            if (x == 1.0f) return 1.0f;
            float c4 = (2.0f * 3.14159265f) / 3.0f;
            return powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
            })
        DEFINE_EASING(EaseInOutElastic, {
            if (x == 0.0f) return 0.0f;
            if (x == 1.0f) return 1.0f;
            float c5 = (2.0f * 3.14159265f) / 4.5f;
            return x < 0.5f
                ? -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f
                : (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
            })
        DEFINE_EASING(EaseOutBounce, {
            if (x < 1.0f / 2.75f) {
                return 7.5625f * x * x;
            }
         else if (x < 2.0f / 2.75f) {
          x -= 1.5f / 2.75f;
          return 7.5625f * x * x + 0.75f;
      }
   else if (x < 2.5f / 2.75f) {
    x -= 2.25f / 2.75f;
    return 7.5625f * x * x + 0.9375f;
}
else {
 x -= 2.625f / 2.75f;
 return 7.5625f * x * x + 0.984375f;
}
            })
        DEFINE_EASING(EaseInBounce, { return 1.0f - EaseOutBounceRaw(1.0f - x); })
        DEFINE_EASING(EaseInOutBounce, {
            return x < 0.5f
                ? (1.0f - EaseOutBounceRaw(1.0f - 2.0f * x)) / 2.0f
                : (1.0f + EaseOutBounceRaw(2.0f * x - 1.0f)) / 2.0f;
            })

#undef DEFINE_EASING

} // namespace EasingFunc



namespace EasingUtils {

	struct EasingInfo {
		float time;								// 効果時間
		float totaltime;						// 効果時間
		bool isActive;							// 機能しているか
		GameTime::AdditionMethod additionMethod;	// 時間加算方法
		EasingType type;						// 補間種類

		EasingInfo() :
			time(0.0f),
			totaltime(1.0f),
			isActive(false),
			additionMethod(GameTime::AdditionMethod::Rate),
			type(EasingType::Linear) {
		};

		EasingInfo(float _time, float _totalTime, bool active, GameTime::AdditionMethod _additionMethod, EasingType _type) :
			time(_time),
			totaltime(_totalTime),
			isActive(active),
			additionMethod(_additionMethod),
			type(_type) {
		};
	};


} // namespace EasingUtils

using namespace EasingUtils;

namespace EasingFunc {

	template<typename Ty = float>
	[[nodiscard]] static inline void Process(EasingInfo* info, Ty* value, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		if (info->isActive == false)
			return;

		switch (info->additionMethod) {
		case GameTime::AdditionMethod::Usual:		info->time += GTime.deltaTime;			break;
		case GameTime::AdditionMethod::Rate:		info->time += GTime.deltaTime;			break;
		default:
			break;
		}

		if (info->time >= info->totaltime) {
			info->time = info->totaltime;
			info->isActive = false;

			if (info->totaltime == 0.0f) {
				*value = _max;
				return;
			}
		}

		switch (info->type) {
		case EasingType::None:	break;
		case EasingType::InQuad:		*value = EasingFunc::EaseInQuad(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutQuad:		*value = EasingFunc::EaseOutQuad(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutQuad:		*value = EasingFunc::EaseInOutQuad(info->time, info->totaltime, _max, _min); break;
		case EasingType::InCubic:		*value = EasingFunc::EaseInCubic(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutCubic:		*value = EasingFunc::EaseOutCubic(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutCubic:	*value = EasingFunc::EaseInOutCubic(info->time, info->totaltime, _max, _min); break;
		case EasingType::InQuart:		*value = EasingFunc::EaseInQuart(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutQuart:		*value = EasingFunc::EaseOutQuart(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutQuart:	*value = EasingFunc::EaseInOutQuart(info->time, info->totaltime, _max, _min); break;
		case EasingType::InQuint:		*value = EasingFunc::EaseInQuint(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutQuint:		*value = EasingFunc::EaseOutQuint(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutQuint:	*value = EasingFunc::EaseInOutQuint(info->time, info->totaltime, _max, _min); break;
		case EasingType::InSine:		*value = EasingFunc::EaseInSine(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutSine:		*value = EasingFunc::EaseOutSine(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutSine:		*value = EasingFunc::EaseInOutSine(info->time, info->totaltime, _max, _min); break;
		case EasingType::InCirc:		*value = EasingFunc::EaseInCirc(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutCirc:		*value = EasingFunc::EaseOutCirc(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutCirc:		*value = EasingFunc::EaseInOutCirc(info->time, info->totaltime, _max, _min); break;
		case EasingType::InBounce:		*value = EasingFunc::EaseInBounce(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutBounce:		*value = EasingFunc::EaseOutBounce(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutBounce:	*value = EasingFunc::EaseInOutBounce(info->time, info->totaltime, _max, _min); break;
		case EasingType::Linear:
		default:
			*value = EasingFunc::Apply(info->time, _max, _min);
			break;
		}
	}
} // namespace EasingFunc

namespace EasingUtils {

	struct EasingInt {
		EasingInfo info;
		int current;
		int begin;
		int end;

		EasingInt() : info(EasingInfo()), current(0), begin(0), end(0) {};
		EasingInt(EasingInfo _info, int _current, int _begin, int _end) : info(_info), current(_current), begin(_begin), end(_end) {};

		void Update() {
			if (info.isActive)
				EasingFunc::Process(&this->info, &this->current, this->end, this->begin);
		}

		/// <summary>
		/// イージング挙動を設定する
		/// </summary>
		/// <param name="begin">始点</param>
		/// <param name="end">終点</param>
		/// <param name="sec">効果時間</param>
		/// <param name="type">イージング種類</param>
		/// <param name="active">設定後に起動するか</param>
		void SetEasing(int _begin, int _end, float _sec, EasingType _type, bool active) {
			current = _begin;
			begin = _begin;
			end = _end;
			info.time = 0.0f;
			info.totaltime = _sec;
			info.type = _type;
			info.isActive = active;
		}
	};

	struct EasingDouble {
		EasingInfo info;
		double current;
		double begin;
		double end;

		EasingDouble() : info(EasingInfo()), current(0.0), begin(0.0), end(0.0) {};
		EasingDouble(EasingInfo _info, double _current, double _begin, double _end) : info(_info), current(_current), begin(_begin), end(_end) {};

		void Update() {
			if (info.isActive)
				EasingFunc::Process(&this->info, &this->current, this->end, this->begin);
		}

		/// <summary>
		/// イージング挙動を設定する
		/// </summary>
		/// <param name="begin">始点</param>
		/// <param name="end">終点</param>
		/// <param name="sec">効果時間</param>
		/// <param name="type">イージング種類</param>
		/// <param name="active">設定後に起動するか</param>
		void SetEasing(double _begin, double _end, float _sec, EasingType _type, bool active) {
			current = _begin;
			begin = _begin;
			end = _end;
			info.time = 0.0f;
			info.totaltime = _sec;
			info.type = _type;
			info.isActive = active;
		}
	};

	struct EasingFloat {
		EasingInfo info;
		float current;
		float begin;
		float end;

		EasingFloat() : info(EasingInfo()), current(0.0f), begin(0.0f), end(0.0f) {};
		EasingFloat(EasingInfo _info, float _current, float _begin, float _end) : info(_info), current(_current), begin(_begin), end(_end) {};

		void Update() {
			if (info.isActive)
				EasingFunc::Process(&this->info, &this->current, this->end, this->begin);
		}

		/// <summary>
		/// イージング挙動を設定する
		/// </summary>
		/// <param name="begin">始点</param>
		/// <param name="end">終点</param>
		/// <param name="sec">効果時間</param>
		/// <param name="type">イージング種類</param>
		/// <param name="active">設定後に起動するか</param>
		void SetEasing(float _begin, float _end, float _sec, EasingType _type, bool active) {
			current = _begin;
			begin = _begin;
			end = _end;
			info.time = 0.0f;
			info.totaltime = _sec;
			info.type = _type;
			info.isActive = active;
		}
	};

	struct EasingVec2 {
		EasingInfo info;
		Vector2 current;
		Vector2 begin;
		Vector2 end;

		EasingVec2() : info(EasingInfo()), current(Vector2::Zero), begin(Vector2::Zero), end(Vector2::Zero) {};
		EasingVec2(EasingInfo _info, Vector2 _current, Vector2 _begin, Vector2 _end) : info(_info), current(_current), begin(_begin), end(_end) {};

		void Update() {
			if (info.isActive)
				EasingFunc::Process(&this->info, &this->current, this->end, this->begin);
		}

		/// <summary>
		/// イージング挙動を設定する
		/// </summary>
		/// <param name="begin">始点</param>
		/// <param name="end">終点</param>
		/// <param name="sec">効果時間</param>
		/// <param name="type">イージング種類</param>
		/// <param name="active">設定後に起動するか</param>
		void SetEasing(Vector2 _begin, Vector2 _end, float _sec, EasingType _type, bool active) {
			current = _begin;
			begin = _begin;
			end = _end;
			info.time = 0.0f;
			info.totaltime = _sec;
			info.type = _type;
			info.isActive = active;
		}
	};

	struct EasingVec3 {
		EasingInfo info;
		Vector3 current;
		Vector3 begin;
		Vector3 end;

		EasingVec3() : info(EasingInfo()), current(Vector3::Zero), begin(Vector3::Zero), end(Vector3::Zero) {};
		EasingVec3(EasingInfo _info, Vector3 _current, Vector3 _begin, Vector3 _end) : info(_info), current(_current), begin(_begin), end(_end) {};

		void Update() {
			if (info.isActive)
				EasingFunc::Process(&this->info, &this->current, this->end, this->begin);
		}

		/// <summary>
		/// イージング挙動を設定する
		/// </summary>
		/// <param name="begin">始点</param>
		/// <param name="end">終点</param>
		/// <param name="sec">効果時間</param>
		/// <param name="type">イージング種類</param>
		/// <param name="active">設定後に起動するか</param>
		void SetEasing(Vector3 _begin, Vector3 _end, float _sec, EasingType _type, bool active) {
			current = _begin;
			begin = _begin;
			end = _end;
			info.time = 0.0f;
			info.totaltime = _sec;
			info.type = _type;
			info.isActive = active;
		}
	};
} // namespace EasingUtils
