#pragma once
#include <string>

/// <summary>
/// CrystalFragment.jsonのパラメーターを保持するクラス
/// Jsonから読みこんだパラメーターを保持するよ
/// </summary>
class CrystalFragmentRef
{
private:
    const std::string   FILEPATH = "data/Json/Crystal/CrystalFragment.json";    // ファイルのパス
    const std::string   FILEKEY  = "CrystalFragment";                           // ファイル識別用キー
public:
    static CrystalFragmentRef& Inst()
    {
        static CrystalFragmentRef instance;
        return instance;
    }

    void Load(bool _ForceLoad = false);

    float               m_RotSpeed;     // 破片のY軸回転速度
    float               m_ElementPoint; // 属性ポイント
    float               m_Size;         // 大きさ
    float               m_LifeTime;     // 寿命

private:
    bool                m_WasLoad = false; // ロード済みか？ 重複回避用

    CrystalFragmentRef() :
        m_RotSpeed    (0.0f),
        m_ElementPoint(0.0f),
        m_Size        (0.0f),
        m_LifeTime    (0.0f)
    { /*DO NOTHING*/ }

    ~CrystalFragmentRef()
    { /*DO NOTHING*/ }

};

#define CRYSTALFRAGMENT_REF CrystalFragmentRef::Inst()