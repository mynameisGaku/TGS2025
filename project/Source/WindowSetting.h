#pragma once
#include <string>

class WindowSetting
{
public:
    static WindowSetting& Inst()
    {
        static WindowSetting instance;
        return instance;
    }

    static void Load(const std::string& file = "window.ini");

    static float width;
    static float width_half;
    static float height;
    static float height_half;
    static float extend;
    static std::string name;
    static bool isFull;
private:

};