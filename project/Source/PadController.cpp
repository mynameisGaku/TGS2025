#include "PadController.h"

using namespace KeyDefine;

namespace {

	std::vector<XINPUT_STATE>* inputs;			// �p�b�h�̓��͏��
	std::vector<XINPUT_STATE>* inputsBefore;	// 1�t���[���O�̃p�b�h�̓��͏��
	Vector2 stickSensitivity;					// �X�e�B�b�N���x
}

void PadController::Init() {

	//==========================================================================================
	// ���R���g���[���[�̐ڑ��m�F

	if (inputs == nullptr) {
		inputs = new std::vector<XINPUT_STATE>();

		for (int i = 0; i < PAD_NUMBER_MAX; i++) {
			inputs->push_back(XINPUT_STATE());

			// ���R���g���[���[����̐ڑ����m�F
			if (CheckPadNumber(i + 1))
				GetJoypadXInputState(i + 1, &(*inputs)[i]);
		}
	}

	if (inputsBefore == nullptr) {
		inputsBefore = new std::vector<XINPUT_STATE>();
		*inputsBefore = *inputs;
	}

	stickSensitivity = Vector2(1.5f);
}

void PadController::Release() {

	if (inputs != nullptr) {
		inputs->clear();
		delete inputs;
		inputs = nullptr;
	}

	if (inputsBefore != nullptr) {
		inputsBefore->clear();
		delete inputsBefore;
		inputsBefore = nullptr;
	}
}

void PadController::Update() {

	// �X�V�O��inputsBefore�Ƀp�b�h�̓��͏�Ԃ�ۑ�
	*inputsBefore = *inputs;

	const int inputPadNum = min(max(GetJoypadNum(), 0), PAD_NUMBER_MAX);

	// �p�b�h�̏����X�V
	for (int i = 0; i < inputPadNum; i++) {
		if (CheckPadNumber(i + 1))
			GetJoypadXInputState(i + 1, &(*inputs)[i]);
		else
			(*inputs)[i] = XINPUT_STATE();
	}
}

bool PadController::CheckPushStatusCurrent(KeyDefine::KeyCode keyCode, int padNumber) {

	if (CheckPadNumber(padNumber) == false)
		return false;

	if (KeyCodeToDeviceType(keyCode) != DeviceType::Pad)
		return false;

	int* value = KeyDefine::GetInputData(keyCode).value;

	return ((*inputs)[padNumber - 1].Buttons[*value] != 0);
}

bool PadController::CheckPushStatusBefore(KeyDefine::KeyCode keyCode, int padNumber) {

	if (CheckPadNumber(padNumber) == false)
		return false;

	if (KeyCodeToDeviceType(keyCode) != DeviceType::Pad)
		return false;

	int* value = KeyDefine::GetInputData(keyCode).value;

	return ((*inputsBefore)[padNumber - 1].Buttons[*value] != 0);
}

void PadController::SetStickSensitivity(Vector2 value) {

	stickSensitivity = value;
}

Vector2 PadController::StickSensitivity() {

	return stickSensitivity;
}

Vector2 PadController::NormalizedLeftStick(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return V2::ZERO;

	float x = static_cast<float>((*inputs)[padNumber - 1].ThumbLX) / 32767.0f;
	float y = static_cast<float>((*inputs)[padNumber - 1].ThumbLY) / 32767.0f;

	return Vector2(x, y);
}

Vector2 PadController::NormalizedRightStick(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return V2::ZERO;

	float x = static_cast<float>((*inputs)[padNumber - 1].ThumbRX) / 32767.0f;
	float y = static_cast<float>((*inputs)[padNumber - 1].ThumbRY) / 32767.0f;

	return Vector2(x, y);
}

float PadController::NormalizedLeftTrigger(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return 0.0f;

	return static_cast<float>((*inputs)[padNumber - 1].LeftTrigger) / 255;
}

float PadController::NormalizedRightTrigger(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return 0.0f;

	return static_cast<float>((*inputs)[padNumber - 1].RightTrigger) / 255;
}

XINPUT_STATE PadController::Input(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return XINPUT_STATE();

	return (*inputs)[padNumber - 1];
}
