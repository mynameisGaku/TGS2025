#include "src/util/input/keyDefine.h"
#include "framework/myDxLib.h"

namespace {

	std::unordered_map<KeyDefine::KeyCode, KeyDefine::InputData>* keys;
	std::list<KeyDefine::InputData>* keyInputInfo;
	std::list<KeyDefine::InputData>* padInputInfo;
	std::list<KeyDefine::InputData>* mouseInputInfo;
}

void KeyDefine::Init() {

	if (keys != nullptr)
		return;

	keys = new std::unordered_map<KeyCode, InputData>;

	auto& key = (*keys);

	key[KeyCode::None] = InputData(new int{ -1 }, KeyCode::None, DeviceType::None);
	key[KeyCode::Up] = InputData(new int{ XINPUT_BUTTON_DPAD_UP }, KeyCode::Up, DeviceType::Pad);
	key[KeyCode::Down] = InputData(new int{ XINPUT_BUTTON_DPAD_DOWN }, KeyCode::Down, DeviceType::Pad);
	key[KeyCode::Left] = InputData(new int{ XINPUT_BUTTON_DPAD_LEFT }, KeyCode::Left, DeviceType::Pad);
	key[KeyCode::Right] = InputData(new int{ XINPUT_BUTTON_DPAD_RIGHT }, KeyCode::Right, DeviceType::Pad);
	key[KeyCode::Start] = InputData(new int{ XINPUT_BUTTON_START }, KeyCode::Start, DeviceType::Pad);
	key[KeyCode::Back] = InputData(new int{ XINPUT_BUTTON_BACK }, KeyCode::Back, DeviceType::Pad);
	key[KeyCode::LeftThumb] = InputData(new int{ XINPUT_BUTTON_LEFT_THUMB }, KeyCode::LeftThumb, DeviceType::Pad);
	key[KeyCode::RightThumb] = InputData(new int{ XINPUT_BUTTON_RIGHT_THUMB }, KeyCode::RightThumb, DeviceType::Pad);
	key[KeyCode::LeftShoulder] = InputData(new int{ XINPUT_BUTTON_LEFT_SHOULDER }, KeyCode::LeftShoulder, DeviceType::Pad);
	key[KeyCode::RightShoulder] = InputData(new int{ XINPUT_BUTTON_RIGHT_SHOULDER }, KeyCode::RightShoulder, DeviceType::Pad);
	key[KeyCode::ButtonA] = InputData(new int{ XINPUT_BUTTON_A }, KeyCode::ButtonA, DeviceType::Pad);
	key[KeyCode::ButtonB] = InputData(new int{ XINPUT_BUTTON_B }, KeyCode::ButtonB, DeviceType::Pad);
	key[KeyCode::ButtonX] = InputData(new int{ XINPUT_BUTTON_X }, KeyCode::ButtonX, DeviceType::Pad);
	key[KeyCode::ButtonY] = InputData(new int{ XINPUT_BUTTON_Y }, KeyCode::ButtonY, DeviceType::Pad);

	key[KeyCode::A] = InputData(new int{ KEY_INPUT_A }, KeyCode::A, DeviceType::Key);
	key[KeyCode::B] = InputData(new int{ KEY_INPUT_B }, KeyCode::B, DeviceType::Key);
	key[KeyCode::C] = InputData(new int{ KEY_INPUT_C }, KeyCode::C, DeviceType::Key);
	key[KeyCode::D] = InputData(new int{ KEY_INPUT_D }, KeyCode::D, DeviceType::Key);
	key[KeyCode::E] = InputData(new int{ KEY_INPUT_E }, KeyCode::E, DeviceType::Key);
	key[KeyCode::F] = InputData(new int{ KEY_INPUT_F }, KeyCode::F, DeviceType::Key);
	key[KeyCode::G] = InputData(new int{ KEY_INPUT_G }, KeyCode::G, DeviceType::Key);
	key[KeyCode::H] = InputData(new int{ KEY_INPUT_H }, KeyCode::H, DeviceType::Key);
	key[KeyCode::I] = InputData(new int{ KEY_INPUT_I }, KeyCode::I, DeviceType::Key);
	key[KeyCode::J] = InputData(new int{ KEY_INPUT_J }, KeyCode::J, DeviceType::Key);
	key[KeyCode::K] = InputData(new int{ KEY_INPUT_K }, KeyCode::K, DeviceType::Key);
	key[KeyCode::L] = InputData(new int{ KEY_INPUT_L }, KeyCode::L, DeviceType::Key);
	key[KeyCode::M] = InputData(new int{ KEY_INPUT_M }, KeyCode::M, DeviceType::Key);
	key[KeyCode::N] = InputData(new int{ KEY_INPUT_N }, KeyCode::N, DeviceType::Key);
	key[KeyCode::O] = InputData(new int{ KEY_INPUT_O }, KeyCode::O, DeviceType::Key);
	key[KeyCode::P] = InputData(new int{ KEY_INPUT_P }, KeyCode::P, DeviceType::Key);
	key[KeyCode::Q] = InputData(new int{ KEY_INPUT_Q }, KeyCode::Q, DeviceType::Key);
	key[KeyCode::R] = InputData(new int{ KEY_INPUT_R }, KeyCode::R, DeviceType::Key);
	key[KeyCode::S] = InputData(new int{ KEY_INPUT_S }, KeyCode::S, DeviceType::Key);
	key[KeyCode::T] = InputData(new int{ KEY_INPUT_T }, KeyCode::T, DeviceType::Key);
	key[KeyCode::U] = InputData(new int{ KEY_INPUT_U }, KeyCode::U, DeviceType::Key);
	key[KeyCode::V] = InputData(new int{ KEY_INPUT_V }, KeyCode::V, DeviceType::Key);
	key[KeyCode::W] = InputData(new int{ KEY_INPUT_W }, KeyCode::W, DeviceType::Key);
	key[KeyCode::X] = InputData(new int{ KEY_INPUT_X }, KeyCode::X, DeviceType::Key);
	key[KeyCode::Y] = InputData(new int{ KEY_INPUT_Y }, KeyCode::Y, DeviceType::Key);
	key[KeyCode::Z] = InputData(new int{ KEY_INPUT_Z }, KeyCode::Z, DeviceType::Key);
	key[KeyCode::Alpha0] = InputData(new int{ KEY_INPUT_0 }, KeyCode::Alpha0, DeviceType::Key);
	key[KeyCode::Alpha1] = InputData(new int{ KEY_INPUT_1 }, KeyCode::Alpha1, DeviceType::Key);
	key[KeyCode::Alpha2] = InputData(new int{ KEY_INPUT_2 }, KeyCode::Alpha2, DeviceType::Key);
	key[KeyCode::Alpha3] = InputData(new int{ KEY_INPUT_3 }, KeyCode::Alpha3, DeviceType::Key);
	key[KeyCode::Alpha4] = InputData(new int{ KEY_INPUT_4 }, KeyCode::Alpha4, DeviceType::Key);
	key[KeyCode::Alpha5] = InputData(new int{ KEY_INPUT_5 }, KeyCode::Alpha5, DeviceType::Key);
	key[KeyCode::Alpha6] = InputData(new int{ KEY_INPUT_6 }, KeyCode::Alpha6, DeviceType::Key);
	key[KeyCode::Alpha7] = InputData(new int{ KEY_INPUT_7 }, KeyCode::Alpha7, DeviceType::Key);
	key[KeyCode::Alpha8] = InputData(new int{ KEY_INPUT_8 }, KeyCode::Alpha8, DeviceType::Key);
	key[KeyCode::Alpha9] = InputData(new int{ KEY_INPUT_9 }, KeyCode::Alpha9, DeviceType::Key);
	key[KeyCode::F1] = InputData(new int{ KEY_INPUT_F1 }, KeyCode::F1, DeviceType::Key);
	key[KeyCode::F2] = InputData(new int{ KEY_INPUT_F2 }, KeyCode::F2, DeviceType::Key);
	key[KeyCode::F3] = InputData(new int{ KEY_INPUT_F3 }, KeyCode::F3, DeviceType::Key);
	key[KeyCode::F4] = InputData(new int{ KEY_INPUT_F4 }, KeyCode::F4, DeviceType::Key);
	key[KeyCode::F5] = InputData(new int{ KEY_INPUT_F5 }, KeyCode::F5, DeviceType::Key);
	key[KeyCode::F6] = InputData(new int{ KEY_INPUT_F6 }, KeyCode::F6, DeviceType::Key);
	key[KeyCode::F7] = InputData(new int{ KEY_INPUT_F7 }, KeyCode::F7, DeviceType::Key);
	key[KeyCode::F8] = InputData(new int{ KEY_INPUT_F8 }, KeyCode::F8, DeviceType::Key);
	key[KeyCode::F9] = InputData(new int{ KEY_INPUT_F9 }, KeyCode::F9, DeviceType::Key);
	key[KeyCode::F10] = InputData(new int{ KEY_INPUT_F10 }, KeyCode::F10, DeviceType::Key);
	key[KeyCode::F11] = InputData(new int{ KEY_INPUT_F11 }, KeyCode::F11, DeviceType::Key);
	key[KeyCode::F12] = InputData(new int{ KEY_INPUT_F12 }, KeyCode::F12, DeviceType::Key);
	key[KeyCode::BackSpace] = InputData(new int{ KEY_INPUT_BACK }, KeyCode::BackSpace, DeviceType::Key);
	key[KeyCode::Tab] = InputData(new int{ KEY_INPUT_TAB }, KeyCode::Tab, DeviceType::Key);
	key[KeyCode::Enter] = InputData(new int{ KEY_INPUT_RETURN }, KeyCode::Enter, DeviceType::Key);
	key[KeyCode::LeftShift] = InputData(new int{ KEY_INPUT_LSHIFT }, KeyCode::LeftShift, DeviceType::Key);
	key[KeyCode::RightShift] = InputData(new int{ KEY_INPUT_RSHIFT }, KeyCode::RightShift, DeviceType::Key);
	key[KeyCode::LeftControl] = InputData(new int{ KEY_INPUT_LCONTROL }, KeyCode::LeftControl, DeviceType::Key);
	key[KeyCode::RightControl] = InputData(new int{ KEY_INPUT_RCONTROL }, KeyCode::RightControl, DeviceType::Key);
	key[KeyCode::Escape] = InputData(new int{ KEY_INPUT_ESCAPE }, KeyCode::Escape, DeviceType::Key);
	key[KeyCode::Space] = InputData(new int{ KEY_INPUT_SPACE }, KeyCode::Space, DeviceType::Key);
	key[KeyCode::PageUp] = InputData(new int{ KEY_INPUT_PGUP }, KeyCode::PageUp, DeviceType::Key);
	key[KeyCode::PageDown] = InputData(new int{ KEY_INPUT_PGDN }, KeyCode::PageDown, DeviceType::Key);
	key[KeyCode::End] = InputData(new int{ KEY_INPUT_END }, KeyCode::End, DeviceType::Key);
	key[KeyCode::Home] = InputData(new int{ KEY_INPUT_HOME }, KeyCode::Home, DeviceType::Key);
	key[KeyCode::UpArrow] = InputData(new int{ KEY_INPUT_UP }, KeyCode::UpArrow, DeviceType::Key);
	key[KeyCode::DownArrow] = InputData(new int{ KEY_INPUT_DOWN }, KeyCode::DownArrow, DeviceType::Key);
	key[KeyCode::LeftArrow] = InputData(new int{ KEY_INPUT_LEFT }, KeyCode::LeftArrow, DeviceType::Key);
	key[KeyCode::RightArrow] = InputData(new int{ KEY_INPUT_RIGHT }, KeyCode::RightArrow, DeviceType::Key);
	key[KeyCode::Insert] = InputData(new int{ KEY_INPUT_INSERT }, KeyCode::Insert, DeviceType::Key);
	key[KeyCode::Delete] = InputData(new int{ KEY_INPUT_DELETE }, KeyCode::Delete, DeviceType::Key);

	key[KeyCode::LeftClick] = InputData(new int{ MOUSE_INPUT_LEFT }, KeyCode::LeftClick, DeviceType::Mouse);
	key[KeyCode::RightClick] = InputData(new int{ MOUSE_INPUT_RIGHT }, KeyCode::RightClick, DeviceType::Mouse);
	key[KeyCode::MiddleClick] = InputData(new int{ MOUSE_INPUT_MIDDLE }, KeyCode::MiddleClick, DeviceType::Mouse);
	key[KeyCode::MouseButton1] = InputData(new int{ MOUSE_INPUT_1 }, KeyCode::MouseButton1, DeviceType::Mouse);
	key[KeyCode::MouseButton2] = InputData(new int{ MOUSE_INPUT_2 }, KeyCode::MouseButton2, DeviceType::Mouse);
	key[KeyCode::MouseButton3] = InputData(new int{ MOUSE_INPUT_3 }, KeyCode::MouseButton3, DeviceType::Mouse);
	key[KeyCode::MouseButton4] = InputData(new int{ MOUSE_INPUT_4 }, KeyCode::MouseButton4, DeviceType::Mouse);
	key[KeyCode::MouseButton5] = InputData(new int{ MOUSE_INPUT_5 }, KeyCode::MouseButton5, DeviceType::Mouse);

	SetKeyInputValue();
	SetPadInputValue();
	SetMouseInputValue();
}

void KeyDefine::Release() {

	if (keys != nullptr) {

		for (auto& itr : *keys) {
			if (itr.second.value != nullptr) {
				delete itr.second.value;
				itr.second.value = nullptr;
			}
		}

		keys->clear();
		delete keys;
		keys = nullptr;
	}

	if (keyInputInfo != nullptr) {
		keyInputInfo->clear();
		delete keyInputInfo;
		keyInputInfo = nullptr;
	}

	if (padInputInfo != nullptr) {
		padInputInfo->clear();
		delete padInputInfo;
		padInputInfo = nullptr;
	}

	if (mouseInputInfo != nullptr) {
		mouseInputInfo->clear();
		delete mouseInputInfo;
		mouseInputInfo = nullptr;
	}
}

void KeyDefine::SetKeyInputValue() {

	if (keys == nullptr)
		return;

	if (keyInputInfo == nullptr) {
		keyInputInfo = new std::list<KeyDefine::InputData>();

		for (const auto& itr : *keys) {
			if (itr.second.deviceType == DeviceType::Key)
				keyInputInfo->push_back(itr.second);
		}
	}
}

void KeyDefine::SetPadInputValue() {

	if (keys == nullptr)
		return;

	if (padInputInfo == nullptr) {
		padInputInfo = new std::list<KeyDefine::InputData>();

		for (const auto& itr : *keys) {
			if (itr.second.deviceType == DeviceType::Pad)
				padInputInfo->push_back(itr.second);
		}
	}
}

void KeyDefine::SetMouseInputValue() {

	if (keys == nullptr)
		return;

	if (mouseInputInfo == nullptr) {
		mouseInputInfo = new std::list<KeyDefine::InputData>();

		for (const auto& itr : *keys) {
			if (itr.second.deviceType == DeviceType::Mouse)
				mouseInputInfo->push_back(itr.second);
		}
	}
}

KeyDefine::DeviceType KeyDefine::KeyCodeToDeviceType(KeyCode key) {

	if (keys == nullptr)
		return DeviceType::None;

	return (*keys)[key].deviceType;
}

const std::list<KeyDefine::InputData> KeyDefine::DeviceTypeToInputData(DeviceType type) {

	switch (type) {
	case KeyDefine::DeviceType::Key:	return *keyInputInfo;	break;
	case KeyDefine::DeviceType::Pad:	return *padInputInfo;	break;
	case KeyDefine::DeviceType::Mouse:	return *mouseInputInfo;	break;
	default:	return std::list<KeyDefine::InputData>();		break;
	}

	return std::list<KeyDefine::InputData>();
}

const std::unordered_map<KeyDefine::KeyCode, KeyDefine::InputData> KeyDefine::GetKeyAll() {

	if (keys == nullptr)
		return std::unordered_map<KeyCode, InputData>();

	return (*keys);
}

const KeyDefine::InputData KeyDefine::GetInputData(KeyCode key) {

	if ((*keys).contains(key) == false)
		return InputData();

	return (*keys)[key];
}
