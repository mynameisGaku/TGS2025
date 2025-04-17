#include "CharaBase.h"
#include "InputManager.h"

using namespace KeyDefine;

CharaBase::CharaBase()
{

}

CharaBase::~CharaBase()
{

}

void CharaBase::Update() {

	// デバッグ用
	// 後で消す
	if (InputManager::Hold(KeyCode::W)) {
		transform->position.z += 100.0f / 60.0f;
	}
	else if (InputManager::Hold(KeyCode::S)) {
		transform->position.z -= 100.0f / 60.0f;
	}
}
