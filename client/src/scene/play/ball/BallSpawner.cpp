#include "src/scene/play/ball/BallSpawner.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/spawner/SpawnerDescs.h"
#include "src/util/math/MathIncluder.h"
#include "src/util/time/GameTime.h"
#include <src/reference/network/NetworkRef.h>
#include <src/common/network/NetworkManager.h>
#include <src/common/component/renderer/BallRenderer.h>

BallSpawner::BallSpawner()
{
	m_GenerateBallIndex		= -1;
	m_pBallManager			= nullptr;
	m_pGeneratedBall		= nullptr;
	m_IsActive				= false;
	m_Desc					= {};
	m_SpawnIntervalSec		= 0.0f;
}

BallSpawner::~BallSpawner()
{
}

void BallSpawner::Init(const BALL_SPAWNER_DESC& desc, const std::string& id)
{
	m_Desc = desc;
	m_UniqueID = id;

	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (net.IsHost)
		{
			m_IsActive = true;
			m_pNetworkManager = FindGameObject<NetworkManager>();
		}
		else
		{
			m_IsActive = false;
		}
	}
}

void BallSpawner::Start()
{
	m_pBallManager = FindGameObject<BallManager>();
	m_SpawnIntervalSec = m_Desc.INTERVAL_SEC + Random.GetFloatRange(-m_Desc.INTERVAL_SEC_RANDOM_RANGE, m_Desc.INTERVAL_SEC_RANDOM_RANGE);
}

void BallSpawner::Update()
{
	if (m_pGeneratedBall)
	{
		if (m_pGeneratedBall->IsActive())
		{
			Deactivate();
		}
		else
		{
			Activate();
		}
	}

	if (not m_IsActive)
		return;

	// インターバルを進める
	intervalProcess();

	// 生成処理
	if (generateProcess()) return;
}

void BallSpawner::Draw()
{
	Object3D::Draw();
}

void BallSpawner::onSpawn(const Vector3& pos)
{
	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (net.IsHost)
		{
			m_pGeneratedBall = m_pBallManager->CreateBall(pos, true);
			m_pNetworkManager->SendBallSpawnBySpawner(m_UniqueID, *m_pGeneratedBall);
		}
	}
	else
	{
		m_pGeneratedBall = m_pBallManager->CreateBall(pos, true);
	}
}

void BallSpawner::ForceSpawn(const std::string& ballID, const std::string& charaTag, const BallTexture& texture)
{
	if (m_pGeneratedBall)
	{
		m_pGeneratedBall->DestroyMe();
		m_pGeneratedBall = nullptr;
	}

	Vector3 random = Vector3
	(
		Random.GetFloatRange(-m_Desc.SPAWN_RANGE.x, m_Desc.SPAWN_RANGE.x),
		Random.GetFloatRange(-m_Desc.SPAWN_RANGE.y, m_Desc.SPAWN_RANGE.y),
		Random.GetFloatRange(-m_Desc.SPAWN_RANGE.z, m_Desc.SPAWN_RANGE.z)
	);

	Vector3 spawnPos = transform->position + random;

	m_pGeneratedBall = m_pBallManager->CreateBall(spawnPos, true);
	m_pGeneratedBall->SetUniqueID(ballID);
	m_pGeneratedBall->SetCharaTag(charaTag);
	m_pGeneratedBall->GetBallRenderer().SetTexture(texture);
}

void BallSpawner::intervalProcess()
{
	m_SpawnIntervalSec -= GTime.deltaTime;
}

bool BallSpawner::generateProcess()
{
	if (m_SpawnIntervalSec > 0.0f)
		return false;

	int spawnAmount = 0;

	// 一度に生成する数をランダムに決定
	if (m_Desc.SPAWN_AMOUNT_ONCE_MIN == 1 &&
		m_Desc.SPAWN_AMOUNT_ONCE_MAX == 1)
	{
		spawnAmount = 1;
	}
	else
	{
		spawnAmount = Random.GetIntRange(m_Desc.SPAWN_AMOUNT_ONCE_MIN, m_Desc.SPAWN_AMOUNT_ONCE_MAX + m_Desc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE);
	}

	// 沸かす
	spawn(spawnAmount);

	// 生成処理終了
	m_SpawnIntervalSec = m_Desc.INTERVAL_SEC + Random.GetFloatRange(-m_Desc.INTERVAL_SEC_RANDOM_RANGE, m_Desc.INTERVAL_SEC_RANDOM_RANGE);

	return true;
}

void BallSpawner::spawn(int spawnAmount)
{
	for (int i = 0; i < spawnAmount; ++i)
	{
		Vector3 random = Vector3
		(
			Random.GetFloatRange(-m_Desc.SPAWN_RANGE.x, m_Desc.SPAWN_RANGE.x),
			Random.GetFloatRange(-m_Desc.SPAWN_RANGE.y, m_Desc.SPAWN_RANGE.y),
			Random.GetFloatRange(-m_Desc.SPAWN_RANGE.z, m_Desc.SPAWN_RANGE.z)
		);

		Vector3 spawnPos = transform->position + random;

		onSpawn(spawnPos);
	}
}

BallSpawner* AddBallSpawner(int hModel, const Transform& trs, const BALL_SPAWNER_DESC& desc, const std::string& id)
{
	BallSpawner* spawner = Instantiate<BallSpawner>();
	spawner->Init(desc, id);
	spawner->SetModel(hModel);
	spawner->SetTransform(trs);
	spawner->Activate();
	return spawner;
}

BallSpawner* GetBallSpawnerFromUniqueID(const std::string& id)
{
	auto spawners = FindGameObjects<BallSpawner>();
	for (auto& spawner : spawners)
	{
		if (spawner->GetUniqueID() == id)
		{
			return spawner;
		}
	}
	return nullptr;
}
