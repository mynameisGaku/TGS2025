#pragma once
#include <vector>
#include <string>
#include <src/util/math/Vector2.h>
#include <src/util/singleton/singleton.h>

class UI_ChatBarRef
{
private:
    const std::string PATH = "data/Json/UI/ChatBar.json";
    const std::string FILEKEY = "UI_ChatBar";
public:
    void Load(bool forceLoad = false);
private:
    bool m_WasLoad = false;
};

#define UI_CHATBAR_REF Singleton<UI_ChatBarRef>::Instance()
