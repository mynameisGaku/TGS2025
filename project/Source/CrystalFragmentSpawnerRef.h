#pragma once
#include <string>

class CrystalFragmentSpawnerRef
{
private:
    const std::string   FILENAME = "data/Json/Crystal/CrystalFragmentSpawner.json";
    const std::string   FILEKEY = "CrystalFragmentSpawner";
public:
    static CrystalFragmentSpawnerRef& Inst()
    {
        static CrystalFragmentSpawnerRef instance;
        return instance;
    };

    void Load(bool ForceLoad = false);

    float               SpawnInterval;
    float               SpawnAmount;

private:
    bool                m_WasLoad = false;

    CrystalFragmentSpawnerRef() :
        SpawnInterval(0.0f),
        SpawnAmount(0.0f)
    { /*DO NOTHING*/ };
    ~CrystalFragmentSpawnerRef()
    { /*DO NOTHING*/ };
};

#define CRYSTALFRAGMENTSPAWNER_REF CrystalFragmentSpawnerRef::Inst()