#pragma once

#include "framework/sceneBase.h"

class SystemManager;
class GameManager;
class NetworkManager;
class LoadScreen;

class BootScene : public SceneBase {
public:
	BootScene(std::string name);
	~BootScene();
	void Update() override;
	void Draw() override;

private:
	SystemManager* systemManager;
	GameManager* gameM;
	LoadScreen* loadScreen;
};