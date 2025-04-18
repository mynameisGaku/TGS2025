#include "CrystalFragmentRef.h"
#include <Library/FileUtil.h>
#include "CrystalFragment.h"
#include "settings_json.h"

void CrystalFragmentRef::Load(bool _ForceLoad)
{
    if (not _ForceLoad && m_WasLoad)
    {return;}

    // 諸々の初期化
    auto jsonLoader = Settings_json::Inst();

    // JSON 読み込み
    jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

    // 各種値取得
    m_RotSpeed        = jsonLoader->GetOrDefault<float>("Param.RotSpeed",     0.0f, FILEKEY);
    m_ElementPoint    = jsonLoader->GetOrDefault<float>("Param.ElementPoint", 0.0f, FILEKEY);
    m_Size            = jsonLoader->GetOrDefault<float>("Param.Size",         0.0f, FILEKEY);
    m_LifeTime        = jsonLoader->GetOrDefault<float>("Param.LifeTime",     0.0f, FILEKEY);

    // ロードしたよ
    m_WasLoad = true;
}