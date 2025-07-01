#include "src/util/input/InputManager.h"

// ◇汎用
#include "src/util/time/GameTime.h"
#include <vendor/magic_enum/magic_enum.hpp>

#include "src/util/input/KeyController.h"
#include "src/util/input/PadController.h"
#include "src/util/input/MouseController.h"
#include "src/util/enum/EnumUtil.h"

// 参照
#include "src/reference/input/InputRef.h"

// ◇デバッグ
#include "src/util/debug/imgui/imGuiManager.h"
#include <src/util/file/resource_loader/resourceLoader.h>

using namespace KeyDefine;

namespace {

	std::unordered_map<std::string, std::vector<KeyCode>>* keyList;			// 仮想ボタン
	std::unordered_map<int, std::list<AdvancedEntryInfo>>* advancedEntry;	// 先行入力

	std::unordered_map<KeyCode, InputData>* isInputs;		// 入力しているか
	std::unordered_map<KeyCode, InputData>* isBeforeInputs;	// 1フレーム前に入力しているか

	std::list<InputData> keyCodes;	// 直近に押されたキー情報を保持する

	InputRef* pRef;

	struct BUTTON_IMAGE
	{
		int hImage = -1;
		int hPushImage = -1;
	};
	std::unordered_map<std::string, BUTTON_IMAGE>* pButtonImageMap;
}

void InputManager::Init() {

	//==========================================================================================
	// ▼各種入力情報を初期化

	KeyDefine::Init();

	KeyController::Init();
	PadController::Init();
	MouseController::Init();

	pRef = &InputRef::Inst();
	pRef->Load(true);

	pButtonImageMap = new std::unordered_map<std::string, BUTTON_IMAGE>;

	// 画像読み込み & マップ登録
	const std::string image_path	= "data/Img/UI/ButtonHint/";
	std::string push				= "_Push";
	std::string device_type_str		= "None";
	std::string file_extension		= ".png";
	for (auto& phys : pRef->PhysicalKeys)
	{
		std::string key = phys.KeyName;

		BUTTON_IMAGE button;
		KeyDefine::DeviceType device = EnumUtil::ToEnum(phys.DeviceName, KeyDefine::DeviceType::None);

		switch (device)
		{
		case KeyDefine::DeviceType::Key:
			device_type_str = "Key/";
			break;
		case KeyDefine::DeviceType::Pad:
			device_type_str = "Pad/";
			break;
		case KeyDefine::DeviceType::Mouse:
			device_type_str = "Mouse/";
			break;
		}

		std::string hImagePath = image_path + device_type_str + phys.KeyName + file_extension;
		std::string hPushImagePath = image_path + device_type_str + phys.KeyName + push + file_extension;
		button.hImage = ResourceLoader::LoadGraph(hImagePath);
		button.hPushImage = ResourceLoader::LoadGraph(hPushImagePath);
		(*pButtonImageMap)[key] = button;
	}

	//==========================================================================================
	// ▼仮想ボタンを割り当てる

	if (keyList == nullptr) {
		keyList = new std::unordered_map<std::string, std::vector<KeyCode>>();

		auto& key = (*keyList);
		for (auto& vir : pRef->VirtualKeys)
		{
			std::vector<KeyCode> codes;
			for (auto& keyparam : vir.KeyParams)
			{
				codes.push_back(EnumUtil::ToEnum(keyparam, KeyCode::None));
			}
			key[vir.KeyName] = codes;
		}
	}

	//==========================================================================================
	// ▼先行入力

	if (advancedEntry == nullptr) {
		advancedEntry = new std::unordered_map<int, std::list<AdvancedEntryInfo>>();

		for (int i = DX_INPUT_PAD1; i <= PAD_NUMBER_MAX; i++) {
			(*advancedEntry)[i] = std::list<AdvancedEntryInfo>();
		}
	}

	//==========================================================================================
	// ▼入力情報

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

	// 全てのキー入力
	const std::unordered_map<KeyCode, InputData> keyAll = GetKeyAll();

	// 全ての入力状態を調べる。入力されていたら、その情報を保持する
	for (const auto& itr : keyAll) {
		(*isInputs)[itr.first].Update();
	}

	InputDataUpdate();

	// 先行入力用に入力情報を取得する
	for (int i = DX_INPUT_PAD1; i <= PAD_NUMBER_MAX; i++)
		InputManager::AdvancedEntryUpdate(i, ADVANCED_ENTRY_TIME);
}

void InputManager::InputDataUpdate() {

	if (isInputs == nullptr || isBeforeInputs == nullptr)
		return;

	// 入力情報の更新
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

	// 保持されている先行入力の種類数で回す
	for (auto info = (*advancedEntry)[padNumber].begin(); info != (*advancedEntry)[padNumber].end();) {

		// 保持時間の経過
		info->saveTime = max(info->saveTime - GTime.deltaTime, 0.0f);

		// 保持時間が0.0以下の場合、切り離す
		if (info->saveTime <= 0.0f) {
			info = (*advancedEntry)[padNumber].erase(info);
			if (info == (*advancedEntry)[padNumber].end()) break;
		}
		else
			info++;
	}
}

//====================================================================================================
// ▼Push

bool InputManager::Push(const KeyDefine::KeyCode& keyCode, const int& padNumber) {

	if (isInputs == nullptr)
		return false;

	// 入力情報
	InputData inputData = (*isInputs)[keyCode];

	// 既に入力を受け付けていた場合
	if (inputData.isAccepted[padNumber][TouchPhase::Begin])
		return inputData.isInput[padNumber][TouchPhase::Begin];

	DeviceType device = KeyCodeToDeviceType(keyCode);	// デバイスの種類
	bool isInput = false;	// 押下されたか

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
// ▼Hold

bool InputManager::Hold(const KeyDefine::KeyCode& keyCode, const int& padNumber) {

	if (isInputs == nullptr)
		return false;

	// 入力情報
	InputData inputData = (*isInputs)[keyCode];

	// 既に入力を受け付けていた場合
	if (inputData.isAccepted[padNumber][TouchPhase::Moved])
		return inputData.isInput[padNumber][TouchPhase::Moved];

	DeviceType device = KeyCodeToDeviceType(keyCode);	// デバイスの種類
	bool isInput = false;	// 押下されたか

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
// ▼Release

bool InputManager::Release(const KeyDefine::KeyCode& keyCode, const int& padNumber) {

	if (isInputs == nullptr)
		return false;

	// 入力情報
	InputData inputData = (*isInputs)[keyCode];

	// 既に入力を受け付けていた場合
	if (inputData.isAccepted[padNumber][TouchPhase::Ended])
		return inputData.isInput[padNumber][TouchPhase::Ended];

	DeviceType device = KeyCodeToDeviceType(keyCode);	// デバイスの種類
	bool isInput = false;	// 押下されたか

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
// ▼先行入力

std::list<InputData> InputManager::LatestInput(const int& padNumber) {

	// 直近に押されたキー情報を保持する
	std::list<InputData> keyCodes;

	if (isInputs == nullptr)
		return keyCodes;

	// 全ての入力状態を調べる。入力されていたら、その情報を保持する
	for (const auto& itr : *isInputs) {
		if (itr.second.isInput)
			keyCodes.push_back(itr.second);
	}

	// 保持した情報を返す。
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

	Vector3 analog = Vector3(PadController::NormalizedLeftStick(padNumber).x, 0.0f, PadController::NormalizedLeftStick(padNumber).y);

	if (Hold("MoveUp"))		analog.z = 1.0f;
	if (Hold("MoveDown"))	analog.z = -1.0f;
	if (Hold("MoveRight"))	analog.x = 1.0f;
	if (Hold("MoveLeft"))	analog.x = -1.0f;

	if (analog.GetLength() > 1.0f) analog = analog.Normalize();	// 1を超えないようにリミッターをかける

	return analog;
}

std::pair<int, int> InputManager::GetImagePair(const KeyDefine::KeyCode& keyCode)
{
	return GetImagePair(EnumUtil::ToString(keyCode));
}

std::pair<int, int> InputManager::GetImagePair(const std::string& keyName)
{
	std::pair<int, int> ret{};
	ret.first = (*pButtonImageMap)[keyName].hImage;
	ret.second = (*pButtonImageMap)[keyName].hPushImage;
	return ret;
}

#ifdef _DEBUG

#include <vendor/magic_enum/magic_enum.hpp>
#include "src/util/string/StringUtil.h"

void InputManager::DrawTest(const float& x, const float& y) {

	if (isInputs == nullptr)
		return;

	std::list<InputData> inputData;

	// 入力データを別のリストに取得
	for (auto& itr : *isInputs) {
		if (itr.second.isInput[DX_INPUT_PAD1][TouchPhase::Moved]) {
			inputData.push_back(itr.second);
		}
	}

	int pushNum = 0;

	// 入力データを画面に表示
	for (const auto& itr : inputData) {
		std::string sKeyName = magic_enum::enum_name(itr.keyCode).data();
		std::string sPushTime = StringUtil::FormatToString("time %.2f", itr.pushTime);

		DrawFormatStringF(x, y + 25.0f * pushNum, GetColor(255, 255, 255), (sKeyName + ":" + sPushTime).c_str());
		pushNum++;
	}
}

#endif // _DEBUG