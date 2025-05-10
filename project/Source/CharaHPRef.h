#pragma once
#include <string>

/// <summary>
/// �L������HP�Ɋւ���ݒ��ێ�����N���X
/// Json����ǂ݂��񂾃p�����[�^�[��ێ������
/// </summary>
class CharaHPRef
{
private:
    const std::string   FILEPATH = "data/Json/Chara/CharaHP.json";         // �t�@�C���̃p�X
    const std::string   FILEKEY = "CharaHPRef";                           // �t�@�C�����ʗp�L�[
public:

    static CharaHPRef& Inst()
    {
        static CharaHPRef instance;
        return instance;
    }

    float Max;              // �ő�HP
    float Damage;           // �_���[�W��
    float Heal;             // �񕜗�

    void Load(bool ForceLoad = false);

private:
    bool m_WasLoad = false; // ���[�h�ς݂��H �d�����p

    CharaHPRef() :
        Max(0.0f),
        Damage(0.0f),
        Heal(0.0f)
    { /*DO NOTHING*/ }

    ~CharaHPRef()
    { /*DO NOTHING*/ }

};

#define CHARAHP_REF CharaHPRef::Inst()