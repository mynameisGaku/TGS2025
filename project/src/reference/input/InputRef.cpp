#include "InputRef.h"
#include "src/util/file/json/settings_json.h"

InputRef* InputRef::instance = nullptr;

InputRef& InputRef::Inst()
{
    if (!instance) instance = new InputRef;
    return *instance;
}

void InputRef::Destroy()
{
    delete instance;
    instance = nullptr;
}

void InputRef::Load(bool forceLoad)
{
    if (!forceLoad && m_WasLoad) return;

    PhysicalKeys.clear();
    VirtualKeys.clear();

    auto jsonLoader = Settings_json::Inst();
    jsonLoader->LoadSettingJson(PHYSICALKEY_PATH, PHYSICALKEY_FILEKEY);
    jsonLoader->LoadSettingJson(VIRTUALKEY_PATH, VIRTUALKEY_FILEKEY);
    auto& physJson = jsonLoader->GetJson(PHYSICALKEY_FILEKEY);
    auto& virtJson = jsonLoader->GetJson(VIRTUALKEY_FILEKEY);

    // 物理キー
    for (const auto& physData : physJson["Params"])
    {
        PHYS_KEY_PARAM param{};
        param.DeviceName = physData.value<std::string>("Device", "ERR");
        if (param.DeviceName == "ERR")
            continue;
        param.KeyName = physData.value<std::string>("KeyName", "ERR");
        if (param.KeyName == "ERR")
            continue;
        param.KeyParam = physData.value<int>("KeyParam", -1);

        PhysicalKeys.push_back(param);
    }

    // 仮想キー
    for (const auto& virtData : virtJson["Params"])
    {
        VIRT_KEY_PARAM param{};
        param.KeyName = virtData.value<std::string>("KeyName", "ERR");
        if (param.KeyName == "ERR")
            continue;

        std::vector<std::string> params{};
        std::vector<std::string> virtParams = virtData["KeyParams"];
        for (const auto& param : virtParams)
        {
            params.push_back(param);
        }
        param.KeyParams = params;

        VirtualKeys.push_back(param);
    }

    m_WasLoad = true;
}
