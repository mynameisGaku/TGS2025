#include "src/util/input/InputManager.h"

// ���ėp
#include "src/util/time/GameTime.h"
#include <vendor/magic_enum/magic_enum.hpp>
#include "src/util/Utils.h"

#include "src/util/input/KeyController.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"

// ���f�o�b�O
#include "src/util/debug/imgui/imGuiManager.h"

using namespace KeyDefine;

namespace {

	std::unordered_map<std::string, std::vector<KeyCode>>* keyList;			// ���z�{�^��
	std::unordered_map<int, std::list<AdvancedEntryInfo>>* advancedEntry;	// ��s����

	std::unordered_map<KeyCode, InputData>* isInputs;		// ���͂��Ă��邩
	std::unordered_map<KeyCode, InputData>* isBeforeInputs;	// 1�t���[���O�ɓ��͂��Ă��邩

	std::list<InputData> keyCodes;	// ���߂ɉ����ꂽ�L�[����ێ�����
}

void InputManager::Init() {

	//==========================================================================================
	// ���e����͏���������

	KeyDefine::Init();

	KeyController::Init();
	PadController::Init();
	MouseController::Init();

	//==========================================================================================
	// �����z�{�^�������蓖�Ă�

	if (keyList == nullptr) {
		keyList = new std::unordered_map<std::string, std::vector<KeyCode>>();

		auto& key = (*keyList);

		key["Select"] = { KeyCode::Z, KeyCode::ButtonA };
		key["Cancel"] = { KeyCode::X, KeyCode::ButtonB };
		key["Pause"] = { KeyCode::Escape, KeyCode::Start };
		key["TargetCamera"] = { KeyCode::MiddleClick, KeyCode::RightThumb };
		key["Skip"] = { KeyCode::Z, KeyCode::Space, KeyCode::ButtonA, KeyCode::LeftClick };

		key["Up"] = { KeyCode::Up, KeyCode::UpArrow };
		key["Down"] = { KeyCode::Down, KeyCode::DownArrow };
		key["Left"] = { KeyCode::Left, KeyCode::LeftArrow };
		key["Right"] = { KeyCode::Right, KeyCode::RightArrow };

		key["Throw"] = { KeyCode::LeftClick, KeyCode::RightShoulder };
		key["Catch"] = { KeyCode::RightClick, KeyCode::LeftShoulder };
		key["Jump"] = { KeyCode::Space, KeyCode::ButtonA };
		key["Slide"] = { KeyCode::LeftShift, KeyCode::LeftControl, KeyCode::ButtonB };
		key["Teleport"] = { KeyCode::E, KeyCode::Y };

		key["Movement"] = { KeyCode::W, KeyCode::S, KeyCode::A, KeyCode::D };
		key["MoveUp"] = { KeyCode::W };
		key["MoveDown"] = { KeyCode::S };
		key["MoveLeft"] = { KeyCode::A };
		key["MoveRight"] = { KeyCode::D };

		key["GoNext"] = {
			KeyCode::Z, KeyCode::Space, KeyCode::Enter,
			KeyCode::ButtonA, KeyCode::ButtonB, KeyCode::ButtonX, KeyCode::ButtonY
		};
	}

	//==========================================================================================
	// ����s����

	if (advancedEntry == nullptr) {
		advancedEntry = new std::unordered_map<int, std::list<AdvancedEntryInfo>>();

		for (int i = DX_INPUT_PAD1; i <= PAD_NUMBER_MAX; i++) {
			(*advancedEntry)[i] = std::list<AdvancedEntryInfo>();
		}
	}

	//==========================================================================================
	// �����͏��

	if (isInputs == nullptr) {
		isInputs = new std::unordered_map<KeyCode, InputData>();

		for (const auto& itr : GetKeyAll())
			(*isInputs)[itr.second.keyCode] = itr.second;

		if (isBeforeInputs == nullptr)
			isBeforeInputs = new std::unordered_map<KeyCode, InputData>();

		*isBeforeInputs = *isInputs;
	}
}

void InputManager::Release() {

	KeyDefine::Release();

	KeyController::Release();
	PadController::Release();
	MouseController::Release();

	if (keyList != nullptr) {
		keyList->clear();

		delete keyList;
		keyList = nullptr;
	}

	if (advancedEntry != nullptr) {

		for (auto& itr : *advancedEntry) {
			if (itr.second.empty() == false)
				itr.second.clear();
		}

		advancedEntry->clear();

		delete advancedEntry;
		advancedEntry = nullptr;
	}

	if (isInputs != nullptr) {
		isInputs->clear();

		delete isInputs;
		isInputs = nullptr;
	}

	if (isBeforeInputs != nullptr) {
		isBeforeInputs->clear();

		delete isBeforeInputs;
		isBeforeInputs = nullptr;
	}
}

void InputManager::Update() {

	KeyController::Update();
	PadController::Update();
	MouseController::Update();

	// �S�ẴL�[����
	const std::unordered_map<KeyCode, InputData> keyAll = GetKeyAll();

	// �S�Ă̓��͏�Ԃ𒲂ׂ�B���͂���Ă�����A���̏���ێ�����
	for (const auto& itr : keyAll) {
		(*isInputs)[itr.first].Update();
	}

	InputDataUpdate();

	// ��s���͗p�ɓ��͏����擾����
	for (int i = DX_INPUT_PAD1; i <= PAD_NUMBER_MAX; i++)
		InputManager::AdvancedEntryUpdate(i, ADVANCED_ENTRY_TIME);
}

void InputManager::InputDataUpdate() {

	if (isInputs == nullptr || isBeforeInputs == nullptr)
		return;

	// ���͏��̍X�V
	*isBeforeInputs = *isInputs;

	for (int i = DX_INPUT_PAD1; i <= PAD_NUMBER_MAX; i++) {
		for (auto& itr : *isInputs) {
			if (itr.second.isInput[i][TouchPhase::Moved])
				itr.second.pushTime += GTime.deltaTime;
			else
				itr.second.pushTime = 0.0f;
		}
	}
}

void InputManager::AdvancedEntryUpdate(const int& padNumber, const float& _advancedEntry) {

	if (advancedEntry == nullptr) {
		advancedEntry = new std::unordered_map<int, std::list<AdvancedEntryInfo>>();

		for (int i = DX_INPUT_PAD1; i <= PAD_NUMBER_MAX; i++) {
			(*advancedEntry)[i] = std::list<AdvancedEntryInfo>();
		}
	}

	// �ێ�����Ă����s���͂̎�ސ��ŉ�
	for (auto info = (*advancedEntry)[padNumber].begin(); info != (*advancedEntry)[padNumber].end();) {

		// �ێ����Ԃ̌o��
		info->saveTime = max(info->saveTime - GTime.deltaTime, 0.0f);

		// �ێ����Ԃ�0.0�ȉ��̏ꍇ�A�؂藣��
		if (info->saveTime <= 0.0f) {
			info = (*advancedEntry)[padNumber].erase(info);
			if (info == (*advancedEntry)[padNumber].end()) break;
		}
		else
			info++;
	}
}

//====================================================================================================
// ��Push

bool InputManager::Push(const KeyDefine::KeyCode& keyCode, const int& padNumber) {

	if (isInputs == nullptr)
		return false;

	// ���͏��
	InputData inputData = (*isInputs)[keyCode];

	// ���ɓ��͂��󂯕t���Ă����ꍇ
	if (inputData.isAccepted[padNumber][TouchPhase::Begin])
		return inputData.isInput[padNumber][TouchPhase::Begin];

	DeviceType device = KeyCodeToDeviceType(keyCode);	// �f�o�C�X�̎��
	bool isInput = false;	// �������ꂽ��

	switch (device) {
	case KeyDefine::DeviceType::Key:
		isInput = (KeyController::CheckPushStatusCurrent(keyCode) && KeyController::CheckPushStatusBefore(keyCode) == false);
		break;

	case KeyDefine::DeviceType::Pad:
		if (PadController::CheckPadNumber(padNumber) == false)
			return false;

		isInput = (PadController::CheckPushStatusCurrent(keyCode, padNumber) && PadController::CheckPushStatusBefore(keyCode, padNumber) == false);
		break;

	case KeyDefine::DeviceType::Mouse:
		isInput = MouseController::CheckClick(keyCode);
		break;
	}

	inputData.isInput[padNumber][TouchPhase::Begin] = isInput;
	inputData.isAccepted[padNumber][TouchPhase::Begin] = true;

	(*isInputs)[keyCode] = inputData;

	if (isInput)
		(*advancedEntry)[padNumber].push_back(AdvancedEntryInfo(inputData, ADVANCED_ENTRY_TIME));

	return isInput;
}

bool InputManager::Push(const std::string& name, const int& padNumber) {

	if (keyList == nullptr)
		return false;

	if (keyList->contains(name) == false)
		return false;

	for (const auto& itr : (*keyList)[name]) {
		if (Push(itr, padNumber))
			return true;
	}

	return false;
}

//====================================================================================================
// ��Hold

bool InputManager::Hold(const KeyDefine::KeyCode& keyCode, const int& padNumber) {

	if (isInputs == nullptr)
		return false;

	// ���͏��
	InputData inputData = (*isInputs)[keyCode];

	// ���ɓ��͂��󂯕t���Ă����ꍇ
	if (inputData.isAccepted[padNumber][TouchPhase::Moved])
		return inputData.isInput[padNumber][TouchPhase::Moved];

	DeviceType device = KeyCodeToDeviceType(keyCode);	// �f�o�C�X�̎��
	bool isInput = false;	// �������ꂽ��

	switch (device) {
	case KeyDefine::DeviceType::Key:
		isInput = (KeyController::CheckPushStatusCurrent(keyCode));
		break;

	case KeyDefine::DeviceType::Pad:
		if (PadController::CheckPadNumber(padNumber) == false)
			return false;

		isInput = (PadController::CheckPushStatusCurrent(keyCode, padNumber));
		break;

	case KeyDefine::DeviceType::Mouse:
		isInput = MouseController::CheckHold(keyCode);
		break;
	}

	inputData.isInput[padNumber][TouchPhase::Moved] = isInput;
	inputData.isAccepted[padNumber][TouchPhase::Moved] = true;

	(*isInputs)[keyCode] = inputData;

	if (isInput)
		(*advancedEntry)[padNumber].push_back(AdvancedEntryInfo(inputData, ADVANCED_ENTRY_TIME));

	return isInput;
}

bool InputManager::Hold(const std::string& name, const int& padNumber) {

	if (keyList == nullptr)
		return false;

	if (keyList->contains(name) == false)
		return false;

	for (const auto& itr : (*keyList)[name]) {
		if (Hold(itr, padNumber))
			return true;
	}

	return false;
}

//====================================================================================================
// ��Release

bool InputManager::Release(const KeyDefine::KeyCode& keyCode, const int& padNumber) {

	if (isInputs == nullptr)
		return false;

	// ���͏��
	InputData inputData = (*isInputs)[keyCode];

	// ���ɓ��͂��󂯕t���Ă����ꍇ
	if (inputData.isAccepted[padNumber][TouchPhase::Ended])
		return inputData.isInput[padNumber][TouchPhase::Ended];

	DeviceType device = KeyCodeToDeviceType(keyCode);	// �f�o�C�X�̎��
	bool isInput = false;	// �������ꂽ��

	switch (device) {
	case KeyDefine::DeviceType::Key:
		isInput = (KeyController::CheckPushStatusCurrent(keyCode) == false && KeyController::CheckPushStatusBefore(keyCode));
		break;

	case KeyDefine::DeviceType::Pad:
		if (PadController::CheckPadNumber(padNumber) == false)
			return false;

		isInput = (PadController::CheckPushStatusCurrent(keyCode, padNumber) == false && PadController::CheckPushStatusBefore(keyCode, padNumber));
		break;

	case KeyDefine::DeviceType::Mouse:
		isInput = MouseController::CheckRelease(keyCode);
		break;
	}

	inputData.isInput[padNumber][TouchPhase::Ended] = isInput;
	inputData.isAccepted[padNumber][TouchPhase::Ended] = true;

	(*isInputs)[keyCode] = inputData;

	return isInput;
}

bool InputManager::Release(const std::string& name, const int& padNumber) {

	if (keyList == nullptr)
		return false;

	if (keyList->contains(name) == false)
		return false;

	for (const auto& itr : (*keyList)[name]) {
		if (Release(itr, padNumber))
			return true;
	}

	return false;
}

bool InputManager::Simultaneously(const std::list<KeyDefine::KeyCode>& keyCodes, const int& padNumber) {

	if (keyCodes.empty())
		return false;

	for (const auto& itr : keyCodes) {
		if (Hold(itr) == false)
			return false;
	}

	return true;
}

std::vector<KeyDefine::KeyCode> InputManager::KeyList(const std::string& name) {

	if (keyList == nullptr)
		return std::vector<KeyDefine::KeyCode>();

	if (keyList->contains(name) == false)
		return std::vector<KeyDefine::KeyCode>();

	return (*keyList)[name];
}

//====================================================================================================
// ����s����

std::list<InputData> InputManager::LatestInput(const int& padNumber) {

	// ���߂ɉ����ꂽ�L�[����ێ�����
	std::list<InputData> keyCodes;

	if (isInputs == nullptr)
		return keyCodes;

	// �S�Ă̓��͏�Ԃ𒲂ׂ�B���͂���Ă�����A���̏���ێ�����
	for (const auto& itr : *isInputs) {
		if (itr.second.isInput)
			keyCodes.push_back(itr.second);
	}

	// �ێ���������Ԃ��B
	return keyCodes;
}

bool InputManager::AdvancedEntry(const KeyDefine::KeyCode& keyCode, const int& padNumber) {

	if (advancedEntry == nullptr)
		return false;

	for (const auto& itr : (*advancedEntry)[padNumber]) {
		if (itr.inputData.keyCode == keyCode)
			return true;
	}

	return false;
}

bool InputManager::AdvancedEntry(const std::string& name, const int& padNumber) {

	if (keyList == nullptr || advancedEntry == nullptr)
		return false;

	for (auto k : (*keyList)[name]) {
		for (const auto& itr : (*advancedEntry)[padNumber]) {
			if (itr.inputData.keyCode == k)
				return true;
		}
	}

	return false;
}

Vector3 InputManager::AnalogStick(int padNumber) {

	Vector3 analog = Vector3(PadController::NormalizedLeftStick().x, 0.0f, PadController::NormalizedLeftStick().y);

	if (Hold("MoveUp"))		analog.z = 1.0f;
	if (Hold("MoveDown"))	analog.z = -1.0f;
	if (Hold("MoveRight"))	analog.x = 1.0f;
	if (Hold("MoveLeft"))	analog.x = -1.0f;

	if (analog.GetLength() > 1.0f) analog = analog.Normalize();	// 1�𒴂��Ȃ��悤�Ƀ��~�b�^�[��������

	return analog;
}

#ifdef _DEBUG

#include <vendor/magic_enum/magic_enum.hpp>

void InputManager::DrawTest(const float& x, const float& y) {

	if (isInputs == nullptr)
		return;

	std::list<InputData> inputData;

	// ���̓f�[�^��ʂ̃��X�g�Ɏ擾
	for (auto& itr : *isInputs) {
		if (itr.second.isInput[DX_INPUT_PAD1][TouchPhase::Moved]) {
			inputData.push_back(itr.second);
		}
	}

	int pushNum = 0;

	// ���̓f�[�^����ʂɕ\��
	for (const auto& itr : inputData) {
		std::string sKeyName = magic_enum::enum_name(itr.keyCode).data();
		std::string sPushTime = Function::FormatToString("time %.2f", itr.pushTime);

		DrawFormatStringF(x, y + 25.0f * pushNum, GetColor(255, 255, 255), (sKeyName + ":" + sPushTime).c_str());
		pushNum++;
	}
}

#endif // _DEBUG