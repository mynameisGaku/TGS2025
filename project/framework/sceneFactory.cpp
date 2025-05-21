#include "framework/sceneFactory.h"
#include <windows.h>
#include <assert.h>

// ���V�[���֘A
#include "src/scene/boot/bootScene.h"
#include "src/scene/title/titleScene.h"
#include "src/scene/play/playScene.h"

SceneBase* SceneFactory::CreateFirst()
{
	return new BootScene("BootScene");
}

SceneBase * SceneFactory::Create(const std::string& name)
{
	if (name == "TitleScene") {
		return new TitleScene(name);
	}
	if (name == "PlayScene") {
		return new PlayScene(name);
	}
	MessageBox(NULL, ("���̃V�[���͂���܂���\n" + name).c_str(), "SceneFactory", MB_ICONERROR | MB_OK);
	assert(false);
	return nullptr;
}
