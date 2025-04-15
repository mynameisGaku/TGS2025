#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"

#include "InputManager.h"
#include "Ball.h"
#include "BallManager.h"

PlayScene::PlayScene(std::string name) : SceneBase(true, name) 
{
	BallManager* ballManager = Instantiate<BallManager>();
	for (int i = 0; i < 10; i++)
	{
		Ball* ball = ballManager->CreateBall(Vector3(100 * i, 100, 0));
		ball->SetVelocity(Vector3(0, 100 * i, 0));
	}
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (InputManager::Push(KeyCode::T)) 
	{
		SceneManager::ChangeScene("TitleScene");
	}

	SceneBase::Update();
}


void PlayScene::Draw()
{
	SceneBase::Draw();

	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
