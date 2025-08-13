#pragma once
#include "src/util/ui/UI_Image.h"

class SliceBar;

class UI_ChatBar : public UI_Image {
public:
	UI_ChatBar();
	UI_ChatBar(const RectTransform& trs, int index);
	~UI_ChatBar();

	void Update() override;
	void Draw() override;

private:
	SliceBar* m_BarBack;
	SliceBar* m_BarGaugeBack;

};
