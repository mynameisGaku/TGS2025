#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"

#include "InputManager.h"
#include "Ball.h"

PlayScene::PlayScene(std::string name) : SceneBase(true, name) 
{
	Ball* ball = Instantiate<Ball>();
	ball->SetVelocity(Vector3(0, 100, 0));
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
