#include "src/reference/crystal/CrystalFragmentSpawnerRef.h"
#include "src/util/file/json/settings_json.h"

CrystalFragmentSpawnerRef* CrystalFragmentSpawnerRef::instance = nullptr;

void CrystalFragmentSpawnerRef::Load(bool ForceLoad)
{
    if (m_WasLoad && !ForceLoad)
        return;

    // 諸々の初期化
    auto jsonLoader = Settings_json::Inst();

    // JSON 読み込み
    jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

    // 各種値取得
    SpawnInterval   = jsonLoader->GetOrDefault<float>("Param.Interval", 0.0f, FILEKEY);
    SpawnAmount     = jsonLoader->GetOrDefault<float>("Param.Amount",   0.0f, FILEKEY);

    m_WasLoad = true;
}
