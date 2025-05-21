#pragma once

// ���ėp
#include "src/util/Utils.h"
#include "src/util/math/Vector2.h"
#include <vector>

/// <summary>
/// ��ʔz�u�̊�_
/// </summary>
/// <author> �~�b�` </author>
namespace Anchor {

	// �A���J�[�|�C���g�̊���z�u
	enum class Preset {
		None = -1,
		LeftUp,		// ����
		LeftMiddle,	// ����
		LeftDown,	// ����
		MiddleUp,	// ����
		Middle,		// ����
		MiddleDown,	// ����
		RightUp,	// �E��
		RightMiddle,// �E��
		RightDown,	// �E��
		Max
	};

	// �A���J�[�|�C���g�̔z�u���
	enum class Placement {
		None = -1,
		LEFT,	// ����
		MIDDLE,	// ������
		RIGHT,	// �E��
		UP,		// �㑤
		DOWN,	// ����
		Max
	};

	//================================================================================
	// ���e��֐�

	// ���������擾����
	inline const Placement PlacementHorizontal(Preset preset) {
		switch (static_cast<int>(preset) / 3) {
		case 0:		return Placement::LEFT;		break;
		case 1:		return Placement::MIDDLE;	break;
		case 2:		return Placement::RIGHT;	break;
		default:	return Placement::MIDDLE;	break;
		}
	}

	// ���������擾����
	inline const Placement PlacementVertical(Preset preset) {
		switch (static_cast<int>(preset) % 3) {
		case 0:		return Placement::UP;		break;
		case 1:		return Placement::MIDDLE;	break;
		case 2:		return Placement::DOWN;		break;
		default:	return Placement::MIDDLE;	break;
		}
	}

	// �������Ɛ��������擾����
	inline const std::vector<Placement> PlacementElement(Preset preset) {
		std::vector<Placement> placements;

		placements.push_back(PlacementHorizontal(preset));
		placements.push_back(PlacementVertical(preset));

		return placements;
	}

	// �A���J�[�|�C���g���Ƃ̊���W
	inline const Vector2 Point(Preset preset) {

		using namespace Screen;

		switch (preset) {
		case Anchor::Preset::LeftUp:		return Vector2(0.0f,		0.0f);			break;
		case Anchor::Preset::LeftMiddle:	return Vector2(0.0f,		HEIGHT_HALF);	break;
		case Anchor::Preset::LeftDown:		return Vector2(0.0f,		HEIGHT_F);		break;
		case Anchor::Preset::MiddleUp:		return Vector2(WIDTH_HALF,	0.0f);			break;
		case Anchor::Preset::Middle:		return Vector2(WIDTH_HALF,	HEIGHT_HALF);	break;
		case Anchor::Preset::MiddleDown:	return Vector2(WIDTH_HALF,	HEIGHT_F);		break;
		case Anchor::Preset::RightUp:		return Vector2(WIDTH_F,		0.0f);			break;
		case Anchor::Preset::RightMiddle:	return Vector2(WIDTH_F,		HEIGHT_HALF);	break;
		case Anchor::Preset::RightDown:		return Vector2(WIDTH_F,		HEIGHT_F);		break;
		default:
			return Vector2(0.0f, 0.0f);
			break;
		}

		return Vector2(0.0f, 0.0f);
	}
};
