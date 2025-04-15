#include "keyDefine.h"
#include "../Library/myDxLib.h"

namespace {

	std::unordered_map<KeyDefine::KeyCode, KeyDefine::KeyInfo>* keys;
	std::list<KeyDefine::KeyInfo>* keyInputInfo;
	std::list<KeyDefine::KeyInfo>* padInputInfo;
	std::list<KeyDefine::KeyInfo>* mouseInputInfo;
}

void KeyDefine::Init() {

	if (keys != nullptr)
		return;

	keys = new std::unordered_map<KeyCode, KeyInfo>;

	auto& key = (*keys);

	key[KeyCode::None]			= KeyInfo{ new int { -1 },								KeyCode::None,			DeviceType::None };
	key[KeyCode::Up]			= KeyInfo{ new int { XINPUT_BUTTON_DPAD_UP },			KeyCode::Up,			DeviceType::Pad };
	key[KeyCode::Down]			= KeyInfo{ new int { XINPUT_BUTTON_DPAD_DOWN },			KeyCode::Down,			DeviceType::Pad };
	key[KeyCode::Left]			= KeyInfo{ new int { XINPUT_BUTTON_DPAD_LEFT },			KeyCode::Left,			DeviceType::Pad };
	key[KeyCode::Right]			= KeyInfo{ new int { XINPUT_BUTTON_DPAD_RIGHT },		KeyCode::Right,			DeviceType::Pad };
	key[KeyCode::Start]			= KeyInfo{ new int { XINPUT_BUTTON_START },				KeyCode::Start,			DeviceType::Pad };
	key[KeyCode::Back]			= KeyInfo{ new int { XINPUT_BUTTON_BACK },				KeyCode::Back,			DeviceType::Pad };
	key[KeyCode::LeftThumb]		= KeyInfo{ new int { XINPUT_BUTTON_LEFT_THUMB },		KeyCode::LeftThumb,		DeviceType::Pad };
	key[KeyCode::RightThumb]	= KeyInfo{ new int { XINPUT_BUTTON_RIGHT_THUMB },		KeyCode::RightThumb,	DeviceType::Pad };
	key[KeyCode::LeftShoulder]	= KeyInfo{ new int { XINPUT_BUTTON_LEFT_SHOULDER },		KeyCode::LeftShoulder,	DeviceType::Pad };
	key[KeyCode::RightShoulder]	= KeyInfo{ new int { XINPUT_BUTTON_RIGHT_SHOULDER },	KeyCode::RightShoulder,	DeviceType::Pad };
	key[KeyCode::ButtonA]		= KeyInfo{ new int { XINPUT_BUTTON_A },					KeyCode::ButtonA,		DeviceType::Pad };
	key[KeyCode::ButtonB]		= KeyInfo{ new int { XINPUT_BUTTON_B },					KeyCode::ButtonB,		DeviceType::Pad };
	key[KeyCode::ButtonX]		= KeyInfo{ new int { XINPUT_BUTTON_X },					KeyCode::ButtonX,		DeviceType::Pad };
	key[KeyCode::ButtonY]		= KeyInfo{ new int { XINPUT_BUTTON_Y },					KeyCode::ButtonY,		DeviceType::Pad };

	key[KeyCode::A]				= KeyInfo{ new int { KEY_INPUT_A },						KeyCode::A,				DeviceType::Key };
	key[KeyCode::B]				= KeyInfo{ new int { KEY_INPUT_B },						KeyCode::B,				DeviceType::Key };
	key[KeyCode::C]				= KeyInfo{ new int { KEY_INPUT_C },						KeyCode::C,				DeviceType::Key };
	key[KeyCode::D]				= KeyInfo{ new int { KEY_INPUT_D },						KeyCode::D,				DeviceType::Key };
	key[KeyCode::E]				= KeyInfo{ new int { KEY_INPUT_E },						KeyCode::E,				DeviceType::Key };
	key[KeyCode::F]				= KeyInfo{ new int { KEY_INPUT_F },						KeyCode::F,				DeviceType::Key };
	key[KeyCode::G]				= KeyInfo{ new int { KEY_INPUT_G },						KeyCode::G,				DeviceType::Key };
	key[KeyCode::H]				= KeyInfo{ new int { KEY_INPUT_H },						KeyCode::H,				DeviceType::Key };
	key[KeyCode::I]				= KeyInfo{ new int { KEY_INPUT_I },						KeyCode::I,				DeviceType::Key };
	key[KeyCode::J]				= KeyInfo{ new int { KEY_INPUT_J },						KeyCode::J,				DeviceType::Key };
	key[KeyCode::K]				= KeyInfo{ new int { KEY_INPUT_K },						KeyCode::K,				DeviceType::Key };
	key[KeyCode::L]				= KeyInfo{ new int { KEY_INPUT_L },						KeyCode::L,				DeviceType::Key };
	key[KeyCode::M]				= KeyInfo{ new int { KEY_INPUT_M },						KeyCode::M,				DeviceType::Key };
	key[KeyCode::N]				= KeyInfo{ new int { KEY_INPUT_N },						KeyCode::N,				DeviceType::Key };
	key[KeyCode::O]				= KeyInfo{ new int { KEY_INPUT_O },						KeyCode::O,				DeviceType::Key };
	key[KeyCode::P]				= KeyInfo{ new int { KEY_INPUT_P },						KeyCode::P,				DeviceType::Key };
	key[KeyCode::Q]				= KeyInfo{ new int { KEY_INPUT_Q },						KeyCode::Q,				DeviceType::Key };
	key[KeyCode::R]				= KeyInfo{ new int { KEY_INPUT_R },						KeyCode::R,				DeviceType::Key };
	key[KeyCode::S]				= KeyInfo{ new int { KEY_INPUT_S },						KeyCode::S,				DeviceType::Key };
	key[KeyCode::T]				= KeyInfo{ new int { KEY_INPUT_T },						KeyCode::T,				DeviceType::Key };
	key[KeyCode::U]				= KeyInfo{ new int { KEY_INPUT_U },						KeyCode::U,				DeviceType::Key };
	key[KeyCode::V]				= KeyInfo{ new int { KEY_INPUT_V },						KeyCode::V,				DeviceType::Key };
	key[KeyCode::W]				= KeyInfo{ new int { KEY_INPUT_W },						KeyCode::W,				DeviceType::Key };
	key[KeyCode::X]				= KeyInfo{ new int { KEY_INPUT_X },						KeyCode::X,				DeviceType::Key };
	key[KeyCode::Y]				= KeyInfo{ new int { KEY_INPUT_Y },						KeyCode::Y,				DeviceType::Key };
	key[KeyCode::Z]				= KeyInfo{ new int { KEY_INPUT_Z },						KeyCode::Z,				DeviceType::Key };
	key[KeyCode::Alpha0]		= KeyInfo{ new int { KEY_INPUT_0 },						KeyCode::Alpha0,		DeviceType::Key };
	key[KeyCode::Alpha1]		= KeyInfo{ new int { KEY_INPUT_1 },						KeyCode::Alpha1,		DeviceType::Key };
	key[KeyCode::Alpha2]		= KeyInfo{ new int { KEY_INPUT_2 },						KeyCode::Alpha2,		DeviceType::Key };
	key[KeyCode::Alpha3]		= KeyInfo{ new int { KEY_INPUT_3 },						KeyCode::Alpha3,		DeviceType::Key };
	key[KeyCode::Alpha4]		= KeyInfo{ new int { KEY_INPUT_4 },						KeyCode::Alpha4,		DeviceType::Key };
	key[KeyCode::Alpha5]		= KeyInfo{ new int { KEY_INPUT_5 },						KeyCode::Alpha5,		DeviceType::Key };
	key[KeyCode::Alpha6]		= KeyInfo{ new int { KEY_INPUT_6 },						KeyCode::Alpha6,		DeviceType::Key };
	key[KeyCode::Alpha7]		= KeyInfo{ new int { KEY_INPUT_7 },						KeyCode::Alpha7,		DeviceType::Key };
	key[KeyCode::Alpha8]		= KeyInfo{ new int { KEY_INPUT_8 },						KeyCode::Alpha8,		DeviceType::Key };
	key[KeyCode::Alpha9]		= KeyInfo{ new int { KEY_INPUT_9 },						KeyCode::Alpha9,		DeviceType::Key };
	key[KeyCode::F1]			= KeyInfo{ new int { KEY_INPUT_F1 },					KeyCode::F1,			DeviceType::Key };
	key[KeyCode::F2]			= KeyInfo{ new int { KEY_INPUT_F2 },					KeyCode::F2,			DeviceType::Key };
	key[KeyCode::F3]			= KeyInfo{ new int { KEY_INPUT_F3 },					KeyCode::F3,			DeviceType::Key };
	key[KeyCode::F4]			= KeyInfo{ new int { KEY_INPUT_F4 },					KeyCode::F4,			DeviceType::Key };
	key[KeyCode::F5]			= KeyInfo{ new int { KEY_INPUT_F5 },					KeyCode::F5,			DeviceType::Key };
	key[KeyCode::F6]			= KeyInfo{ new int { KEY_INPUT_F6 },					KeyCode::F6,			DeviceType::Key };
	key[KeyCode::F7]			= KeyInfo{ new int { KEY_INPUT_F7 },					KeyCode::F7,			DeviceType::Key };
	key[KeyCode::F8]			= KeyInfo{ new int { KEY_INPUT_F8 },					KeyCode::F8,			DeviceType::Key };
	key[KeyCode::F9]			= KeyInfo{ new int { KEY_INPUT_F9 },					KeyCode::F9,			DeviceType::Key };
	key[KeyCode::F10]			= KeyInfo{ new int { KEY_INPUT_F10 },					KeyCode::F10,			DeviceType::Key };
	key[KeyCode::F11]			= KeyInfo{ new int { KEY_INPUT_F11 },					KeyCode::F11,			DeviceType::Key };
	key[KeyCode::F12]			= KeyInfo{ new int { KEY_INPUT_F12 },					KeyCode::F12,			DeviceType::Key };
	key[KeyCode::BackSpace]		= KeyInfo{ new int { KEY_INPUT_BACK },					KeyCode::BackSpace,		DeviceType::Key };
	key[KeyCode::Tab]			= KeyInfo{ new int { KEY_INPUT_TAB },					KeyCode::Tab,			DeviceType::Key };
	key[KeyCode::Enter]			= KeyInfo{ new int { KEY_INPUT_RETURN },				KeyCode::Enter,			DeviceType::Key };
	key[KeyCode::LeftShift]		= KeyInfo{ new int { KEY_INPUT_LSHIFT },				KeyCode::LeftShift,		DeviceType::Key };
	key[KeyCode::RightShift]	= KeyInfo{ new int { KEY_INPUT_RSHIFT },				KeyCode::RightShift,	DeviceType::Key };
	key[KeyCode::LeftControl]	= KeyInfo{ new int { KEY_INPUT_LCONTROL },				KeyCode::LeftControl,	DeviceType::Key };
	key[KeyCode::RightControl]	= KeyInfo{ new int { KEY_INPUT_RCONTROL },				KeyCode::RightControl,	DeviceType::Key };
	key[KeyCode::Escape]		= KeyInfo{ new int { KEY_INPUT_ESCAPE },				KeyCode::Escape,		DeviceType::Key };
	key[KeyCode::Space]			= KeyInfo{ new int { KEY_INPUT_SPACE },					KeyCode::Space,			DeviceType::Key };
	key[KeyCode::PageUp]		= KeyInfo{ new int { KEY_INPUT_PGUP },					KeyCode::PageUp,		DeviceType::Key };
	key[KeyCode::PageDown]		= KeyInfo{ new int { KEY_INPUT_PGDN },					KeyCode::PageDown,		DeviceType::Key };
	key[KeyCode::End]			= KeyInfo{ new int { KEY_INPUT_END },					KeyCode::End,			DeviceType::Key };
	key[KeyCode::Home]			= KeyInfo{ new int { KEY_INPUT_HOME },					KeyCode::Home,			DeviceType::Key };
	key[KeyCode::UpArrow]		= KeyInfo{ new int { KEY_INPUT_UP },					KeyCode::UpArrow,		DeviceType::Key };
	key[KeyCode::DownArrow]		= KeyInfo{ new int { KEY_INPUT_DOWN },					KeyCode::DownArrow,		DeviceType::Key };
	key[KeyCode::LeftArrow]		= KeyInfo{ new int { KEY_INPUT_LEFT },					KeyCode::LeftArrow,		DeviceType::Key };
	key[KeyCode::RightArrow]	= KeyInfo{ new int { KEY_INPUT_RIGHT },					KeyCode::RightArrow,	DeviceType::Key };
	key[KeyCode::Insert]		= KeyInfo{ new int { KEY_INPUT_INSERT },				KeyCode::Insert,		DeviceType::Key };
	key[KeyCode::Delete]		= KeyInfo{ new int { KEY_INPUT_DELETE },				KeyCode::Delete,		DeviceType::Key };

	key[KeyCode::LeftClick]		= KeyInfo{ new int { MOUSE_INPUT_LEFT },				KeyCode::LeftClick,		DeviceType::Mouse };
	key[KeyCode::RightClick]	= KeyInfo{ new int { MOUSE_INPUT_RIGHT },				KeyCode::RightClick,	DeviceType::Mouse };
	key[KeyCode::MiddleClick]	= KeyInfo{ new int { MOUSE_INPUT_MIDDLE },				KeyCode::MiddleClick,	DeviceType::Mouse };
	key[KeyCode::MouseButton1]	= KeyInfo{ new int { MOUSE_INPUT_1 },					KeyCode::MouseButton1,	DeviceType::Mouse };
	key[KeyCode::MouseButton2]	= KeyInfo{ new int { MOUSE_INPUT_2 },					KeyCode::MouseButton2,	DeviceType::Mouse };
	key[KeyCode::MouseButton3]	= KeyInfo{ new int { MOUSE_INPUT_3 },					KeyCode::MouseButton3,	DeviceType::Mouse };
	key[KeyCode::MouseButton4]	= KeyInfo{ new int { MOUSE_INPUT_4 },					KeyCode::MouseButton4,	DeviceType::Mouse };
	key[KeyCode::MouseButton5]	= KeyInfo{ new int { MOUSE_INPUT_5 },					KeyCode::MouseButton5,	DeviceType::Mouse };

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
		keyInputInfo = new std::list<KeyDefine::KeyInfo>();

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
		padInputInfo = new std::list<KeyDefine::KeyInfo>();

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
		mouseInputInfo = new std::list<KeyDefine::KeyInfo>();

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

std::list<KeyDefine::KeyInfo> KeyDefine::DeviceTypeToKeyInfo(DeviceType type) {

	switch (type) {
	case KeyDefine::DeviceType::Key:	return *keyInputInfo;	break;
	case KeyDefine::DeviceType::Pad:	return *padInputInfo;	break;
	case KeyDefine::DeviceType::Mouse:	return *mouseInputInfo;	break;
	default:	return std::list<KeyDefine::KeyInfo>();			break;
	}

	return std::list<KeyDefine::KeyInfo>();
}

std::unordered_map<KeyDefine::KeyCode, KeyDefine::KeyInfo> KeyDefine::GetKeyAll() {

	if (keys == nullptr)
		return std::unordered_map<KeyCode, KeyInfo>();

	return (*keys);
}
