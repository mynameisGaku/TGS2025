#include "framework/sceneManager.h"
#include "framework/sceneBase.h"
#include "framework/sceneFactory.h"
#include "src/common/system/SystemManager.h"

#include "src/common/camera/CameraManager.h"
#include "src/common/setting/window/WindowSetting.h"

namespace {

	std::string m_currentName; // 現在のシーンの名称
	std::string m_nextName;    // 次のシーンの名称
	SceneBase* m_currentScene; // 今のシーンのインスタンスを保持
	SceneBase* m_commonScene;  // 共通シーン
	SceneFactory* m_factory;   // シーン切り替え用のFactoryのポインター
};

void SceneManager::Start()
{
	m_nextName = "";
	m_currentName = "";
	m_commonScene = new SceneBase(false, "CommonScene");

	m_factory = new SceneFactory();
	// 最初に動くシーンを、SceneFactoryに作ってもらう
	m_currentScene = m_factory->CreateFirst();
}

void SceneManager::Update()
{
	if (m_nextName != m_currentName) { // シーン切り替えの指定があったので
		if (m_currentScene != nullptr) { // 今までのシーンを解放
			delete m_currentScene;
			m_currentScene = nullptr;
		}
		m_currentName = m_nextName;
		m_currentScene = m_factory->Create(m_nextName); // 次のシーンを作成
	}
	if (m_currentScene != nullptr)
		m_currentScene->Update();
	m_commonScene->Update();
}

void SceneManager::Draw()
{
	if (not CameraManager::IsScreenDivision()) {
		DrawBefore();

		if (m_currentScene != nullptr) {
			m_currentScene->Draw();
		}
		m_commonScene->Draw();
	}
	else {

		int screenBeingX = 0;
		int screenBeingY = 0;

		int screenWidth = WindowSetting::Inst().width;
		int screenHeight = WindowSetting::Inst().height;

		int screenHalfX = WindowSetting::Inst().width_half;
		int screenHalfY = WindowSetting::Inst().height;

		//===========================================================
		// 1Pカメラの描画

		CameraManager::CameraScreenDivisionDraw(screenBeingX, screenBeingY, screenHalfX, screenHeight, 0);

		DrawBefore();

		if (m_currentScene != nullptr) {
			m_currentScene->Draw();
		}
		m_commonScene->Draw();

		//===========================================================
		// 2Pカメラの描画

		CameraManager::CameraScreenDivisionDraw(screenHalfX, screenBeingY, screenHalfX, screenHeight, 1);

		DrawBefore();

		if (m_currentScene != nullptr) {
			m_currentScene->Draw();
		}
		m_commonScene->Draw();

		//===========================================================
		// 画面分割処を初期化

		CameraManager::CameraScreenDivision(0, 0, screenWidth, screenHeight);
	}
}

void SceneManager::Release()
{
	m_currentName.clear();
	m_nextName.clear();

	if (m_currentScene != nullptr) {
		delete m_currentScene;
		m_currentScene = nullptr;
	}
	if (m_commonScene != nullptr) {
		delete m_commonScene;
		m_commonScene = nullptr;
	}
	if (m_factory != nullptr) {
		delete m_factory;
		m_factory = nullptr;
	}
}

SceneBase* SceneManager::CommonScene()
{
	return m_commonScene;
}

SceneBase* SceneManager::CurrentScene()
{
	return m_currentScene;
}

void SceneManager::SetCurrentScene(SceneBase* scene)
{
	m_currentScene = scene;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	m_nextName = sceneName;
}
