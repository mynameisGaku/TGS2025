#pragma once
#include "framework/gameObject.h"
#include <unordered_map>

class UI_Canvas;

class UI_Setter_ResultScene : public GameObject
{
public:
	UI_Setter_ResultScene();
	~UI_Setter_ResultScene();

	void Update() override;
	void Draw() override;

private:
	std::unordered_map<std::string, UI_Canvas*> m_UIs;
};