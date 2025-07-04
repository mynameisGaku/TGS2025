#pragma once
#include "framework/gameObject.h"

#include "src/common/game/GameManager.h"

class UI_Canvas;
class UI_Text;

class UI_Setter_ResultScene : public GameObject
{
public:
	UI_Setter_ResultScene();
	~UI_Setter_ResultScene();

	void Update() override;
	void Draw() override;

private:
	GameManager* m_pGameManager;

	UI_Canvas* m_UI_Result_Won;	// Ÿ—˜‰‰o‰æ‘œ
};