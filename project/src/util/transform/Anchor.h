#pragma once

// ◇汎用
#include "src/util/math/Vector2.h"
#include "src/common/setting/window/WindowSetting.h"
#include <vector>

/// <summary>
/// 画面配置の基準点
/// </summary>
/// <author> ミッチ </author>
namespace Anchor {

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
	inline const Placement PlacementHorizontal(Preset preset) {
		switch (static_cast<int>(preset) / 3) {
		case 0:		return Placement::LEFT;		break;
		case 1:		return Placement::MIDDLE;	break;
		case 2:		return Placement::RIGHT;	break;
		default:	return Placement::MIDDLE;	break;
		}
	}

	// 垂直情報を取得する
	inline const Placement PlacementVertical(Preset preset) {
		switch (static_cast<int>(preset) % 3) {
		case 0:		return Placement::UP;		break;
		case 1:		return Placement::MIDDLE;	break;
		case 2:		return Placement::DOWN;		break;
		default:	return Placement::MIDDLE;	break;
		}
	}

	// 水平情報と垂直情報を取得する
	inline const std::vector<Placement> PlacementElement(Preset preset) {
		std::vector<Placement> placements;

		placements.push_back(PlacementHorizontal(preset));
		placements.push_back(PlacementVertical(preset));

		return placements;
	}

	// アンカーポイントごとの基準座標
	inline const Vector2 Point(Preset preset) {

		switch (preset) {
		case Anchor::Preset::LeftUp:		return Vector2(0.0f,								0.0f);								break;
		case Anchor::Preset::LeftMiddle:	return Vector2(0.0f,								WindowSetting::Inst().height_half);	break;
		case Anchor::Preset::LeftDown:		return Vector2(0.0f,								WindowSetting::Inst().height);		break;
		case Anchor::Preset::MiddleUp:		return Vector2(WindowSetting::Inst().width_half,	0.0f);								break;
		case Anchor::Preset::Middle:		return Vector2(WindowSetting::Inst().width_half,	WindowSetting::Inst().height_half);	break;
		case Anchor::Preset::MiddleDown:	return Vector2(WindowSetting::Inst().width_half,	WindowSetting::Inst().height);		break;
		case Anchor::Preset::RightUp:		return Vector2(WindowSetting::Inst().width,			0.0f);								break;
		case Anchor::Preset::RightMiddle:	return Vector2(WindowSetting::Inst().width,			WindowSetting::Inst().height_half);	break;
		case Anchor::Preset::RightDown:		return Vector2(WindowSetting::Inst().width,			WindowSetting::Inst().height);		break;
		default:
			return Vector2(0.0f, 0.0f);
			break;
		}

		return Vector2(0.0f, 0.0f);
	}
};
