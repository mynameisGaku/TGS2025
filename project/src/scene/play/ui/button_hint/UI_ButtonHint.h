#pragma once
#include "src/util/ui/UI_Canvas.h"
#include "src/reference/input/InputRef.h"
#include "src/util/input/InputManager.h"
#include <unordered_map>
#include <string>

class UI_ButtonHint : public UI_Canvas
{
public:
    UI_ButtonHint(const RectTransform& trs, int index);
    ~UI_ButtonHint();

    void Draw() override;

    void Push(const std::string& key);
    void Push(const KeyDefine::KeyCode& code);
    void Release(const std::string& key);
    void Release(const KeyDefine::KeyCode& code);

    void Activate(const std::string& key);
    void Deactivate(const std::string& key);

private:

    struct BUTTON
    {
        int hImage = -1;
        int hPushImage = -1;
        bool isPush = false;
        bool isActive = false;
    };

    int m_CharaIndex{};
    InputRef* m_pInputRef = nullptr;
    std::unordered_map<std::string, BUTTON> m_ButtonHints{};
};