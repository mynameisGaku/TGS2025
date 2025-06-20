#pragma once
#include <list>
#include <unordered_map>
#include "framework/gameObject.h"
#include "src/scene/play/chara/CharaSpawnPoint.h"

/// <summary>
/// �L�����N�^�[�̃X�|�i�[���Ǘ�����N���X�B
/// �X�e�[�W����ǂݍ��ނƂ���JSON����Z�b�g�œǂݍ���ł��܂��B
/// </summary>
/// <author>���{��</author>
class CharaSpawnPointManager : public GameObject
{
public:
    CharaSpawnPointManager();
    ~CharaSpawnPointManager();

    void Update() override;
    void Draw() override;

    // �X�|�[���n�_��ǉ�����
    void AddSpawnPoint(int hModel, const Transform& trs, const CHARA_SPAWN_POINT_DESC& desc);

    // �^����ꂽ���W����ł��߂��X�|�i�[���擾����
    CharaSpawnPoint* Get_Near(const Vector3& pos);
    // �^����ꂽ���W����ł������X�|�i�[���擾����
    CharaSpawnPoint* Get_Far(const Vector3& pos);
    // ���݂���X�|�i�[���烉���_���Ɉ�擾����
    CharaSpawnPoint* Get_Random();
    // ���݂���X�|�i�[�̒��ōł��g�p�����Ⴂ�X�|�i�[���擾����
    CharaSpawnPoint* Get_LowUsageRate();
    // ���݂���X�|�i�[�̒��ōł��g�p���������X�|�i�[���擾����
    CharaSpawnPoint* Get_HighUsageRate();
private:
    // �������ꂽ�X�|�i�[�̃��X�g
    std::list<CharaSpawnPoint*> m_SpawnPoints;
};