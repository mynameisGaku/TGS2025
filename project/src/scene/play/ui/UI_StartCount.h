#pragma once
#include "src/util/ui/UI_Canvas.h"
#include <unordered_map>
#include <vector>

class MatchManager;

class UI_StartCount : public UI_Canvas {
public:
	UI_StartCount();
	UI_StartCount(const RectTransform& trs, int index);
	~UI_StartCount();

	void Update() override;
	void Draw() override;

private:
	MatchManager* m_pMatchManager;
	int m_CharaIndex;
};
