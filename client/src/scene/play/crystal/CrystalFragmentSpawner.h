#pragma once
#include "src/util/object3D/Object3D.h"

class CrystalFragmentManager;

class CrystalFragmentSpawner : public Object3D
{
public:
    CrystalFragmentSpawner();
    CrystalFragmentSpawner(const Vector3& pos);
    ~CrystalFragmentSpawner();
    
    void Start() override;
    void Update() override;
    void Draw() override;

    // �X�|�i�[�L��
    void Activate() {m_IsActive = true;}
    // �X�|�i�[����
    void Deactivate() { m_IsActive = false; }

private:
    CrystalFragmentManager* m_pCrystalM;        // �N���X�^���}�l�[�W���[
    float                   m_SpawnInterval;    // �����܂ł̃C���^�[�o��
    float                   m_SpawnAmount;      // ��x�Ő������鐔
    float                   m_SpawnTimer;       // �����܂ł̎��Ԃ��v������p�^�C�}�[
    bool                    m_IsActive;         // �X�|�i�[���L�����ǂ���
};