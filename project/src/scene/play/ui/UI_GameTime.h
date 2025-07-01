#pragma once
#include "src/util/ui/UI_Canvas.h"
#include <unordered_map>
#include <vector>

class GameManager;

class UI_GameTime : public UI_Canvas {
public:
	UI_GameTime();
	UI_GameTime(const RectTransform& trs, int index);
	~UI_GameTime();

	void Update() override;
	void Draw() override;

private:
	const GameManager* m_pGameManager;
	int m_CharaIndex;
};