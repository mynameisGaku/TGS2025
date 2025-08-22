#pragma once
#include "src/util/ui/UI_Canvas.h"

class GameManager;
class UI_Text;

class UI_Ranking : public UI_Canvas
{
public:

	enum class STATE {
		S_NONE,
		S_FADE_IN,
		S_DISPLAY,
		S_MAX
	};

	UI_Ranking();
	~UI_Ranking();

	void Update() override;
	void Draw() override;

	void ChangeState(STATE state);

	inline STATE CurrentState() const { return m_CurrentState; }
	inline STATE PrevState() const { return m_PrevState; }

private:
	GameManager* m_pGameManager;
	std::vector<UI_Text*> m_UI_Scores;
	STATE m_CurrentState;
	STATE m_PrevState;
};
