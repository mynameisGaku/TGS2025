#pragma once
#include <src/scene/play/spawner/SpawnerDescs.h>
#include <src/util/object3D/Object3D.h>
#include <src/util/math/mathIncluder.h>
#include <src/common/component/renderer/BallRenderer.h>

class BallManager;
class Ball;
class NetworkManager;

class BallSpawner : public Object3D
{
public:
	BallSpawner();
	~BallSpawner();

	void Init(const BALL_SPAWNER_DESC& desc, const std::string& id = "NULL");
	void Start() override;
	void Update() override;
	void Draw() override;
	void onSpawn(const Vector3& pos);

	// Spawner�̗L����
	void Activate() { m_IsActive = true; }
	// Spawner�̖�����
	void Deactivate() { m_IsActive = false; }

	const std::string& GetUniqueID() const { return m_UniqueID; }
	void SetUniqueID(const std::string& id) { m_UniqueID = id; }

	void ForceSpawn(const std::string& ballID, const std::string& charaTag, const BallTexture& texture);
private:
	// �����Ԋu��i�߂�
	void intervalProcess();
	// �����������s��
	bool generateProcess();
	// �w�����������
	void spawn(int spawnAmount);

	int					m_GenerateBallIndex;	// ���̃X�|�i�[�����������{�[���̃C���f�b�N�X
	Ball*				m_pGeneratedBall;		// ���̃X�|�i�[�����������{�[���̃|�C���^
	BallManager*		m_pBallManager;			// �{���}�l�|�C���^
	BALL_SPAWNER_DESC	m_Desc;					// �X�|�i�[�̐ݒ�
	float				m_SpawnIntervalSec;		// �����܂ł̎��Ԃ��v������p�^�C�}�[
	bool				m_IsActive;				// Spawner���L�����ǂ���
	std::string			m_UniqueID;				// �ŗLID

	NetworkManager*		m_pNetworkManager;
};

BallSpawner* AddBallSpawner(int hModel, const Transform& trs, const BALL_SPAWNER_DESC& desc, const std::string& id = "NULL");
BallSpawner* GetBallSpawnerFromUniqueID(const std::string& id);