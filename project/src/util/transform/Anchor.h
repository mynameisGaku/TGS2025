#pragma once

// ◇汎用
#include "src/util/math/Vector2.h"
#include "src/common/setting/window/WindowSetting.h"
#include <vector>

/// <summary>
/// 画面配置の基準点
/// </summary>
/// <author> ミッチ </author>
class Anchor {
public:
	Anchor() :
		begin(Vector2(0.0f, 0.0f)),
		end(Vector2(WindowSetting::Inst().width, WindowSetting::Inst().height)),
		preset(Preset::None)
	{
	}

	~Anchor() = default;

	// アンカーポイントの既定配置
	enum class Preset {
		None = -1,
		LeftUp,		// 左上
		LeftMiddle,	// 左中
		LeftDown,	// 左下
		MiddleUp,	// 中上
		Middle,		// 中央
		MiddleDown,	// 中下
		RightUp,	// 右上
		RightMiddle,// 右中
		RightDown,	// 右下
		Max
	};

	// アンカーポイントの配置情報
	enum class Placement {
		None = -1,
		LEFT,	// 左側
		MIDDLE,	// 中央側
		RIGHT,	// 右側
		UP,		// 上側
		DOWN,	// 下側
		Max
	};

	//================================================================================
	// ▼各種関数

	// 水平情報を取得する
	static inline const Placement PlacementHorizontal(Preset preset) {
		switch (static_cast<int>(preset) / 3) {
		case 0:		return Placement::LEFT;		break;
		case 1:		return Placement::MIDDLE;	break;
		case 2:		return Placement::RIGHT;	break;
		default:	return Placement::None;	break;
		}
	}

	// 垂直情報を取得する
	static inline const Placement PlacementVertical(Preset preset) {
		switch (static_cast<int>(preset) % 3) {
		case 0:		return Placement::UP;		break;
		case 1:		return Placement::MIDDLE;	break;
		case 2:		return Placement::DOWN;		break;
		default:	return Placement::None;	break;
		}
	}

	// 水平情報と垂直情報を取得する
	static inline const std::vector<Placement> PlacementElement(Preset preset) {
		std::vector<Placement> placements;

		placements.push_back(PlacementHorizontal(preset));
		placements.push_back(PlacementVertical(preset));

		return placements;
	}

	// アンカーポイントごとの基準座標
	inline Vector2 Point() const {

		switch (preset) {
		case Preset::LeftUp:		return Vector2(begin.x,	begin.y);					break;
		case Preset::LeftMiddle:	return Vector2(begin.x,(begin.y + end.y) * 0.5f);	break;
		case Preset::LeftDown:		return Vector2(begin.x,	end.y);						break;

		case Preset::MiddleUp:		return Vector2((begin.x + end.x) * 0.5f,begin.y);					break;
		case Preset::Middle:		return Vector2((begin.x + end.x) * 0.5f,(begin.y + end.y) * 0.5f);	break;
		case Preset::MiddleDown:	return Vector2((begin.x + end.x) * 0.5f,end.y);						break;
		
		case Preset::RightUp:		return Vector2(end.x,	begin.y);					break;
		case Preset::RightMiddle:	return Vector2(end.x,  (begin.y + end.y) * 0.5f);	break;
		case Preset::RightDown:		return Vector2(end.x,	end.y);						break;
		default:
			return Vector2(0.0f, 0.0f);
			break;
		}

		return Vector2(0.0f, 0.0f);
	}

	void SetBegin(const Vector2& begin) { this->begin = begin; }
	void SetEnd(const Vector2& end) { this->end = end; }
	void SetPreset(Preset preset) {
		this->preset = preset;
	}

	Preset GetPreset() const { return preset; }

private:
	Vector2 begin;	// アンカーポイントの座標
	Vector2 end;	// アンカーポイントの座標
	Preset preset;	// アンカーポイントの配置

};
