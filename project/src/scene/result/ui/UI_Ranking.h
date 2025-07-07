#pragma once
#include "src/util/ui/UI_Canvas.h"

class GameManager;

class UI_Ranking : public UI_Canvas
{
public:
	UI_Ranking();
	~UI_Ranking();

	void Update() override;
	void Draw() override;

private:
	GameManager* m_pGameManager;
};
