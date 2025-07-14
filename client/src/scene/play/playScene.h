#pragma once
#include "framework/SceneBase.h"
#include "src/util/fx/post_effect/bloom/BloomManager.h"

class PlayScene : public SceneBase {
public:
	PlayScene(std::string name);
	~PlayScene();
	void Update() override;
	void Draw() override;
private:
	BloomManager* m_BloomManager;
};