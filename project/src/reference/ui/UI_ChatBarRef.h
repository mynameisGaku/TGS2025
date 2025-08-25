#pragma once
#include <vector>
#include <string>
#include <src/util/math/Vector2.h>
#include <src/util/singleton/singleton.h>
#include <src/util/color/RGBColor.h>

class UI_ChatBarRef
{
private:
    const std::string PATH = "data/Json/UI/ChatBar.json";
    const std::string FILEKEY = "UI_ChatBar";
public:
    UI_ChatBarRef() { Load(false); }
    void Load(bool forceLoad = false);

    float PositionY;
    float IconOffsetX;
    float IconScale;
    Vector2 BackScale;
    RGBColor BackColor;
    Vector2 GaugeBackScale;
    RGBColor GaugeBackColor;
private:
    bool m_WasLoad = false;
};

#define UI_CHATBAR_REF Singleton<UI_ChatBarRef>::Instance()
