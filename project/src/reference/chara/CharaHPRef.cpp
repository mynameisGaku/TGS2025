#include "src/reference/chara/CharaHPRef.h"
#include "src/util/file/json/settings_json.h"

void CharaHPRef::Load(bool ForceLoad)
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
    Max = jsonLoader->GetOrDefault<float>("Param.Max", 0.0f, FILEKEY);
    Damage = jsonLoader->GetOrDefault<float>("Param.Damage", 0.0f, FILEKEY);
    Heal = jsonLoader->GetOrDefault<float>("Param.Heal", 0.0f, FILEKEY);

    // ロードしたよ
    m_WasLoad = true;
}
