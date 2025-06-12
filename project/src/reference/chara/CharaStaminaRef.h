#pragma once
#include <string>

/// <summary>
/// �L�����̃X�^�~�i�Ɋւ���ݒ��ێ�����N���X
/// Json����ǂ݂��񂾃p�����[�^�[��ێ������
/// </summary>
class CharaStaminaRef
{
private:
    const std::string   FILEPATH = "data/Json/Chara/CharaStamina.json";         // �t�@�C���̃p�X
    const std::string   FILEKEY  = "CharaStaminaRef";                           // �t�@�C�����ʗp�L�[

    static CharaStaminaRef* instance;
public:

    static CharaStaminaRef* Inst()
    {
        if (instance == nullptr)
        {
            instance = new CharaStaminaRef;
        }
        return instance;
    }

    // " ���t�@�����X��� "
    // ���𕶎��񌟍������璅���Ƃ���ŉ�����Ă܂��B(�����炭���C�����[�v�̂�������)
    void Destroy()
    {
        if (instance)
        {
            delete instance;
        }
        instance = nullptr;
    }

    float Max;              // �ő�X�^�~�i
    float RegenTakesTime;   // �񕜂ɂ����鎞��
    float RegenStartTime;   // �񕜊J�n�܂ł̎���
    float RegenStartTime_AllLost;   // �X�^�~�i���؂ꂽ�ۂ̉񕜊J�n�܂ł̎���

    void Load(bool ForceLoad = false);

private:
    bool                m_WasLoad = false; // ���[�h�ς݂��H �d�����p

    CharaStaminaRef() :
        Max(0.0f),
        RegenTakesTime(0.0f),
        RegenStartTime(0.0f),
        RegenStartTime_AllLost(0.0f)
    { /*DO NOTHING*/ }

    ~CharaStaminaRef()
    { /*DO NOTHING*/ }

};

#define CHARASTAMINA_REF (*CharaStaminaRef::Inst())