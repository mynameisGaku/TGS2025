#include "src/scene/play/ball/BallSpawner.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/spawner/SpawnerDesc.h"
#include "src/util/file/json/settings_json.h"
#include "src/util/file/json/VectorJson.h"

BallSpawner::BallSpawner()
{
    m_GenerateBallIndex = -1;
    m_pBallManager = nullptr;
}

BallSpawner::~BallSpawner()
{
}

void BallSpawner::Start()
{
    m_pBallManager = FindGameObject<BallManager>();
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

    ISpawner::Update();

}

void BallSpawner::OnSpawn(const Vector3& pos)
{
    m_pGeneratedBall = m_pBallManager->CreateBall(pos, true);
}

void BallSpawnerPlaceByJson(const std::string& filepath, const std::string& filekey)
{
	// “Ç‚Ýž‚Ý
	{
		std::string key = filekey;
		if (key == "")
			key = filepath;

		auto settings_json = Settings_json::Inst();
		settings_json->LoadSettingJson(filepath, key, true);
		std::vector<nlohmann::json> params = Settings_json::Inst()->Get<std::vector<nlohmann::json>>("Params", key);

		for (const auto& param : params)
		{
			SPAWNER_DESC desc = {};
			BallSpawner* spawner = Instantiate<BallSpawner>();

			desc.INTERVAL_SEC = param.at("INTERVAL_SEC");
			desc.INTERVAL_SEC_RANDOM_RANGE = param.at("INTERVAL_SEC_RANDOM_RANGE");
			desc.SPAWN_AMOUNT_ONCE_MAX = param.at("SPAWN_AMOUNT_ONCE_MAX");
			desc.SPAWN_AMOUNT_ONCE_MIN = param.at("SPAWN_AMOUNT_ONCE_MIN");
			desc.SPAWN_AMOUNT_ONCE_RANDOM_RANGE = param.at("SPAWN_AMOUNT_ONCE_RANDOM_RANGE");
			desc.SPAWN_AMOUNT_INITIAL = param.at("SPAWN_AMOUNT_INITIAL");

			auto& range = param.at("SPAWN_RANGE");
			float rx = range.at("x").get<float>();
			float ry = range.at("y").get<float>();
			float rz = range.at("z").get<float>();
			desc.SPAWN_RANGE = Vector3(rx, ry, rz);
			
			auto& vel = param.at("SPAWN_FIRST_VELOCITY");
			float vx = vel.at("x").get<float>();
			float vy = vel.at("y").get<float>();
			float vz = vel.at("z").get<float>();
			desc.SPAWN_FIRST_VELOCITY = Vector3(vx, vy, vz);

			auto& pos = param.at("SPAWNER_POSITION");
			float px = pos.at("x").get<float>();
			float py = pos.at("y").get<float>();
			float pz = pos.at("z").get<float>();
			desc.SPAWNER_POSITION = Vector3(px, py, pz);

			std::string name = param.at("SPAWNER_NAME").get<std::string>();
			desc.SPAWNER_NAME = name;

			spawner->Init(desc);
			spawner->Activate();
		}
	}
}
