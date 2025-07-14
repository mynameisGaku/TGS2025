#include "src/reference/crystal/CrystalFragmentRef.h"
#include "src/scene/play/crystal/CrystalFragment.h"
#include "src/util/file/FileUtil.h"
#include "src/util/file/json/settings_json.h"

CrystalFragmentRef* CrystalFragmentRef::instance = nullptr;

void CrystalFragmentRef::Load(bool _ForceLoad)
{
    if (not _ForceLoad && m_WasLoad)
    {return;}

    // 諸々の初期化
    auto jsonLoader = Settings_json::Inst();

    // JSON 読み込み
    jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

    // Has Element
    {
        for (int i = 0; i < InfoType::MAX; i++)
        {
            std::string infotype = "Param.";
            switch (i)
            {
            case InfoType::NONE:
                infotype += "NONE.";
                break;
            case InfoType::SMALL:
                infotype += "ELEMENT.Small.";
                break;
            case InfoType::MEDIUM:
                infotype += "ELEMENT.Medium.";
                break;
            case InfoType::LARGE:
                infotype += "ELEMENT.Large.";
                break;
            }

            // 各種値取得
            Info info = {};
            info.RotSpeed       = jsonLoader->GetOrDefault<float>(infotype + "RotSpeed", 0.0f, FILEKEY);
            info.ElementPoint   = jsonLoader->GetOrDefault<float>(infotype + "ElementPoint", 0.0f, FILEKEY);
            info.Size           = jsonLoader->GetOrDefault<float>(infotype + "Size", 0.0f, FILEKEY);
            info.LifeTime       = jsonLoader->GetOrDefault<float>(infotype + "LifeTime", 0.0f, FILEKEY);
            m_Refs.push_back(info);
        }
    }// End Has Element

    m_LimitMax = jsonLoader->GetOrDefault<int>("Param.LimitMax", 0, FILEKEY);

    // ロードしたよ
    m_WasLoad = true;
}