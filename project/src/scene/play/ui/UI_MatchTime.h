#pragma once
#include "src/util/ui/UI_Canvas.h"
#include <unordered_map>
#include <vector>

class MatchManager;
class SliceBar;

class UI_MatchTime : public UI_Canvas {
public:
	UI_MatchTime();
	UI_MatchTime(const RectTransform& trs, int index);
	~UI_MatchTime();

	void Update() override;
	void Draw() override;

	void ReloadParam();

private:
	MatchManager* m_pMatchManager;
	int m_CharaIndex;
	int m_hBarImage;
	SliceBar* m_BackBar;
	int m_hFont{};
};