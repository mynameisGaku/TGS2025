#include "src/reference/chara/CharaDefineRef.h"
#include "src/util/file/json/settings_json.h"

CharaDefineRef* CharaDefineRef::instance = nullptr;

void CharaDefineRef::Load(bool ForceLoad)
{
    if (not ForceLoad && m_WasLoad)
    {
        return;
    }

    // 諸々の初期化
    auto jsonLoader = Settings_json::Inst();

    // JSON 読み込み
    jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

    // 各種値取得
    Max = jsonLoader->GetOrDefault<int>("Param.Max", 0, FILEKEY);
    auto Tags_tmp = jsonLoader->Get<std::vector<std::string>>("Param.Tags", FILEKEY);
    for (auto& tmp : Tags_tmp)
    {
        Tags.push_back(tmp);
    }
    CatchRadius = jsonLoader->GetOrDefault<float>("Param.CatchRadius", 0.0f, FILEKEY);
    EmoteInterval = jsonLoader->GetOrDefault<float>("Param.EmoteInterval", 0.0f, FILEKEY);
    MoveSpeed = jsonLoader->GetOrDefault<float>("Param.MoveSpeed", 0.0f, FILEKEY);
    RotSpeed = jsonLoader->GetOrDefault<float>("Param.RotSpeed", 0.0f, FILEKEY);
    JumpPower = jsonLoader->GetOrDefault<float>("Param.JumpPower", 0.0f, FILEKEY);
    TackleRadius = jsonLoader->GetOrDefault<float>("Param.TackleRadius", 0.0f, FILEKEY);
    TackleRecovery = jsonLoader->GetOrDefault<float>("Param.TackleRecovery", 0.0f, FILEKEY);
    TackleInterval = jsonLoader->GetOrDefault<float>("Param.TackleInterval", 0.0f, FILEKEY);
    TackleForce_Horizontal = jsonLoader->GetOrDefault<float>("Param.TackleForce_Horizontal", 0.0f, FILEKEY);
    TackleForce_Vertical = jsonLoader->GetOrDefault<float>("Param.TackleForce_Vertical", 0.0f, FILEKEY);
    TackleInvincibleDurationSec = jsonLoader->GetOrDefault<float>("Param.TackleInvincibleDurationSec", 0.0f, FILEKEY);
    GetTackleInvincibleTime = jsonLoader->GetOrDefault<float>("Param.GetTackleInvincibleTime", 0.0f, FILEKEY);

    // ロードしたよ
    m_WasLoad = true;
}
