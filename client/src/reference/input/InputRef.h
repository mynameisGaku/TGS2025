#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "src/util/input/KeyDefine.h"

class InputRef
{
private:
    const std::string VIRTUALKEY_PATH = "data/Json/Input/VirtualKey.json";
    const std::string PHYSICALKEY_PATH = "data/Json/Input/PhysicalKey.json";

    const std::string VIRTUALKEY_FILEKEY = "Input_VirtualKey";
    const std::string PHYSICALKEY_FILEKEY = "Input_PhysicalKey";
public:
    static InputRef& Inst();
    static void Destroy();
    void Load(bool forceLoad = false);

    struct PHYS_KEY_PARAM
    {
        std::string DeviceName;
        int KeyParam;
        std::string KeyName;
    };

    struct VIRT_KEY_PARAM
    {
        std::string KeyName;
        std::vector<std::string> KeyParams;
    };

    std::vector<PHYS_KEY_PARAM> PhysicalKeys;
    std::vector<VIRT_KEY_PARAM> VirtualKeys;

private:
    static InputRef* instance;
    bool m_WasLoad = false;
};