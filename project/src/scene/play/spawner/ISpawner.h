#pragma once
#include "framework/gameObject.h"
#include "src/util/math/Vector3.h"
#include "src/scene/play/spawner/SpawnerDescs.h"

/// <summary>
/// �X�|�i�[�̃C���^�[�t�F�[�X
/// </summary>
/// <author>���{��</author>
class ISpawner : public GameObject
{
public:
    ISpawner();
    ~ISpawner();

    void Init(const SPAWNER_DESC& desc);
    virtual void Start() override;
    virtual void Update() override;
    virtual void Draw() override;
    // �������������s�B�����͔h���N���X�ōs���B
    virtual void OnSpawn(const Vector3& pos) {}

    // Spawner�̗L����
    void Activate() { m_IsActive = true; }
    // Spawner�̖�����
    void Deactivate() { m_IsActive = false; }

    // �����Ԋu��i�߂�
    void IntervalProcess();

    // �����������s��
    bool GenerateProcess();
    
    // �w�����������
    void Spawn(int spawnAmount);
    
protected:
    SPAWNER_DESC    m_Desc;             // �X�|�i�[�̐ݒ�
    float           m_SpawnIntervalSec; // �����܂ł̎��Ԃ��v������p�^�C�}�[
    bool            m_IsActive;         // Spawner���L�����ǂ���
};