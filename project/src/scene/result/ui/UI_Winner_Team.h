#pragma once
#include "src/util/ui/UI_Canvas.h"

class GameManager;

class UI_Winner_Team : public UI_Canvas
{
public:
	UI_Winner_Team();
	~UI_Winner_Team();

	void Update() override;
	void Draw() override;

private:
	GameManager* m_pGameManager;
};
