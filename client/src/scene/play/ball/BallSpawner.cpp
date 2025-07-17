#include "src/scene/play/ball/BallSpawner.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/spawner/SpawnerDescs.h"
#include "src/util/math/MathIncluder.h"
#include "src/util/time/GameTime.h"

BallSpawner::BallSpawner()
{
	m_GenerateBallIndex		= -1;
	m_pBallManager			= nullptr;
	m_pGeneratedBall		= nullptr;
	m_IsActive				= false;
	m_Desc					= {};
}

BallSpawner::~BallSpawner()
{
}

void BallSpawner::Init(const BALL_SPAWNER_DESC& desc)
{
	m_Desc = desc;
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
	IntervalProcess();

	// 生成処理
	if (GenerateProcess()) return;
}

void BallSpawner::Draw()
{
	Object3D::Draw();
}

void BallSpawner::OnSpawn(const Vector3& pos)
{
	m_pGeneratedBall = m_pBallManager->CreateBall(pos, true);
}

void BallSpawner::IntervalProcess()
{
	m_SpawnIntervalSec -= GTime.deltaTime;
}

bool BallSpawner::GenerateProcess()
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
	Spawn(spawnAmount);

	// 生成処理終了
	m_SpawnIntervalSec = m_Desc.INTERVAL_SEC + Random.GetFloatRange(-m_Desc.INTERVAL_SEC_RANDOM_RANGE, m_Desc.INTERVAL_SEC_RANDOM_RANGE);

	return true;
}

void BallSpawner::Spawn(int spawnAmount)
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

		OnSpawn(spawnPos);
	}
}

void AddBallSpawner(int hModel, const Transform& trs, const BALL_SPAWNER_DESC& desc)
{
	BallSpawner* spawner = Instantiate<BallSpawner>();
	spawner->Init(desc);
	spawner->SetModel(hModel);
	spawner->SetTransform(trs);
	spawner->Activate();
}

//void BallSpawnerPlaceByJson(const std::string& filepath, const std::string& filekey)
//{
//	// 読み込み
//	{
//		std::string key = filekey;
//		if (key == "")
//			key = filepath;
//
//		auto settings_json = Settings_json::Inst();
//		settings_json->LoadSettingJson(filepath, key, true);
//		std::vector<nlohmann::json> params = Settings_json::Inst()->Get<std::vector<nlohmann::json>>("Params", key);
//
//		for (const auto& param : params)
//		{
//			SPAWNER_DESC desc		= {};
//			BallSpawner* spawner	= Instantiate<BallSpawner>();
//
//			desc.INTERVAL_SEC						= param.at("INTERVAL_SEC");
//			desc.INTERVAL_SEC_RANDOM_RANGE			= param.at("INTERVAL_SEC_RANDOM_RANGE");
//			desc.SPAWN_AMOUNT_ONCE_MAX				= param.at("SPAWN_AMOUNT_ONCE_MAX");
//			desc.SPAWN_AMOUNT_ONCE_MIN				= param.at("SPAWN_AMOUNT_ONCE_MIN");
//			desc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE		= param.at("SPAWN_AMOUNT_ONCE_RANDOM_RANGE");
//			desc.SPAWN_AMOUNT_INITIAL				= param.at("SPAWN_AMOUNT_INITIAL");
//
//			auto& range					= param.at("SPAWN_RANGE");
//			float rx					= range.at("x").get<float>();
//			float ry					= range.at("y").get<float>();
//			float rz					= range.at("z").get<float>();
//			desc.SPAWN_RANGE			= Vector3(rx, ry, rz);			
//
//			auto& vel					= param.at("SPAWN_INITIAL_VELOCITY");
//			float vx					= vel.at("x").get<float>();
//			float vy					= vel.at("y").get<float>();
//			float vz					= vel.at("z").get<float>();
//			desc.SPAWN_INITIAL_VELOCITY	= Vector3(vx, vy, vz);
//
//			spawner->Init(desc);
//			spawner->Activate();
//		}
//	}
//}
