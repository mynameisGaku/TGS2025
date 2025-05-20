#pragma once
#include <string>

class CrystalFragmentSpawnerRef
{
private:
    const std::string   FILEPATH = "data/Json/Crystal/CrystalFragmentSpawner.json"; // ファイルのパス
    const std::string   FILEKEY = "CrystalFragmentSpawner";                         // ファイル識別用キー

    static CrystalFragmentSpawnerRef* instance;
public:
    static CrystalFragmentSpawnerRef* Inst()
    {
        if (instance == nullptr)
        {
            instance = new CrystalFragmentSpawnerRef;
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

    void Load(bool ForceLoad = false);

    float               SpawnInterval;  // 生成までのインターバル
    float               SpawnAmount;    // 一度で生成する数

private:
    bool                m_WasLoad = false; // ロード済みか？ 重複回避用

    CrystalFragmentSpawnerRef() :
        SpawnInterval(0.0f),
        SpawnAmount(0.0f)
    { /*DO NOTHING*/ };
    ~CrystalFragmentSpawnerRef()
    { /*DO NOTHING*/ };
};

#define CRYSTALFRAGMENTSPAWNER_REF (*CrystalFragmentSpawnerRef::Inst())