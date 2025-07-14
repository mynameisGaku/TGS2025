#pragma once

// ���ėp
#include "src/util/math/Vector2.h"
#include "src/common/setting/window/WindowSetting.h"
#include <vector>

/// <summary>
/// ��ʔz�u�̊�_
/// </summary>
/// <author> �~�b�` </author>
class Anchor {
public:
	Anchor() :
		begin(Vector2(0.0f, 0.0f)),
		end(Vector2(WindowSetting::Inst().width, WindowSetting::Inst().height)),
		preset(Preset::None)
	{
	}

	~Anchor() = default;

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
	static inline const Placement PlacementHorizontal(Preset preset) {
		switch (static_cast<int>(preset) / 3) {
		case 0:		return Placement::LEFT;		break;
		case 1:		return Placement::MIDDLE;	break;
		case 2:		return Placement::RIGHT;	break;
		default:	return Placement::None;	break;
		}
	}

	// ���������擾����
	static inline const Placement PlacementVertical(Preset preset) {
		switch (static_cast<int>(preset) % 3) {
		case 0:		return Placement::UP;		break;
		case 1:		return Placement::MIDDLE;	break;
		case 2:		return Placement::DOWN;		break;
		default:	return Placement::None;	break;
		}
	}

	// �������Ɛ��������擾����
	static inline const std::vector<Placement> PlacementElement(Preset preset) {
		std::vector<Placement> placements;

		placements.push_back(PlacementHorizontal(preset));
		placements.push_back(PlacementVertical(preset));

		return placements;
	}

	// �A���J�[�|�C���g���Ƃ̊���W
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
	Vector2 begin;	// �A���J�[�|�C���g�̍��W
	Vector2 end;	// �A���J�[�|�C���g�̍��W
	Preset preset;	// �A���J�[�|�C���g�̔z�u

};
