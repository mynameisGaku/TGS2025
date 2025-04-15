#include "inputManager.h"

// ◇汎用
#include "../Library/time.h"
#include "Util/Utils.h"

#include "KeyController.h"
#include "PadController.h"
#include "MouseController.h"

// ◇デバッグ
#include "ImGuiTree/imGuiManager.h"

using namespace KeyDefine;

namespace {	

	std::unordered_map<std::string, std::vector<KeyCode>>* keyList;			// 仮想ボタン
	std::unordered_map<int, std::list<AdvancedEntryInfo>>* advancedEntry;	// 先行入力

	std::unordered_map<KeyCode, InputData>* isInputs;		// 入力しているか
	std::unordered_map<KeyCode, InputData>* isBeforeInputs;	// 1フレーム前に入力しているか
}

void InputManager::Init() {

	//==========================================================================================
	// ▼各種入力情報を初期化

	KeyDefine::Init();

	KeyController::Init();
	PadController::Init();
	MouseController::Init();

	//==========================================================================================
	// ▼仮想ボタンを割り当てる

	if (keyList == nullptr) {
		keyList = new std::unordered_map<std::string, std::vector<KeyCode>>();

		auto& key = (*keyList);

		key["Select"]		= { KeyCode::Z, KeyCode::ButtonA };
		key["Select_Click"] = { KeyCode::Z, KeyCode::ButtonA, KeyCode::LeftClick };
		key["Cancel"]		= { KeyCode::X, KeyCode::ButtonB };
		key["Pause"]		= { KeyCode::Escape, KeyCode::Start };
		key["TargetCamera"] = { KeyCode::MiddleClick, KeyCode::RightThumb };
		key["Skip"]			= { KeyCode::Z, KeyCode::ButtonA, KeyCode::LeftClick };

		key["Up"]	 = { KeyCode::Up, KeyCode::UpArrow };
		key["Down"]	 = { KeyCode::Down, KeyCode::DownArrow };
		key["Left"]  = { KeyCode::Left, KeyCode::LeftArrow };
		key["Right"] = { KeyCode::Right, KeyCode::RightArrow };

		key["Attack"]	= { KeyCode::LeftClick, KeyCode::ButtonX };
		key["Strong"]	= { KeyCode::RightClick, KeyCode::ButtonY };
		key["Special"]	= { KeyCode::Z, KeyCode::ButtonB };
		key["Avoid"]	= { KeyCode::Space, KeyCode::ButtonA };
		key["Run"]		= { KeyCode::LeftShift, KeyCode::LeftThumb };

		key["MoveUp"]	 = { KeyCode::W };
		key["MoveDown"]  = { KeyCode::S };
		key["MoveLeft"]  = { KeyCode::A };
		key["MoveRight"] = { KeyCode::D };
	}

	//==========================================================================================
	// ▼先行入力

	if (advancedEntry == nullptr) {
		advancedEntry = new std::unordered_map<int, std::list<AdvancedEntryInfo>>();
	}

	//==========================================================================================
	// ▼入力情報

	if (isInputs == nullptr) {
		isInputs = new std::unordered_map<KeyCode, InputData>();

		for (const auto& itr : GetKeyAll())
			(*isInputs)[itr.second.keyCode] = InputData(itr.second, 0.0f, false);

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

	// 入力情報の更新
	if (isInputs != nullptr && isBeforeInputs != nullptr) {
		*isBeforeInputs = *isInputs;
		for (auto& itr : *isInputs) {
			if (itr.second.isPut)
				itr.second.putTime += Time::DeltaTime();
			else
				itr.second.putTime = 0.0f;
		}
	}

	// 先行入力の更新
	if (advancedEntry != nullptr) {

		for (auto& itr : *advancedEntry) {
			for (auto info = itr.second.begin(); info != itr.second.end();) {

				// 保持時間の経過
				info->saveTime = max(info->saveTime - Time::DeltaTimeLapseRate(), 0.0f);

				if (info->saveTime == 0.0f) {
					info = itr.second.erase(info);
					if (info == itr.second.end()) break;
				}
				else
					info++;
			}
		}
	}

	// 先行入力用に入力情報を取得する
	for (int i = 0; i < PAD_NUMBER_MAX; i++)
		InputManager::AdvancedEntryUpdate(i + 1, ADVANCED_ENTRY_TIME);

	//dynamic_cast<ImGuiNode_Text*>(ImGuiManager::FindDefineNode("Mouse Pos"))->SetText(Function::FormatToString("Mouse Pos (%.1f, %.1f) Move (%d, %d)", mouse.position.x, mouse.position.y, mouse.moveX, mouse.moveY));

#ifdef _DEBUG

	//if (InputManager::Push(KeyCode::Alpha0))
	//	MouseController::SetMouseMovement(MouseMovement::Free);
	//else if (InputManager::Push(KeyCode::Alpha9))
	//	MouseController::SetMouseMovement(MouseMovement::Fixed);
	//else if (InputManager::Push(KeyCode::Alpha8))
	//	MouseController::SetMouseMovement(MouseMovement::OnScreen);

	//InputManager::DrawTest(300.0f, 5.0f);

#endif

}

//====================================================================================================
// ▼Push

bool InputManager::Push(KeyCode keyCode, int num) {

	DeviceType device = KeyCodeToDeviceType(keyCode);

	switch (device) {
	case KeyDefine::DeviceType::Key:
		(*isInputs)[keyCode].isPut = (KeyController::CheckPushStatusCurrent(keyCode) && KeyController::CheckPushStatusBefore(keyCode) == false);
		break;

	case KeyDefine::DeviceType::Pad:
		if (PadController::CheckPadNumber(num) == false)
			return false;

		(*isInputs)[keyCode].isPut = (PadController::CheckPushStatusCurrent(keyCode, num) && PadController::CheckPushStatusBefore(keyCode, num) == false);
		break;
	
	case KeyDefine::DeviceType::Mouse:
		(*isInputs)[keyCode].isPut = MouseController::CheckClick(keyCode);
		break;
	}

	return (*isInputs)[keyCode].isPut;
}

bool InputManager::Push(std::string name, int num) {

	if (keyList == nullptr)
		return false;

	if (keyList->contains(name) == false)
		return false;

	for (const auto& itr : (*keyList)[name]) {
		if (Push(itr, num))
			return true;
	}

	return false;
}

//====================================================================================================
// ▼Hold

bool InputManager::Hold(KeyCode keyCode, int num) {

	DeviceType device = KeyCodeToDeviceType(keyCode);

	switch (device) {
	case KeyDefine::DeviceType::Key:
		(*isInputs)[keyCode].isPut = KeyController::CheckPushStatusCurrent(keyCode);
		break;

	case KeyDefine::DeviceType::Pad:
		if (PadController::CheckPadNumber(num) == false)
			return false;

		(*isInputs)[keyCode].isPut = PadController::CheckPushStatusCurrent(keyCode, num);
		break;

	case KeyDefine::DeviceType::Mouse:
		(*isInputs)[keyCode].isPut = MouseController::CheckHold(keyCode);
		break;
	}

	return (*isInputs)[keyCode].isPut;
}

bool InputManager::Hold(std::string name, int num) {

	if (keyList == nullptr)
		return false;

	if (keyList->contains(name) == false)
		return false;

	for (const auto& itr : (*keyList)[name]) {
		if (Hold(itr, num))
			return true;
	}

	return false;
}

//====================================================================================================
// ▼Release

bool InputManager::Release(KeyCode keyCode, int num) {

	DeviceType device = KeyCodeToDeviceType(keyCode);

	switch (device) {
	case KeyDefine::DeviceType::Key:
		(*isInputs)[keyCode].isPut = (KeyController::CheckPushStatusCurrent(keyCode) == false && KeyController::CheckPushStatusBefore(keyCode));
		break;

	case KeyDefine::DeviceType::Pad:
		if (PadController::CheckPadNumber(num) == false)
			return false;

		(*isInputs)[keyCode].isPut = (PadController::CheckPushStatusCurrent(keyCode, num) == false && PadController::CheckPushStatusBefore(keyCode, num));
		break;

	case KeyDefine::DeviceType::Mouse:
		(*isInputs)[keyCode].isPut = MouseController::CheckRelease(keyCode);
		break;
	}

	return (*isInputs)[keyCode].isPut;
}

bool InputManager::Release(std::string name, int num) {

	if (keyList == nullptr)
		return false;

	if (keyList->contains(name) == false)
		return false;

	for (const auto& itr : (*keyList)[name]) {
		if (Release(itr, num))
			return true;
	}

	return false;
}

bool InputManager::Simultaneously(std::list<KeyDefine::KeyCode> keyCodes, int num) {

	if (keyCodes.empty())
		return false;

	for (const auto& itr : keyCodes) {
		if (Hold(itr) == false)
			return false;
	}

	return true;
}

std::vector<KeyDefine::KeyCode> InputManager::KeyList(std::string name) {

	if (keyList == nullptr)
		return std::vector<KeyDefine::KeyCode>();
	
	if (keyList->contains(name) == false)
		return std::vector<KeyDefine::KeyCode>();

	return (*keyList)[name];
}

//====================================================================================================
// ▼先行入力

std::list<KeyInfo> InputManager::LatestInput(int num) {

	// 直近に押されたキー情報を保持する
	std::list<KeyInfo> keyCodes;

	// 全ての入力状態を調べる。入力されていたら、その情報を保持する
	for (const auto& itr : GetKeyAll()) {
		if (InputManager::Hold(itr.first, num))
			keyCodes.push_back(itr.second);
	}

	// 何も入力が無い場合、Noneを入れる。
	if (keyCodes.size() == 0)
		keyCodes.push_back(KeyInfo{ nullptr, KeyCode::None, DeviceType::None });

	// 保持した情報を返す。
	return keyCodes;
}

void InputManager::AdvancedEntryUpdate(int num, float _advancedEntry) {

	if (advancedEntry == nullptr)
		advancedEntry = new std::unordered_map<int, std::list<AdvancedEntryInfo>>();

	// 直近の入力情報を取得する
	std::list<KeyInfo> keyCodes = InputManager::LatestInput(num);

	for (const auto& itr : keyCodes) {
		if (itr.value == nullptr)
			continue;

		// 入力されていれば、その情報を保持
		(*advancedEntry)[num].push_back(AdvancedEntryInfo(itr, _advancedEntry));
	}
}

bool InputManager::AdvancedEntry(KeyDefine::KeyCode keyCode, int num) {

	if (advancedEntry == nullptr)
		return false;

	for (const auto& itr : (*advancedEntry)[num]) {
		if (itr.keyInfo.keyCode == keyCode)
			return true;
	}

	return false;
}

bool InputManager::AdvancedEntry(std::string name, int num) {

	if (keyList == nullptr || advancedEntry == nullptr)
		return false;

	for (auto k : (*keyList)[name]) {
		for (const auto& itr : (*advancedEntry)[num]) {
			if (itr.keyInfo.keyCode == k)
				return true;
		}
	}

	return false;
}

Vector3 InputManager::AnalogStick() {

	Vector3 analog = Vector3(PadController::NormalizedLeftStick().x, 0.0f, PadController::NormalizedLeftStick().y);

	if (Hold(KeyDefine::KeyCode::W))		analog.z = 1.0f;
	if (Hold(KeyDefine::KeyCode::S))		analog.z = -1.0f;
	if (Hold(KeyDefine::KeyCode::D))		analog.x = 1.0f;
	if (Hold(KeyDefine::KeyCode::A))		analog.x = -1.0f;

	if (analog.Size() > 1.0f) analog = analog.Norm();	// 1を超えないようにリミッターをかける

	return analog;
}

#include "../Library/magic_enum.hpp"

void InputManager::DrawTest(float x, float y) {

	std::list<InputData> inputData;

	// 入力データを別のリストに取得
	for (const auto& itr : *isInputs) {
		if (itr.second.isPut) {
			inputData.push_back(itr.second);
		}
	}

	int pushNum = 0;

	// 入力データを画面に表示
	for (const auto& itr : inputData) {
		std::string sKeyName = magic_enum::enum_name(itr.keyInfo.keyCode).data();
		std::string sPutTime = Function::FormatToString("time %.2f", itr.putTime);

		DrawFormatStringF(x, y + 25.0f * pushNum, GetColor(255, 255, 255), (sKeyName + ":" + sPutTime).c_str());
		pushNum++;
	}
}
