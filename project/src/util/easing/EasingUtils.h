#pragma once
#include "easing.h"

// ◇汎用
#include "../Library/time.h"
#include "Util/Vector2.h"
#include "Util/Vector3.h"

namespace EasingUtils {

	struct EasingInfo {
		float time;								// 効果時間
		float totaltime;						// 効果時間
		bool isActive;							// 機能しているか
		Time::AdditionMethod additionMethod;	// 時間加算方法
		EasingType type;						// 補間種類

		EasingInfo() :
			time(0.0f),
			totaltime(1.0f),
			isActive(false),
			additionMethod(Time::AdditionMethod::Rate),
			type(EasingType::Linear) {
		};

		EasingInfo(float _time, float _totalTime, bool active, Time::AdditionMethod _additionMethod, EasingType _type) :
			time(_time),
			totaltime(_totalTime),
			isActive(active),
			additionMethod(_additionMethod),
			type(_type) {
		};
	};


}

using namespace EasingUtils;

namespace EasingFunc {

	template<typename Ty = float>
	[[nodiscard]] static inline void Process(EasingInfo* info, Ty* value, Ty _max = 1.0f, Ty _min = 0.0f)
	{
		if (info->isActive == false)
			return;

		switch (info->additionMethod) {
		case Time::AdditionMethod::Usual:		info->time += Time::DeltaTime();			break;
		case Time::AdditionMethod::Rate:		info->time += Time::DeltaTimeLapseRate();	break;
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
		case EasingType::InQuad:		*value = EasingFunc::InQuad(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutQuad:		*value = EasingFunc::OutQuad(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutQuad:		*value = EasingFunc::InOutQuad(info->time, info->totaltime, _max, _min); break;
		case EasingType::InCubic:		*value = EasingFunc::InCubic(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutCubic:		*value = EasingFunc::OutCubic(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutCubic:	*value = EasingFunc::InOutCubic(info->time, info->totaltime, _max, _min); break;
		case EasingType::InQuart:		*value = EasingFunc::InQuart(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutQuart:		*value = EasingFunc::OutQuart(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutQuart:	*value = EasingFunc::InOutQuart(info->time, info->totaltime, _max, _min); break;
		case EasingType::InQuint:		*value = EasingFunc::InQuint(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutQuint:		*value = EasingFunc::OutQuint(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutQuint:	*value = EasingFunc::InOutQuint(info->time, info->totaltime, _max, _min); break;
		case EasingType::InSine:		*value = EasingFunc::InSine(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutSine:		*value = EasingFunc::OutSine(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutSine:		*value = EasingFunc::InOutSine(info->time, info->totaltime, _max, _min); break;
		case EasingType::InCirc:		*value = EasingFunc::InCirc(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutCirc:		*value = EasingFunc::OutCirc(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutCirc:		*value = EasingFunc::InOutCirc(info->time, info->totaltime, _max, _min); break;
		case EasingType::InBounce:		*value = EasingFunc::InBounce(info->time, info->totaltime, _max, _min); break;
		case EasingType::OutBounce:		*value = EasingFunc::OutBounce(info->time, info->totaltime, _max, _min); break;
		case EasingType::InOutBounce:	*value = EasingFunc::InOutBounce(info->time, info->totaltime, _max, _min); break;
		case EasingType::Linear:
		default:
			*value = EasingFunc::Linear(info->time, info->totaltime, _max, _min);
			break;
		}
	}
}

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

		EasingVec2() : info(EasingInfo()), current(V2::ZERO), begin(V2::ZERO), end(V2::ZERO) {};
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
}