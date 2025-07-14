#pragma once
#include "framework/myDxLib.h"

namespace UI_Define {

	struct SliderColor {
		int gaugeColor;		// ゲージの色
		int delayColor;		// 減少時の色
		int increaseColor;	// 増加時の色
		int backColor;		// 背景色
		int pointColor;		// つまみの色

		SliderColor() : SliderColor(GetColor(0, 255, 0), GetColor(255, 0, 0), GetColor(240, 255, 240), GetColor(40, 40, 40), GetColor(255, 255, 255)) {}
		
		/// <summary>
		/// スライダーやゲージで使用する色を設定する
		/// </summary>
		/// <param name="gauge">ゲージの色</param>
		/// <param name="delay">減少時の色</param>
		/// <param name="increase">増加時の色</param>
		/// <param name="back">背景色</param>
		/// <param name="point">つまみの色</param>
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