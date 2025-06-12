#include "src/reference/chara/CharaStaminaRef.h"
#include "src/util/file/json/settings_json.h"

CharaStaminaRef* CharaStaminaRef::instance = nullptr;

void CharaStaminaRef::Load(bool ForceLoad)
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
    Max             = jsonLoader->GetOrDefault<float>("Param.Max",            0.0f, FILEKEY);
    RegenTakesTime  = jsonLoader->GetOrDefault<float>("Param.RegenTakesTime", 0.0f, FILEKEY);
    RegenStartTime  = jsonLoader->GetOrDefault<float>("Param.RegenStartTime", 0.0f, FILEKEY);
    RegenStartTime_AllLost = jsonLoader->GetOrDefault<float>("Param.RegenStartTime_AllLost", 0.0f, FILEKEY);

    // ���[�h������
    m_WasLoad = true;
}
