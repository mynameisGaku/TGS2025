#include "src/reference/chara/CharaDefineRef.h"
#include "src/util/file/json/settings_json.h"

void CharaDefineRef::Load(bool ForceLoad)
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
    Max = jsonLoader->GetOrDefault<int>("Param.Max", 0, FILEKEY);
    auto Tags_tmp = jsonLoader->Get<std::vector<std::string>>("Param.Tags", FILEKEY);
    for (auto& tmp : Tags_tmp)
    {
        Tags.push_back(tmp);
    }
    CatchRadius = jsonLoader->GetOrDefault<float>("Param.CatchRadius", 0.0f, FILEKEY);
    EmoteInterval = jsonLoader->GetOrDefault<float>("Param.EmoteInterval", 0.0f, FILEKEY);
    JumpPower = jsonLoader->GetOrDefault<float>("Param.JumpPower", 0.0f, FILEKEY);
    // ���[�h������
    m_WasLoad = true;
}
