#include "src/util/input/keyDefine.h"
#include "framework/myDxLib.h"
#include "src/reference/input/InputRef.h"
#include <src/util/enum/EnumUtil.h>

namespace {

    std::unordered_map<KeyDefine::KeyCode, KeyDefine::InputData>* keys;
    std::list<KeyDefine::InputData>* keyInputInfo;
    std::list<KeyDefine::InputData>* padInputInfo;
    std::list<KeyDefine::InputData>* mouseInputInfo;

    InputRef* pRef;
}

void KeyDefine::Init() {

    if (keys != nullptr)
        return;

    pRef = &InputRef::Inst();
    pRef->Load(true);


    keys = new std::unordered_map<KeyCode, InputData>;

    auto& key = (*keys);

    for (auto& phys : pRef->PhysicalKeys)
    {
        DeviceType device{};
        device = EnumUtil::ConvertFromString(phys.DeviceName, DeviceType::None);
        KeyCode keyCode{};
        keyCode = EnumUtil::ConvertFromString(phys.KeyName, KeyCode::None);
        int* param = new int{ -1 };
        *param = phys.KeyParam;
        key[keyCode] = InputData(param, keyCode, device);
    }

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
