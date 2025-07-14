#pragma once
#include <string>
#include <vector>

/// <summary>
/// CrystalFragment.jsonのパラメーターを保持するクラス
/// Jsonから読みこんだパラメーターを保持するよ
/// </summary>
class CrystalFragmentRef
{
private:
    enum InfoType
    {
        NONE,
        SMALL,
        MEDIUM,
        LARGE,
        MAX
    };
private:
    const std::string   FILEPATH = "data/Json/Crystal/CrystalFragment.json";    // ファイルのパス
    const std::string   FILEKEY  = "CrystalFragment";                           // ファイル識別用キー

    static CrystalFragmentRef* instance;
public:
    static CrystalFragmentRef* Inst()
    {
        if (instance == nullptr)
        {
            instance = new CrystalFragmentRef;
        }
        return instance;
    }

    // " リファレンス解放 "
    // ↑を文字列検索したら着くところで解放してます。(おそらくメインループのすぐあと)
    void Destroy()
    {
        if (instance)
        {
            delete instance;
        }
        instance = nullptr;
    }

    void Load(bool _ForceLoad = false);

    struct Info
    {
        float               RotSpeed;     // 破片のY軸回転速度
        float               ElementPoint; // 属性ポイント
        float               Size;         // 大きさ
        float               LifeTime;     // 寿命
    };

    std::vector<Info>   m_Refs;       // 各サイズのパラメーター
    int                 m_LimitMax;   // 生成される量の限度

private:
    bool                m_WasLoad = false; // ロード済みか？ 重複回避用

    CrystalFragmentRef():
        m_LimitMax(0)
    { /*DO NOTHING*/ }

    ~CrystalFragmentRef()
    { /*DO NOTHING*/ }

};

#define CRYSTALFRAGMENT_REF (*CrystalFragmentRef::Inst())