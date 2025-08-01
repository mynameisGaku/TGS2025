#include "GameRef.h"
#include "src/util/file/json/settings_json.h"

GameRef* GameRef::instance = nullptr;

GameRef& GameRef::Inst()
{
    if (instance == nullptr)
    {
        instance = new GameRef;
    }
    return (*instance);
}

void GameRef::Destroy()
{
    if (instance)
    {
        delete instance;
    }
    instance = nullptr;
}

void GameRef::Load(bool forceLoad)
{
    if (not forceLoad && m_WasLoad)
    {
        return;
    }

    // 諸々の初期化
    auto jsonLoader = Settings_json::Inst();

    // JSON 読み込み
    jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

    auto& rawJson = jsonLoader->GetJson(FILEKEY);
    
    for (const auto& data : rawJson["TeamNames"])
    {
        std::string name = data.value<std::string>("Name", "No Information.");
        TeamNames.push_back(name);
    }

    for (const auto& data : rawJson["StageNames"])
    {
        std::string name = data.value<std::string>("Name", "No Information.");
        StageNames.push_back(name);
    }

    for (const auto& data : rawJson["GameModeDescs"])
    {
        GameModeDesc desc = {};
        
        std::string name = data.value<std::string>("GameModeName", "No Infomation.");
        desc.GameModeName = name;
        desc.PlayTimeMaxSec = data.value<float>("PlayTimeMaxSec", 0.0f);
        desc.WinPointMax = data.value<int>("WinPointMax", 0);
        GameModeDescs[name] = desc;
    }

    for (auto& desc : GameModeDescs)
    {
        GameModeNames.push_back(desc.first);
    }

    GameStartCountMaxSec = jsonLoader->GetOrDefault<float>("GameStartCountMaxSec", 0.0f, FILEKEY);
    GameEndCountMaxSec   = jsonLoader->GetOrDefault<float>("GameEndCountMaxSec", 0.0f, FILEKEY);
    DefaultStageName     = jsonLoader->GetOrDefault<std::string>("DefaultStageName", std::string("Stage_3"), FILEKEY);

    m_WasLoad = true;
}
