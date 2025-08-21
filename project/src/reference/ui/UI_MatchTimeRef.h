#pragma once
#include <vector>
#include <string>
#include <src/util/math/Vector2.h>
#include <src/util/singleton/singleton.h>
#include <src/util/color/RGBColor.h>

class UI_MatchTimeRef
{
private:
    const std::string PATH = "data/Json/UI/MatchTime.json";
    const std::string FILEKEY = "UI_MatchTime";
public:
    UI_MatchTimeRef() { Load(false); }
    void Load(bool forceLoad = false);

    float PositionY;
    Vector2 BackScale;
    RGBColor BackColor;
    RGBColor TimeNormalColor;
    RGBColor TimeWarningColor;
private:
    bool m_WasLoad = false;
};

#define UI_MATCHTIME_REF Singleton<UI_MatchTimeRef>::Instance()
