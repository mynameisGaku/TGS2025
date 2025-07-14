#pragma once
#include "src/util/ui/UI_Canvas.h"
#include "src/util/gauge/gauge.h"

class UI_CrossHair : public UI_Canvas {
public:
	UI_CrossHair(const RectTransform& trs, int index);
	~UI_CrossHair();

	void Update() override;
	void Draw() override;

	void SetHandle_CrossHair(const std::string& path);
	void SetHandle_CrossHairFrame(const std::string& path);
	
	void SetHandle_CrossHairOutSide(const std::string& path);
	void SetHandle_CrossHairOutSideBack(const std::string& path);

	/// <summary>
	/// スクロールを設定します。
	/// </summary>
	/// <param name="value">参照する値へのポインタ。</param>
	/// <param name="min">値の最小値</param>
	/// <param name="max">値の最大値</param>
	/// <param name="scroll">スクロールの種類を指定する Gauge::ScrollType 列挙体</param>
	/// <param name="isDispMode">表示モードを変更するフラグ。true：加算時にスクロール。false：減算時にスクロール。</param>
	void SetScroll(float* value, float min, float max, Gauge::ScrollType scroll, bool isDispMode = true);

private:
	Gauge m_Gauge;
	Gauge::ScrollType m_ScrollType;
	int m_CharaIndex;

	float* m_pValue;
	float m_ValueMin;
	float m_ValueMax;
	bool m_IsDispMode;

	int hCrossHair;
	int hCrossHairFrame;

	int hCrossHairOutSide;
	int hCrossHairOutSideBack;
};