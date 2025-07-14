#pragma once
#include <vector>
#include <string>
#include <src/util/input/KeyDefine.h>
#include <src/util/math/Vector2.h>

class UI_ButtonHintRef
{
private:
    const std::string PATH      = "data/Json/UI/ButtonHint.json";
    const std::string FILEKEY   = "UI_ButtonHint";
public:
    static UI_ButtonHintRef& Inst();
    static void Destroy();
    void Load(bool forceLoad = false);

    struct ButtonPosition
    {
        std::vector<std::string> KeyNames{}; // �L�[���̃��X�g
        Vector2 AnchorFromCenter{};           // �A���J�[����̒��S�ʒu
        Vector2 LocalPosition{};              // ���[�J���ʒu
        std::string DisplayString = "";
    };

    std::vector<ButtonPosition> ButtonPositions;
    ButtonPosition GetData(const std::string& key);
private:
    static UI_ButtonHintRef* instance;
    bool m_WasLoad = false;
};