#include "src/reference/crystal/CrystalFragmentSpawnerRef.h"
#include "src/util/file/json/settings_json.h"

CrystalFragmentSpawnerRef* CrystalFragmentSpawnerRef::instance = nullptr;

void CrystalFragmentSpawnerRef::Load(bool ForceLoad)
{
    if (m_WasLoad && !ForceLoad)
        return;

    // ���X�̏�����
    auto jsonLoader = Settings_json::Inst();

    // JSON �ǂݍ���
    jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

    // �e��l�擾
    SpawnInterval   = jsonLoader->GetOrDefault<float>("Param.Interval", 0.0f, FILEKEY);
    SpawnAmount     = jsonLoader->GetOrDefault<float>("Param.Amount",   0.0f, FILEKEY);

    m_WasLoad = true;
}
