#pragma once
#include <string>

class WindowSetting
{
private:
    static WindowSetting* instance;
public:
    static WindowSetting& Inst()
    {
        if (instance == nullptr)
        {
            instance = new WindowSetting();
        }
        return (*instance);
    }

    void Destroy();

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