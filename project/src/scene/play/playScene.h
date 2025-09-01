#pragma once
#include "framework/SceneBase.h"

class PlayScene : public SceneBase {
public:
	PlayScene(std::string name);
	~PlayScene();
	void Update() override;
	void Draw() override;
private:
};