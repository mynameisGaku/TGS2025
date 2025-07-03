#pragma once
#include "src/util/ui/UI_Canvas.h"
#include "src/reference/input/InputRef.h"
#include "src/util/input/InputManager.h"
#include "src/util/math/Vector2.h"
#include <unordered_map>
#include <string>
#include <src/util/font/Font.h>
#include <src/reference/ui/UI_ButtonHintRef.h>

/// <summary>
/// ボタンヒントUI
/// </summary>
/// <author> フジモトg買う </author>
class UI_ButtonHint : public UI_Canvas
{
public:
    UI_ButtonHint(const RectTransform& trs, int index);
    ~UI_ButtonHint();

    void Update() override;
    void Draw() override;

    void PushKey(const std::string& key);
    void PushKey(const KeyDefine::KeyCode& code);
    void ReleaseKey(const std::string& key);
    void ReleaseKey(const KeyDefine::KeyCode& code);

    void Activate(const std::string& key);
    void Deactivate(const std::string& key);

private:

    struct BUTTON
    {
        int hImage = -1;
        int hPushImage = -1;
        int ImageWidth = 0;
        int ImageHeight = 0;
        std::string DisplayString = "";
        Vector2 AnchorFromCenter{};           // アンカーからの中心位置
        Vector2 LocalPosition{};              // ローカル位置
        bool isPush = false;
        bool isActive = false;
    };

    int m_CharaIndex{};
    InputRef* m_pInputRef = nullptr;
    UI_ButtonHintRef* m_pButtonHintRef = nullptr;
    std::unordered_map<std::string, BUTTON> m_ButtonHints{};
    FontInfo m_Font{};
    int m_hFont{};
};