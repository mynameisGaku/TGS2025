#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class GameRef
{
private:
    const std::string FILEPATH  = "data/json/game/GameRef.json";
    const std::string FILEKEY   = "GameRef";

    static GameRef* instance;
public:

    static GameRef& Inst();

    void Destroy();

    void Load(bool forceLoad = false);

    struct GameModeDesc
    {
        std::string GameModeName;
        int         WinPointMax;
        float       PlayTimeMaxSec;
    };

    std::unordered_map<std::string, GameModeDesc>   GameModeDescs;
    std::vector<std::string>                        GameModeNames;
    std::vector<std::string>                        TeamNames;

    float GameStartCountMaxSec;
    float GameEndCountMaxSec;

private:
    bool m_WasLoad = false;
};

#define GAME_REF GameRef::Inst()

typedef GameRef::GameModeDesc GAME_MODE_DESC;