#include "src/reference/chara/CharaStaminaRef.h"
#include "src/util/file/json/settings_json.h"

CharaStaminaRef* CharaStaminaRef::instance = nullptr;

void CharaStaminaRef::Load(bool ForceLoad)
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
    Max             = jsonLoader->GetOrDefault<float>("Param.Max",            0.0f, FILEKEY);
    RegenTakesTime  = jsonLoader->GetOrDefault<float>("Param.RegenTakesTime", 0.0f, FILEKEY);
    RegenStartTime  = jsonLoader->GetOrDefault<float>("Param.RegenStartTime", 0.0f, FILEKEY);
    RegenStartTime_AllLost = jsonLoader->GetOrDefault<float>("Param.RegenStartTime_AllLost", 0.0f, FILEKEY);

    // ロードしたよ
    m_WasLoad = true;
}
