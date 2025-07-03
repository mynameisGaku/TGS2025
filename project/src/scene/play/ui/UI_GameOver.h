#pragma once
#include "src/util/ui/UI_Canvas.h"
#include "src/util/gauge/gauge.h"

class UI_GameOver : public UI_Canvas {
public:
	UI_GameOver(const RectTransform& trs, int index);
	~UI_GameOver();

	void Update() override;
	void Draw() override;

private:
	int m_CharaIndex;
	int m_hWinImage;
	int m_hLoseImage;
	int m_hDrawImage;
};
