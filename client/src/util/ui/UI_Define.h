#pragma once
#include "framework/myDxLib.h"

namespace UI_Define {

	struct SliderColor {
		int gaugeColor;		// �Q�[�W�̐F
		int delayColor;		// �������̐F
		int increaseColor;	// �������̐F
		int backColor;		// �w�i�F
		int pointColor;		// �܂݂̐F

		SliderColor() : SliderColor(GetColor(0, 255, 0), GetColor(255, 0, 0), GetColor(240, 255, 240), GetColor(40, 40, 40), GetColor(255, 255, 255)) {}
		
		/// <summary>
		/// �X���C�_�[��Q�[�W�Ŏg�p����F��ݒ肷��
		/// </summary>
		/// <param name="gauge">�Q�[�W�̐F</param>
		/// <param name="delay">�������̐F</param>
		/// <param name="increase">�������̐F</param>
		/// <param name="back">�w�i�F</param>
		/// <param name="point">�܂݂̐F</param>
		SliderColor(const int& gauge, const int& delay, const int& increase, const int& back, const int& point) :
			gaugeColor(gauge),
			delayColor(delay),
			increaseColor(increase),
			backColor(back),
			pointColor(point)
		{
		}
	};
}