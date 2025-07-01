#pragma once
#include "framework/gameObject.h"
#include <unordered_map>

class UI_Canvas;

class UI_Setter_PlayScene : public GameObject
{
public:
	UI_Setter_PlayScene();
	~UI_Setter_PlayScene();

private:
	std::unordered_map<std::string, UI_Canvas*> m_UIs;

};