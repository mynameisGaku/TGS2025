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
public:

    static CharaStaminaRef& Inst()
    {
        static CharaStaminaRef instance;
        return instance;
    }

    float Max;              // �ő�X�^�~�i
    float RegenTakesTime;   // �񕜂ɂ����鎞��
    float RegenStartTime;   // �񕜊J�n�܂ł̎���

    void Load(bool ForceLoad = false);

private:
    bool                m_WasLoad = false; // ���[�h�ς݂��H �d�����p

    CharaStaminaRef() :
        Max(0.0f),
        RegenTakesTime(0.0f)
    { /*DO NOTHING*/ }

    ~CharaStaminaRef()
    { /*DO NOTHING*/ }

};

#define CHARASTAMINA_REF CharaStaminaRef::Inst()