#include "src/reference/crystal/CrystalFragmentSpawnerRef.h"
#include "src/util/file/json/settings_json.h"

CrystalFragmentSpawnerRef* CrystalFragmentSpawnerRef::instance = nullptr;

void CrystalFragmentSpawnerRef::Load(bool ForceLoad)
{
    if (m_WasLoad && !ForceLoad)
        return;

    // ”X‚Ì‰Šú‰»
    auto jsonLoader = Settings_json::Inst();

    // JSON “Ç‚Ýž‚Ý
    jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

    // ŠeŽí’lŽæ“¾
    SpawnInterval   = jsonLoader->GetOrDefault<float>("Param.Interval", 0.0f, FILEKEY);
    SpawnAmount     = jsonLoader->GetOrDefault<float>("Param.Amount",   0.0f, FILEKEY);

    m_WasLoad = true;
}
