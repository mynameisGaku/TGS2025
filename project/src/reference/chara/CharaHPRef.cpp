#include "src/reference/chara/CharaHPRef.h"
#include "src/util/file/json/settings_json.h"

void CharaHPRef::Load(bool ForceLoad)
{
    if (not ForceLoad && m_WasLoad)
    {
        return;
    }

    // ���X�̏�����
    auto jsonLoader = Settings_json::Inst();

    // JSON �ǂݍ���
    jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

    // �e��l�擾
    Max = jsonLoader->GetOrDefault<float>("Param.Max", 0.0f, FILEKEY);
    Damage = jsonLoader->GetOrDefault<float>("Param.Damage", 0.0f, FILEKEY);
    Heal = jsonLoader->GetOrDefault<float>("Param.Heal", 0.0f, FILEKEY);

    // ���[�h������
    m_WasLoad = true;
}
