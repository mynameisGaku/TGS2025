#pragma once
#include <string>

class CharaBase;

class UserData
{
public:
    enum class ControllerType
    {
        None,
        Debug,
        Network,
        Player,
        AI
    };
public:
    ControllerType  CtrlType;
    std::string     TeamName;
    unsigned char   UserID;
    unsigned short  Point;
};

typedef UserData::ControllerType UserControllerType;