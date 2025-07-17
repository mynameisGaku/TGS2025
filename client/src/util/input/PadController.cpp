#include "PadController.h"

using namespace KeyDefine;

namespace {

	std::vector<XINPUT_STATE>* inputs;			// パッドの入力情報
	std::vector<XINPUT_STATE>* inputsBefore;	// 1フレーム前のパッドの入力情報

	Vector2 stickSensitivity;					// スティック感度

	std::vector<std::unordered_map<StickNumber, Vector2>>* stick;		// スティックの傾き
	std::vector<std::unordered_map<StickNumber, Vector2>>* stickBefore;	// 1フレーム前のスティックの傾き

	std::vector<std::unordered_map<TriggerNumber, float>>* trigger;			// トリガーの押し込み
	std::vector<std::unordered_map<TriggerNumber, float>>* triggerBefore;	// 1フレーム前のトリガーの押し込み
}

void PadController::Init() {

	//==========================================================================================
	// ▼コントローラーの接続確認

	if (inputs == nullptr) {
		inputs = new std::vector<XINPUT_STATE>();

		for (int i = 0; i < PAD_NUMBER_MAX; i++) {
			inputs->push_back(XINPUT_STATE());

			// ▼コントローラーからの接続を確認
			if (CheckPadNumber(i + 1))
				GetJoypadXInputState(i + 1, &(*inputs)[i]);
		}
	}

	if (inputsBefore == nullptr) {
		inputsBefore = new std::vector<XINPUT_STATE>();
		*inputsBefore = *inputs;
	}

	if (stick == nullptr) {
		stick = new std::vector<std::unordered_map<StickNumber, Vector2>>();

		for (int i = 0; i < PAD_NUMBER_MAX; i++) {
			std::unordered_map<StickNumber, Vector2> stickSlope;
			stickSlope[StickNumber::eLeftStick] = Vector2::Zero;
			stickSlope[StickNumber::eRightStick] = Vector2::Zero;
			stick->push_back(stickSlope);
		}
	}

	if (stickBefore == nullptr) {
		stickBefore = new std::vector<std::unordered_map<StickNumber, Vector2>>();
		*stickBefore = *stick;
	}

	if (trigger == nullptr) {
		trigger = new std::vector<std::unordered_map<TriggerNumber, float>>();

		for (int i = 0; i < PAD_NUMBER_MAX; i++) {
			std::unordered_map<TriggerNumber, float> triggerData;
			triggerData[TriggerNumber::eLeftTrigger] = 0.0f;
			triggerData[TriggerNumber::eRightTrigger] = 0.0f;
			trigger->push_back(triggerData);
		}
	}

	if (triggerBefore == nullptr) {
		triggerBefore = new std::vector<std::unordered_map<TriggerNumber, float>>();
		*triggerBefore = *trigger;
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

	if (stick != nullptr) {
		stick->clear();
		delete stick;
		stick = nullptr;
	}

	if (stickBefore != nullptr) {
		stickBefore->clear();
		delete stickBefore;
		stickBefore = nullptr;
	}

	if (trigger != nullptr) {
		trigger->clear();
		delete trigger;
		trigger = nullptr;
	}

	if (triggerBefore != nullptr) {
		triggerBefore->clear();
		delete triggerBefore;
		triggerBefore = nullptr;
	}
}

void PadController::Update() {

	InputUpdate();
	StickUpdate();
	TriggerUpdate();
}

void PadController::InputUpdate() {

	if (inputs == nullptr || inputsBefore == nullptr)
		return;

	// 更新前にinputsBeforeにパッドの入力状態を保存
	*inputsBefore = *inputs;

	const int inputPadNum = min(max(GetJoypadNum(), 0), PAD_NUMBER_MAX);

	// パッドの情報を更新
	for (int i = 0; i < inputPadNum; i++) {
		if (CheckPadNumber(i + 1))
			GetJoypadXInputState(i + 1, &(*inputs)[i]);
		else
			(*inputs)[i] = XINPUT_STATE();
	}
}

void PadController::StickUpdate() {

	if (stick == nullptr || stickBefore == nullptr)
		return;

	const int inputPadNum = min(max(GetJoypadNum(), 0), PAD_NUMBER_MAX);

	// 更新前にstickBeforeにスティックの傾きを保存
	*stickBefore = *stick;

	// スティックの傾きを更新
	for (int i = 0; i < inputPadNum; i++) {
		(*stick)[i][StickNumber::eLeftStick].x = static_cast<float>((*inputs)[i].ThumbLX) / 32767.0f;
		(*stick)[i][StickNumber::eLeftStick].y = static_cast<float>((*inputs)[i].ThumbLY) / 32767.0f;

		(*stick)[i][StickNumber::eRightStick].x = static_cast<float>((*inputs)[i].ThumbRX) / 32767.0f;
		(*stick)[i][StickNumber::eRightStick].y = static_cast<float>((*inputs)[i].ThumbRY) / 32767.0f;
	}
}

void PadController::TriggerUpdate() {

	if (trigger == nullptr || triggerBefore == nullptr)
		return;

	const int inputPadNum = min(max(GetJoypadNum(), 0), PAD_NUMBER_MAX);

	// 更新前にtriggerBeforeにトリガーの押し込みを保存
	*triggerBefore = *trigger;

	// トリガーの押し込みを更新
	for (int i = 0; i < inputPadNum; i++) {
		(*trigger)[i][TriggerNumber::eLeftTrigger] = static_cast<float>((*inputs)[i].LeftTrigger) / 255.0f;
		(*trigger)[i][TriggerNumber::eRightTrigger] = static_cast<float>((*inputs)[i].RightTrigger) / 255.0f;
	}
}

bool PadController::CheckPushStatusCurrent(KeyDefine::KeyCode keyCode, int padNumber) {

	if (CheckCorrectData(keyCode, padNumber) == false)
		return false;

	if (CheckInclinationStickCurrent(keyCode, padNumber))
		return true;

	if (CheckInclinationTriggerCurrent(keyCode, padNumber))
		return true;

	int* value = KeyDefine::GetInputData(keyCode).value;

	if (*value < 0)
		return false;

	return ((*inputs)[padNumber - 1].Buttons[*value] != 0);
}

bool PadController::CheckPushStatusBefore(KeyDefine::KeyCode keyCode, int padNumber) {

	if (CheckCorrectData(keyCode, padNumber) == false)
		return false;

	if (CheckInclinationStickBefore(keyCode, padNumber))
		return true;

	if (CheckInclinationTriggerBefore(keyCode, padNumber))
		return true;

	int* value = KeyDefine::GetInputData(keyCode).value;

	if (*value < 0)
		return false;

	return ((*inputsBefore)[padNumber - 1].Buttons[*value] != 0);
}

bool PadController::CheckInclinationStickCurrent(KeyCode keyCode, int padNumber) {

	bool result = false;
	const Vector2 leftStick = NormalizedLeftStick(padNumber);
	const Vector2 rightStick = NormalizedRightStick(padNumber);

	switch (keyCode) {
	case KeyDefine::KeyCode::LeftStickUp:		result = (leftStick.y >= STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::LeftStickDown:		result = (leftStick.y <= -STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::LeftStickLeft:		result = (leftStick.x <= -STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::LeftStickRight:	result = (leftStick.x >= STICK_DEADZONE);	break;

	case KeyDefine::KeyCode::RightStickUp:		result = (rightStick.y >= STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::RightStickDown:	result = (rightStick.y <= -STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::RightStickLeft:	result = (rightStick.x <= -STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::RightStickRight:	result = (rightStick.x >= STICK_DEADZONE);	break;
	default:
		break;
	}

	return result;
}

bool PadController::CheckInclinationStickBefore(KeyCode keyCode, int padNumber) {

	bool result = false;
	const Vector2 leftStick = NormalizedLeftStickBefore(padNumber);
	const Vector2 rightStick = NormalizedRightStickBefore(padNumber);

	switch (keyCode) {
	case KeyDefine::KeyCode::LeftStickUp:		result = (leftStick.y >= STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::LeftStickDown:		result = (leftStick.y <= -STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::LeftStickLeft:		result = (leftStick.x <= -STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::LeftStickRight:	result = (leftStick.x >= STICK_DEADZONE);	break;

	case KeyDefine::KeyCode::RightStickUp:		result = (rightStick.y >= STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::RightStickDown:	result = (rightStick.y <= -STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::RightStickLeft:	result = (rightStick.x <= -STICK_DEADZONE);	break;
	case KeyDefine::KeyCode::RightStickRight:	result = (rightStick.x >= STICK_DEADZONE);	break;
	default:
		break;
	}

	return result;
}

bool PadController::CheckInclinationTriggerCurrent(KeyCode keyCode, int padNumber) {

	bool result = false;
	const float leftTrigger = NormalizedLeftTrigger(padNumber);
	const float rightTrigger = NormalizedRightTrigger(padNumber);

	switch (keyCode) {
	case KeyDefine::KeyCode::LeftTrigger:	result = (leftTrigger >= TRIGGER_DEADZONE);		break;
	case KeyDefine::KeyCode::RightTrigger:	result = (rightTrigger >= TRIGGER_DEADZONE);	break;
	default:
		break;
	}

	return result;
}

bool PadController::CheckInclinationTriggerBefore(KeyCode keyCode, int padNumber) {

	bool result = false;
	const float leftTrigger = NormalizedLeftTriggerBefore(padNumber);
	const float rightTrigger = NormalizedRightTriggerBefore(padNumber);

	switch (keyCode) {
	case KeyDefine::KeyCode::LeftTrigger:	result = (leftTrigger >= TRIGGER_DEADZONE);		break;
	case KeyDefine::KeyCode::RightTrigger:	result = (rightTrigger >= TRIGGER_DEADZONE);	break;
	default:
		break;
	}

	return result;
}

void PadController::SetStickSensitivity(Vector2 value) {

	stickSensitivity = value;
}

Vector2 PadController::StickSensitivity() {

	return stickSensitivity;
}

Vector2 PadController::NormalizedLeftStick(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return Vector2::Zero;

	float x = static_cast<float>((*inputs)[padNumber - 1].ThumbLX) / 32767.0f;
	float y = static_cast<float>((*inputs)[padNumber - 1].ThumbLY) / 32767.0f;

	if (fabsf(x) <= STICK_DEADZONE)
		x = 0.0f;
	if (fabsf(y) <= STICK_DEADZONE)
		y = 0.0f;

	return Vector2(x, y);
}

Vector2 PadController::NormalizedRightStick(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return Vector2::Zero;

	float x = static_cast<float>((*inputs)[padNumber - 1].ThumbRX) / 32767.0f;
	float y = static_cast<float>((*inputs)[padNumber - 1].ThumbRY) / 32767.0f;

	if (fabsf(x) <= STICK_DEADZONE)
		x = 0.0f;
	if (fabsf(y) <= STICK_DEADZONE)
		y = 0.0f;

	return Vector2(x, y);
}

Vector2 PadController::NormalizedLeftStickBefore(int padNumber) {

	if (CheckPadNumber(padNumber) == false || stickBefore == nullptr)
		return Vector2::Zero;

	float x = static_cast<float>((*stickBefore)[padNumber - 1][StickNumber::eLeftStick].x) / 32767.0f;
	float y = static_cast<float>((*stickBefore)[padNumber - 1][StickNumber::eLeftStick].y) / 32767.0f;

	if (fabsf(x) <= STICK_DEADZONE)
		x = 0.0f;
	if (fabsf(y) <= STICK_DEADZONE)
		y = 0.0f;

	return Vector2(x, y);
}

Vector2 PadController::NormalizedRightStickBefore(int padNumber) {

	if (CheckPadNumber(padNumber) == false || stickBefore == nullptr)
		return Vector2::Zero;

	float x = static_cast<float>((*stickBefore)[padNumber - 1][StickNumber::eRightStick].x) / 32767.0f;
	float y = static_cast<float>((*stickBefore)[padNumber - 1][StickNumber::eRightStick].y) / 32767.0f;

	if (fabsf(x) <= STICK_DEADZONE)
		x = 0.0f;
	if (fabsf(y) <= STICK_DEADZONE)
		y = 0.0f;

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

float PadController::NormalizedLeftTriggerBefore(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return 0.0f;

	return (*triggerBefore)[padNumber - 1][TriggerNumber::eLeftTrigger];
}

float PadController::NormalizedRightTriggerBefore(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return 0.0f;

	return (*triggerBefore)[padNumber - 1][TriggerNumber::eRightTrigger];
}

XINPUT_STATE PadController::Input(int padNumber) {

	if (CheckPadNumber(padNumber) == false || inputs == nullptr)
		return XINPUT_STATE();

	return (*inputs)[padNumber - 1];
}
