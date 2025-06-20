#pragma once
//gauge.h

#include <DxLib.h>
#include <unordered_map>
#include "src/util/math/Vector2.h"

//  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .----------------.  .-----------------.  .----------------.
// | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |
// | |    ______    | || |      __      | || | _____  _____ | || |    ______    | || |  _________   | || |  _________   | || | _____  _____ | || | ____  _____  | || |     ______   | |
// | |  .' ___  |   | || |     /  \     | || ||_   _||_   _|| || |  .' ___  |   | || | |_   ___  |  | || | |_   ___  |  | || ||_   _||_   _|| || ||_   \|_   _| | || |   .' ___  |  | |
// | | / .'   \_|   | || |    / /\ \    | || |  | |    | |  | || | / .'   \_|   | || |   | |_  \_|  | || |   | |_  \_|  | || |  | |    | |  | || |  |   \ | |   | || |  / .'   \_|  | |
// | | | |    ____  | || |   / ____ \   | || |  | '    ' |  | || | | |    ____  | || |   |  _|  _   | || |   |  _|      | || |  | '    ' |  | || |  | |\ \| |   | || |  | |         | |
// | | \ `.___]  _| | || | _/ /    \ \_ | || |   \ `--' /   | || | \ `.___]  _| | || |  _| |___/ |  | || |  _| |_       | || |   \ `--' /   | || | _| |_\   |_  | || |  \ `.___.'\  | |
// | |  `._____.'   | || ||____|  |____|| || |    `.__.'    | || |  `._____.'   | || | |_________|  | || | |_____|      | || |    `.__.'    | || ||_____|\____| | || |   `._____.'  | |
// | |              | || |              | || |              | || |              | || |              | || |              | || |              | || |              | || |              | |
// | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |
//  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'  '----------------'


///  ______
/// |�g�p��|
///  �P�P�P
/// class A
/// {
/// public:
///		Gauge healthGauge;
///		Gauge expGauge;
///		Status status;
/// 
///		void Draw()
///		{
///			healthGauge.DrawGaugeF(50, 50, 200, 20, status->CurrentHealth(), status->MaxHealth(), 0, GetColor(255, 0, 0), GetColor(128, 0, 0), GetColor(0, 255, 0), 0.5f, GetColor(0, 0, 0), 2.0f);
///			expGauge.DrawGaugeF(50, 50, 200, 20, status->CurrentExp(), status->NextExp(), 0, GetColor(255, 0, 0), GetColor(128, 0, 0), GetColor(0, 255, 0), 0.5f, GetColor(0, 0, 0), 2.0f);
///		}
/// }

//�F�l�̃Q�[�W�N���X���Q�l�ɂ��č��܂���

/// <summary>
/// �Q�[�W�N���X
/// ��̎g�p��̂悤�ɂP�Q�[�W�ɂ����̃N���X�P�g���܂��B
/// </summary>
/// <author> G.F </author>
class Gauge {
public:
	Gauge();

	// �X�N���[���̌���
	enum class ScrollType {
		eUp,	// �ォ�牺
		eDown,	// �������
		eLeft,	// ������E
		eRight,	// �E���獶
	};

	/// <summary>
	/// �P�F�Q�[�W�`�� ���㌴�_
	/// </summary>
	/// <param name="pos">�`��J�n�ʒu��Vector2���W</param>
	/// <param name="size">�Q�[�W�̑傫��</param>
	/// <param name="current">���ݒl</param>
	/// <param name="max">�ő�l</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="gaugeColor">�Q�[�W�̐F</param>
	/// <param name="delayColor">�������x���Q�[�W�̐F</param>
	/// <param name="increaseColor">�������x���Q�[�W�̐F</param>
	/// <param name="backColor">�w�i�F</param>
	/// <param name="scrollSpeed">�x���Q�[�W�̑��x</param>
	/// <param name="thickness">�g���̌���</param>
	template<typename Ty = float>
	[[nodiscard]] inline void DrawGauge(Vector2 pos, Vector2 size, Ty current, Ty max, Ty min, int gaugeColor, int delayColor, int increaseColor, int backColor,
		float scrollSpeed, float thickness = 0.0f, int innerEdgeColor = GetColor(0, 0, 0), int outerEdgeColor = GetColor(255, 255, 255)) {

		// �w�i��`��
		{
			DrawBoxAA(pos.x - thickness * 2,pos.y - thickness * 2,	pos.x + size.x + thickness * 2, pos.y + size.y + thickness * 2, outerEdgeColor, TRUE);
			DrawBoxAA(pos.x - thickness,	pos.y - thickness,		pos.x + size.x + thickness,		pos.y + size.y + thickness,		innerEdgeColor, TRUE);
			DrawBoxAA(pos.x, pos.y, pos.x + size.x, pos.y + size.y, backColor, TRUE, thickness * 2.0f);
		}

		// ����Ăяo�����ɒx���l��ݒ�
		if (delayValuesF.find(id) == delayValuesF.end())
			delayValuesF[id] = current;

		Gauge::UpdateDelayValue(delayValuesF[id], current, scrollSpeed);

		// ���ݒl�ƒx���l��͈͓��Ɏ��߂�
		current = (current < min) ? min : (current > max) ? max : current;
		delayValuesF[id] = (delayValuesF[id] < min) ? min : (delayValuesF[id] > max) ? max : delayValuesF[id];

		// �Q�[�W�̕����v�Z
		float range = max - min;
		
		float percentage = (current - min) / range;
		Vector2 gaugeSize = size * percentage;

		float delayPercentage = (delayValuesF[id] - min) / range;
		Vector2 delayGaugeSize = size * delayPercentage;

		// �Q�[�W��`��
		if (current < delayValuesF[id])
		{

			DrawBoxAA(pos.x, pos.y, pos.x + delayGaugeSize.x,	pos.y + size.y, delayColor, TRUE);
			DrawBoxAA(pos.x, pos.y, pos.x + gaugeSize.x,		pos.y + size.y, gaugeColor, TRUE);
		}
		else
		{
			DrawBoxAA(pos.x, pos.y, pos.x + gaugeSize.x,		pos.y + size.y, increaseColor,	TRUE);
			DrawBoxAA(pos.x, pos.y, pos.x + delayGaugeSize.x,	pos.y + size.y, gaugeColor,		TRUE);
		}
	}

	/// <summary>
	/// �摜�Q�[�W�`�� ���㌴�_
	/// </summary>
	/// <param name="pos">�`��J�n�ʒu��Vector2���W</param>
	/// <param name="current">���ݒl</param>
	/// <param name="max">�ő�l</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="graph">�Q�[�W�̉摜</param>
	/// <param name="delayGraph">�������x���Q�[�W�̉摜</param>
	/// <param name="increaseGraph">�������x���Q�[�W�̉摜</param>
	/// <param name="backGraph">�w�i�摜</param>
	/// <param name="scrollSpeed">�x���Q�[�W�̑��x</param>
	/// <param name="scrollType">�X�N���[���������</param>
	/// <param name="scale">�摜�̊g�嗦</param>
	template<typename Ty = float>
	[[nodiscard]] inline void DrawGraphGauge(Vector2 pos, Ty current, Ty max, Ty min, int graph, int delayGraph, int increaseGraph, int backGraph, float scrollSpeed, Vector2 scale = Vector2::Ones) {

		int srcWidth, srcHeight;
		GetGraphSize(backGraph, &srcWidth, &srcHeight);

		int width = static_cast<int>(srcWidth * scale.x);
		int height = static_cast<int>(srcHeight * scale.y);

		// �w�i��`��
		DrawRectExtendGraphF(pos.x, pos.y, pos.x + width, pos.y + height, 0, 0, srcWidth, srcHeight, backGraph, TRUE);

		// ����Ăяo�����ɒx���l��ݒ�
		if (delayValuesF.find(id) == delayValuesF.end())
			delayValuesF[id] = current;

		Gauge::UpdateDelayValue(delayValuesF[id], current, scrollSpeed);

		// ���ݒl�ƒx���l��͈͓��Ɏ��߂�
		current = (current < min) ? min : (current > max) ? max : current;
		delayValuesF[id] = (delayValuesF[id] < min) ? min : (delayValuesF[id] > max) ? max : delayValuesF[id];

		// �Q�[�W�̕����v�Z
		float range = max - min;
		float percentage = (current - min) / range;
		float gaugeWidth = width * percentage;
		float gaugeHeight = height * percentage;

		float delayPercentage = (delayValuesF[id] - min) / range;
		float delayGaugeWidth = width * delayPercentage;
		float delayGaugeHeight = height * delayPercentage;

		// �Q�[�W��`��
		if (current < delayValuesF[id])
		{
			DrawRectExtendGraphF(pos.x, pos.y, pos.x + delayGaugeWidth, pos.y + height, 0, 0, static_cast<int>(srcWidth * delayPercentage), srcHeight, delayGraph == -1 ? graph : delayGraph, TRUE);
			DrawRectExtendGraphF(pos.x, pos.y, pos.x + gaugeWidth,		pos.y + height, 0, 0, static_cast<int>(srcWidth * percentage), srcHeight, graph, TRUE);
		}
		else
		{
			DrawRectExtendGraphF(pos.x, pos.y, pos.x + gaugeWidth, pos.y + srcHeight, 0, 0, static_cast<int>(srcWidth * percentage), srcHeight, increaseGraph == -1 ? graph : increaseGraph, TRUE);
			DrawRectExtendGraphF(pos.x, pos.y, pos.x + delayGaugeWidth, pos.y + srcHeight, 0, 0, static_cast<int>(srcWidth * delayPercentage), srcHeight, graph, TRUE);
		}
	}

	/// <summary>
	/// �摜�Q�[�W�`�� ���S���_
	/// </summary>
	/// <param name="pos">�`��J�n�ʒu��Vector2���W</param>
	/// <param name="current">���ݒl</param>
	/// <param name="max">�ő�l</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="graph">�Q�[�W�̉摜</param>
	/// <param name="delayGraph">�������x���Q�[�W�̉摜</param>
	/// <param name="increaseGraph">�������x���Q�[�W�̉摜</param>
	/// <param name="backGraph">�w�i�摜</param>
	/// <param name="scrollSpeed">�x���Q�[�W�̑��x</param>
	/// <param name="scrollType">�X�N���[���������</param>
	/// <param name="scale">�摜�̊g�嗦</param>
	/// <param name="angle">�摜�̉�]</param>
	template<typename Ty = float>
	[[nodiscard]] inline void DrawRectRotaGraphGauge(const Vector2& pos, Ty current, Ty max, Ty min, int graph, int delayGraph, int increaseGraph, int backGraph, float scrollSpeed, const Vector2& scale = Vector2::Ones, float angle = 0.0f, ScrollType scroll = ScrollType::eRight) {

		int srcWidth, srcHeight;
		GetGraphSize(graph, &srcWidth, &srcHeight);

		int width = static_cast<int>(srcWidth * scale.x);
		int height = static_cast<int>(srcHeight * scale.y);
		float scl = (scale.x + scale.y) / 2;

		// �w�i��`��
		if (backGraph != -1)
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, width, height, scl, 0, backGraph, TRUE);

		// ����Ăяo�����ɒx���l��ݒ�
		if (delayValuesF.find(id) == delayValuesF.end())
			delayValuesF[id] = current;

		Gauge::UpdateDelayValue(delayValuesF[id], current, scrollSpeed);

		// ���ݒl�ƒx���l��͈͓��Ɏ��߂�
		current = (current < min) ? min : (current > max) ? max : current;
		delayValuesF[id] = (delayValuesF[id] < min) ? min : (delayValuesF[id] > max) ? max : delayValuesF[id];

		// �Q�[�W�̕����v�Z
		float range = max - min;
		float percentage = (current - min) / range;
		float gaugeWidth = width * percentage;
		float gaugeHeight = height * percentage;

		float delayPercentage = (delayValuesF[id] - min) / range;
		float delayGaugeWidth = width * delayPercentage;
		float delayGaugeHeight = height * delayPercentage;

		// �Q�[�W��`��
		if (current < delayValuesF[id])
		{
			switch (scroll) {
			case Gauge::ScrollType::eUp:
				DrawRectRotaGraphF(pos.x, pos.y + delayGaugeHeight / 2, 0, (int)(delayGaugeHeight / scl),	srcWidth, srcHeight, scl, 0.0f, delayGraph == -1 ? graph : delayGraph, TRUE);
				DrawRectRotaGraphF(pos.x, pos.y + gaugeHeight / 2,		0, (int)(gaugeHeight / scl),		srcWidth, srcHeight, scl, 0.0f, graph, TRUE);
				break;
			case Gauge::ScrollType::eDown:
				break;
			case Gauge::ScrollType::eLeft:
				break;
			case Gauge::ScrollType::eRight:
				DrawRectRotaGraphF(pos.x, pos.y, static_cast<int>(delayGaugeWidth), 0, srcWidth, srcHeight, scl, angle, delayGraph == -1 ? graph : delayGraph, TRUE);
				DrawRectRotaGraphF(pos.x, pos.y, static_cast<int>(gaugeWidth),		0, srcWidth, srcHeight, scl, angle, graph, TRUE);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (scroll) {
			case Gauge::ScrollType::eUp:
				DrawRectRotaGraphF(pos.x, pos.y + delayGaugeHeight / 2, 0,(int)(delayGaugeHeight / scl),srcWidth, srcHeight, scl, 0.0f, increaseGraph == -1 ? graph : increaseGraph, TRUE);
				DrawRectRotaGraphF(pos.x, pos.y + gaugeHeight / 2,		0,(int)(gaugeHeight / scl),		srcWidth, srcHeight, scl, 0.0f, graph, TRUE);
				break;
			case Gauge::ScrollType::eDown:
				break;
			case Gauge::ScrollType::eLeft:
				break;
			case Gauge::ScrollType::eRight:
				DrawRectRotaGraphF(pos.x, pos.y, static_cast<int>(delayGaugeWidth), 0, srcWidth, srcHeight, scl, angle, increaseGraph == -1 ? graph : increaseGraph, TRUE);
				DrawRectRotaGraphF(pos.x, pos.y, static_cast<int>(gaugeWidth),		0, srcWidth, srcHeight, scl, angle, graph, TRUE);
				break;
			default:
				break;
			}
		}
	}

private:
	// ���݂̒l�Ɍ������Ēx�����đ�������Q�[�W����邽�߁AID�ŊǗ����Ă��܂��B
	int id;
	static int GenerateId();
	std::unordered_map<int, float> delayValuesF;
	std::unordered_map<int, int> delayValues;
	static int currentId;

	template<typename Ty = float>
	[[nodiscard]] inline void UpdateDelayValue(Ty& delayValue, Ty current, float scrollSpeed) {

		Ty diff = current - delayValue;  // ���ݒl�Ƃ̍����v�Z
		delayValue += diff * scrollSpeed;    // �x���l���X�V
	}
};
