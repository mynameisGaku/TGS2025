#pragma once

// ◇継承元
#include "framework/gameObject.h"

// ◇汎用
#include <fstream>
#include <assert.h>
#include "src/util/file/csv/CsvReader.h"

#include "src/util/math/vector2.h"
#include "src/util/ui/UI_Manager.h"
#include "src/util/easing/Easing.h"

/// <summary>
/// 設定項目の基底クラス
/// </summary>
class SettingBase : public GameObject {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	SettingBase();
	virtual ~SettingBase();

	//==========================================================================================
	// ▼各種関数

	virtual void Update() override;

	/// <summary>
	/// Csvから設定項目の数値を取得する
	/// </summary>
	virtual void LoadCsv(std::string filename) {}

	/// <summary>
	/// Csvに設定項目の数値を保存する
	/// </summary>
	virtual void SaveCsv(std::string filename) {}

	/// <summary>
	/// 設定した項目を適応させる
	/// </summary>
	virtual void Apply() {}

	/// <summary>
	/// 設定項目をフェードインさせる
	/// </summary>
	void FadeIn();

	/// <summary>
	/// 設定項目をフェードアウトさせる
	/// </summary>
	void FadeOut();

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// 調整可能かを設定する
	/// </summary>
	inline void SetIsAdjustable(bool value) { isAdjustable = value; }

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// 調整可能かを取得する
	/// </summary>
	/// <returns>調整可能ならtrue</returns>
	inline bool IsAdjustable() const { return isAdjustable; }

protected:
	std::list<UI_Slider*> sliders;		// スライダーの実体
	UI_Define::SliderColor sliderColor;	// スライダーの色情報
	FontInfo fontInfo;					// フォント情報
	EasingUtils::EasingInt alphaEasing;	// 不透明度

	bool isAdjustable;	// 調整可能か
};