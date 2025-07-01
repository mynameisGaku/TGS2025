#pragma once
#include "src/util/ui/UI_Image.h"

class UI_HitPoint_Icon : public UI_Image {
public:
	UI_HitPoint_Icon();
	UI_HitPoint_Icon(const RectTransform& trs, int index);
	~UI_HitPoint_Icon();

	void Update() override;
	void Draw() override;

	void SetValue(float* value, float valueMin, float valueMax, float dispNum);

private:
	int m_CharaIndex;

	float* m_pValue;
	float m_ValueMin;
	float m_ValueMax;
	float m_DispNum;

	Vector2 m_DiffSize;

	bool m_NeedRelocation;
};