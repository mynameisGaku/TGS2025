#include "KeyController.h"

using namespace KeyDefine;

namespace {

	char keyInput[KEY_BUFFER];		// キーボードの入力情報
	char keyInputBefore[KEY_BUFFER];// 1フレーム前のキーボードの入力情報
}

void KeyController::Init() {

	for (int i = 0; i < KEY_BUFFER; i++) {
		keyInput[i] = 0;
		keyInputBefore[i] = 0;
	}
}

void KeyController::Release()
{
}

void KeyController::Update() {

	// 更新前にkeyInputBeforeにキーボードの入力状態を保存
	for (int i = 0; i < KEY_BUFFER; i++)
		keyInputBefore[i] = keyInput[i];

	// キーボードの情報を更新
	GetHitKeyStateAll(keyInput);
}

bool KeyController::CheckPushStatusCurrent(KeyCode keyCode) {

	if (KeyCodeToDeviceType(keyCode) != DeviceType::Key)
		return false;

	int* value = GetKeyAll()[keyCode].value;

	return (keyInput[*value] != 0);
}

bool KeyController::CheckPushStatusBefore(KeyDefine::KeyCode keyCode) {

	if (KeyCodeToDeviceType(keyCode) != DeviceType::Key)
		return false;

	int* value = GetKeyAll()[keyCode].value;

	return (keyInputBefore[*value] != 0);
}
