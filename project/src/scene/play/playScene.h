#pragma once
#include "framework/SceneBase.h"

class PlayScene : public SceneBase {
public:
	PlayScene(std::string name);
	~PlayScene();
	void Update() override;
	void Draw() override;
private:
	class PerformanceProfiler* m_pUpdateProfiler;
	class PerformanceProfiler* m_pDrawProfiler;
	class BallManager* m_pBallManager;
};