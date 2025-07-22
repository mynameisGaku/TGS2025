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

	// Spawnerの有効化
	void Activate() { m_IsActive = true; }
	// Spawnerの無効化
	void Deactivate() { m_IsActive = false; }

	const std::string& GetUniqueID() const { return m_UniqueID; }
	void SetUniqueID(const std::string& id) { m_UniqueID = id; }

	void ForceSpawn(const std::string& ballID, const std::string& charaTag, const BallTexture& texture);
private:
	// 生成間隔を進める
	void intervalProcess();
	// 生成処理を行う
	bool generateProcess();
	// 指定個数生成する
	void spawn(int spawnAmount);

	int					m_GenerateBallIndex;	// このスポナーが生成したボールのインデックス
	Ball*				m_pGeneratedBall;		// このスポナーが生成したボールのポインタ
	BallManager*		m_pBallManager;			// ボルマネポインタ
	BALL_SPAWNER_DESC	m_Desc;					// スポナーの設定
	float				m_SpawnIntervalSec;		// 生成までの時間を計測する用タイマー
	bool				m_IsActive;				// Spawnerが有効かどうか
	std::string			m_UniqueID;				// 固有ID

	NetworkManager*		m_pNetworkManager;
};

BallSpawner* AddBallSpawner(int hModel, const Transform& trs, const BALL_SPAWNER_DESC& desc, const std::string& id = "NULL");
BallSpawner* GetBallSpawnerFromUniqueID(const std::string& id);