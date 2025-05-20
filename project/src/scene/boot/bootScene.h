#pragma once

#include "framework/sceneBase.h"

class SystemManager;

class BootScene : public SceneBase {
public:
	BootScene(std::string name);
	~BootScene();
	void Update() override;
	void Draw() override;

private:
	SystemManager* systemManager;
};