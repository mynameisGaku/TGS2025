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
/// |使用例|
///  ￣￣￣
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

//友人のゲージクラスを参考にして作りました

/// <summary>
/// ゲージクラス
/// 上の使用例のように１ゲージにつきこのクラス１個使います。
/// </summary>
/// <author> G.F </author>
class Gauge {
public:
	Gauge();

	// スクロールの向き
	enum class ScrollType {
		eUp,	// 上から下
		eDown,	// 下から上
		eLeft,	// 左から右
		eRight,	// 右から左
	};

	/// <summary>
	/// 単色ゲージ描画 左上原点
	/// </summary>
	/// <param name="pos">描画開始位置のVector2座標</param>
	/// <param name="size">ゲージの大きさ</param>
	/// <param name="current">現在値</param>
	/// <param name="max">最大値</param>
	/// <param name="min">最小値</param>
	/// <param name="gaugeColor">ゲージの色</param>
	/// <param name="delayColor">減少時遅延ゲージの色</param>
	/// <param name="increaseColor">増加時遅延ゲージの色</param>
	/// <param name="backColor">背景色</param>
	/// <param name="scrollSpeed">遅延ゲージの速度</param>
	/// <param name="thickness">枠線の厚さ</param>
	template<typename Ty = float>
	[[nodiscard]] inline void DrawGauge(Vector2 pos, Vector2 size, Ty current, Ty max, Ty min, int gaugeColor, int delayColor, int increaseColor, int backColor,
		float scrollSpeed, float thickness = 0.0f, int innerEdgeColor = GetColor(0, 0, 0), int outerEdgeColor = GetColor(255, 255, 255)) {

		// 背景を描画
		{
			DrawBoxAA(pos.x - thickness * 2,pos.y - thickness * 2,	pos.x + size.x + thickness * 2, pos.y + size.y + thickness * 2, outerEdgeColor, TRUE);
			DrawBoxAA(pos.x - thickness,	pos.y - thickness,		pos.x + size.x + thickness,		pos.y + size.y + thickness,		innerEdgeColor, TRUE);
			DrawBoxAA(pos.x, pos.y, pos.x + size.x, pos.y + size.y, backColor, TRUE, thickness * 2.0f);
		}

		// 初回呼び出し時に遅延値を設定
		if (delayValuesF.find(id) == delayValuesF.end())
			delayValuesF[id] = current;

		Gauge::UpdateDelayValue(delayValuesF[id], current, scrollSpeed);

		// 現在値と遅延値を範囲内に収める
		current = (current < min) ? min : (current > max) ? max : current;
		delayValuesF[id] = (delayValuesF[id] < min) ? min : (delayValuesF[id] > max) ? max : delayValuesF[id];

		// ゲージの幅を計算
		float range = max - min;
		
		float percentage = (current - min) / range;
		Vector2 gaugeSize = size * percentage;

		float delayPercentage = (delayValuesF[id] - min) / range;
		Vector2 delayGaugeSize = size * delayPercentage;

		// ゲージを描画
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
	/// 画像ゲージ描画 左上原点
	/// </summary>
	/// <param name="pos">描画開始位置のVector2座標</param>
	/// <param name="current">現在値</param>
	/// <param name="max">最大値</param>
	/// <param name="min">最小値</param>
	/// <param name="graph">ゲージの画像</param>
	/// <param name="delayGraph">減少時遅延ゲージの画像</param>
	/// <param name="increaseGraph">増加時遅延ゲージの画像</param>
	/// <param name="backGraph">背景画像</param>
	/// <param name="scrollSpeed">遅延ゲージの速度</param>
	/// <param name="scrollType">スクロールする方向</param>
	/// <param name="scale">画像の拡大率</param>
	template<typename Ty = float>
	[[nodiscard]] inline void DrawGraphGauge(Vector2 pos, Ty current, Ty max, Ty min, int graph, int delayGraph, int increaseGraph, int backGraph, float scrollSpeed, Vector2 scale = Vector2::Ones) {

		int srcWidth, srcHeight;
		GetGraphSize(backGraph, &srcWidth, &srcHeight);

		int width = static_cast<int>(srcWidth * scale.x);
		int height = static_cast<int>(srcHeight * scale.y);

		// 背景を描画
		DrawRectExtendGraphF(pos.x, pos.y, pos.x + width, pos.y + height, 0, 0, srcWidth, srcHeight, backGraph, TRUE);

		// 初回呼び出し時に遅延値を設定
		if (delayValuesF.find(id) == delayValuesF.end())
			delayValuesF[id] = current;

		Gauge::UpdateDelayValue(delayValuesF[id], current, scrollSpeed);

		// 現在値と遅延値を範囲内に収める
		current = (current < min) ? min : (current > max) ? max : current;
		delayValuesF[id] = (delayValuesF[id] < min) ? min : (delayValuesF[id] > max) ? max : delayValuesF[id];

		// ゲージの幅を計算
		float range = max - min;
		float percentage = (current - min) / range;
		float gaugeWidth = width * percentage;
		float gaugeHeight = height * percentage;

		float delayPercentage = (delayValuesF[id] - min) / range;
		float delayGaugeWidth = width * delayPercentage;
		float delayGaugeHeight = height * delayPercentage;

		// ゲージを描画
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
	/// 画像ゲージ描画 中心原点
	/// </summary>
	/// <param name="pos">描画開始位置のVector2座標</param>
	/// <param name="current">現在値</param>
	/// <param name="max">最大値</param>
	/// <param name="min">最小値</param>
	/// <param name="graph">ゲージの画像</param>
	/// <param name="delayGraph">減少時遅延ゲージの画像</param>
	/// <param name="increaseGraph">増加時遅延ゲージの画像</param>
	/// <param name="backGraph">背景画像</param>
	/// <param name="scrollSpeed">遅延ゲージの速度</param>
	/// <param name="scrollType">スクロールする方向</param>
	/// <param name="scale">画像の拡大率</param>
	/// <param name="angle">画像の回転</param>
	template<typename Ty = float>
	[[nodiscard]] inline void DrawRectRotaGraphGauge(const Vector2& pos, Ty current, Ty max, Ty min, int graph, int delayGraph, int increaseGraph, int backGraph, float scrollSpeed, const Vector2& scale = Vector2::Ones, float angle = 0.0f, ScrollType scroll = ScrollType::eRight) {

		int srcWidth, srcHeight;
		GetGraphSize(graph, &srcWidth, &srcHeight);

		int width = static_cast<int>(srcWidth * scale.x);
		int height = static_cast<int>(srcHeight * scale.y);
		float scl = (scale.x + scale.y) / 2;

		// 背景を描画
		if (backGraph != -1)
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, width, height, scl, 0, backGraph, TRUE);

		// 初回呼び出し時に遅延値を設定
		if (delayValuesF.find(id) == delayValuesF.end())
			delayValuesF[id] = current;

		Gauge::UpdateDelayValue(delayValuesF[id], current, scrollSpeed);

		// 現在値と遅延値を範囲内に収める
		current = (current < min) ? min : (current > max) ? max : current;
		delayValuesF[id] = (delayValuesF[id] < min) ? min : (delayValuesF[id] > max) ? max : delayValuesF[id];

		// ゲージの幅を計算
		float range = max - min;
		float percentage = (current - min) / range;
		float gaugeWidth = width * percentage;
		float gaugeHeight = height * percentage;

		float delayPercentage = (delayValuesF[id] - min) / range;
		float delayGaugeWidth = width * delayPercentage;
		float delayGaugeHeight = height * delayPercentage;

		// ゲージを描画
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
	// 現在の値に向かって遅延して増減するゲージを作るため、IDで管理しています。
	int id;
	static int GenerateId();
	std::unordered_map<int, float> delayValuesF;
	std::unordered_map<int, int> delayValues;
	static int currentId;

	template<typename Ty = float>
	[[nodiscard]] inline void UpdateDelayValue(Ty& delayValue, Ty current, float scrollSpeed) {

		Ty diff = current - delayValue;  // 現在値との差を計算
		delayValue += diff * scrollSpeed;    // 遅延値を更新
	}
};
