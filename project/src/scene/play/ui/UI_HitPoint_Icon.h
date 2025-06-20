#pragma once
#include "src/util/ui/UI_Image.h"

class UI_HitPoint_Icon : public UI_Image {
public:
	UI_HitPoint_Icon();
	UI_HitPoint_Icon(const RectTransform& trs, int index);
	~UI_HitPoint_Icon();

	void Update() override;
	void Draw() override;

	void SetValue(int* value, int valueMin, int valueMax, int dispNum);

	void SetDiffSize(const Vector2& diffSize);

private:
	int charaIndex;

	int* m_Value;
	int m_ValueMin;
	int m_ValueMax;
	int m_DispNum;

	Vector2 m_DiffSize;
};