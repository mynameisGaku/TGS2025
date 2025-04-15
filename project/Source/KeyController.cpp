#include "KeyController.h"

using namespace KeyDefine;

namespace {

	char keyInput[KEY_BUFFER];		// �L�[�{�[�h�̓��͏��
	char keyInputBefore[KEY_BUFFER];// 1�t���[���O�̃L�[�{�[�h�̓��͏��
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

	// �X�V�O��keyInputBefore�ɃL�[�{�[�h�̓��͏�Ԃ�ۑ�
	for (int i = 0; i < KEY_BUFFER; i++)
		keyInputBefore[i] = keyInput[i];

	// �L�[�{�[�h�̏����X�V
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
