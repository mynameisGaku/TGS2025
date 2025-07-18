#include "src/scene/play/ball/BallManager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"
#include "src/scene/play/ball/Ball.h"
#include "src/scene/play/ball/BallManager.h"
#include "src/reference/ball/BallRef.h"
#include "src/util/file/FileUtil.h"
#include "vendor/nlohmann/json.hpp"
#include <fstream>

#include "src/common/camera/CameraManager.h"
#include <src/reference/network/NetworkRef.h>
#include <vendor/uuid4/uuid4.h>

BallManager::BallManager()
{
	m_Model = ResourceLoader::MV1LoadModel("data/Model/Ball/Ball.mv1");
	loadTextures();

	BALL_REF.Load();

	m_hTrails["Green"] = LoadGraph("data/img/trail/Trail_Green.png");
	m_hTrails["Blue"] = LoadGraph("data/img/trail/Trail_Blue.png");
	m_hTrails["Red"] = LoadGraph("data/img/trail/Trail_Red.png");
	m_hTrails["Plain"] = LoadGraph("data/img/trail/Trail_Plain.png");
	m_hTrails["Plain_Distortion"] = LoadGraph("data/img/trail/Trail_Plain_Distortion.png");
	m_hTrails["Plain_Distortion_Thin"] = LoadGraph("data/img/trail/Trail_Plain_Distortion_Thin.png");

	m_pPool = new Pool<Ball>(BALL_REF.PoolMax);
}

BallManager::~BallManager()
{
#ifdef USE_POOL

	for (auto& it : m_hTrails)
	{
		DeleteGraph(it.second);
	}
	m_hTrails.clear();

	for (auto& item : m_pPool->GetAllItems())
	{
		if (item == nullptr)
			continue;

		if (item->m_pObject != nullptr)
		{
			item->m_pObject->DestroyMe();
			item->m_pObject = nullptr;
		}

		delete item;
		item = nullptr;
	}
	delete m_pPool;
#else
	for (Ball* ball : m_Balls)
	{
		ball->DestroyMe();
	}
	m_Balls.clear();
#endif

	ResourceLoader::MV1DeleteModel(m_Model);
	for (auto item : m_Textures)
	{
		ResourceLoader::DeleteGraph(item.second.Texture);
	}
}

void BallManager::Update()
{
#ifdef USE_POOL
	if (m_pPool->GetActiveItemNum() == 0)
		return;

	BALL_REF.Load();

	for (auto& item : m_pPool->GetAllItems())
	{
		auto ball = item->m_pObject;

		if (not ball)
			continue;

		if (not ball->IsActive())
		{
			m_pPool->DeActive(ball->m_Index);
		}
	}
#else
	for (auto it = m_Balls.begin(); it != m_Balls.end(); )
	{
		auto ball = (*it);
		if (not ball->IsActive())
		{
			// 無効化されたら削除
			ball->DestroyMe();
			ball = nullptr;
			it = m_Balls.erase(it);
		}
		else
		{
			++it;
		}
	}
#endif
}

void BallManager::Draw()
{
	//if (CameraManager::IsScreenDivision())
		return;

	m_pPool->PoolImGuiRendererBegin("ball pool debug");

	ImGui::Text(("Capacity: " + std::to_string(m_pPool->GetCapacity())).c_str());
	ImGui::Text(("Active Items: " + std::to_string(m_pPool->GetActiveItemNum())).c_str());
	ImGui::Spacing();
	ImGui::Text("====================================");
	ImGui::Spacing();

	for (auto& item : m_pPool->GetAllItems())
	{
		if (item->m_pObject == nullptr)
			continue;

		ImGui::Text(("Lifetime :" + std::to_string(item->m_pObject->m_LifeTime)).c_str());
	}

	m_pPool->PoolImguiRendererEnd();
}

Ball* BallManager::CreateBall(const Vector3& position, bool isSpawn)
{
	auto& ref = BALL_REF;
	// 各種リファレンスを元に初期化

	// 追加
#ifdef USE_POOL
	if (m_pPool->CheckActiveObjectByCount(m_pPool->GetCapacity()))
	{
		return nullptr;
	}
#endif


#ifdef USE_POOL
	uint32_t index = m_pPool->GetIndex();

	auto obj = m_pPool->Alloc([&](uint32_t i, Ball* p) { return initfunc(i, p); });
	if (isSpawn)
	{
		obj->Spawn();
	}

	obj->transform->position = position;

	// チームに合わせてトレイルカラー変更
	obj->SetTrailImage(m_hTrails["Green"]);

	obj->SetModel(m_Model);
	// テスト用 テクスチャをランダムで選択
	if (not m_Textures.empty())
	{
		BallTexture tex;
		std::string mapKey = "";

		if (GetRand(99) < 3)
		{
			tex = m_Textures.at("Unicorn");
		}
		else
		{
			int randMax = (int)m_Textures.size() - 1;
			int rand = GetRand(randMax);
			if (rand == randMax)
			{
				rand = GetRand(99) < 25 ? rand : 0;
			}

			auto item = (*std::next(m_Textures.begin(), rand));
			if (item.first == "Unicorn")
			{
				tex = m_Textures.at("Lady_Bug");
			}
			else
			{
				tex = item.second;
			}
			mapKey = item.first;
		}

		obj->SetTexture(tex, mapKey);
	}
	m_pPool->SetObjectPointer(index, obj);

	auto& net = NetworkRef::Inst();
	if (net.IsNetworkEnable)
	{
		if (net.IsHost)
		{
			char BUF[UUID4_LEN];
			uuid4_init();
			uuid4_generate(BUF);
			obj->SetUniqueID(BUF);
		}
	}

	return obj;
#else
	Ball* ball = Instantiate<Ball>();
	ball->transform->position = position;
	m_Balls.push_back(ball);
	return ball;
#endif
	return nullptr;
}

Ball* BallManager::GetBall(uint32_t index)
{
#ifdef USE_POOL
	if (not m_pPool)
		return nullptr;

	if (index > m_pPool->GetCapacity())
		return nullptr;

    auto obj = m_pPool->GetItem(index);

    if (obj == nullptr)
        return nullptr;

    auto ball = obj->m_pObject;
    if (ball == nullptr)
        return nullptr;

	return ball;

#else
	// Pool使わない版 未実装

#endif

}

int BallManager::GetTrailImage(const std::string& teamColor)
{
	if (teamColor == "None")
		return DX_NONE_GRAPH;

	return m_hTrails[teamColor];
}

const BallTexture& BallManager::GetBallTexture(std::string key)
{
	return m_Textures[key];
}

Ball* BallManager::initfunc(uint32_t index, Ball* pBall)
{
	pBall = Instantiate<Ball>();
	pBall->m_Index = index;
	return pBall;
}

void BallManager::loadTextures()
{
	std::list<std::string> fileNames = FileUtil::FindFileNames(FOLDER_TEXTURE, true);

	for (std::string fileName : fileNames)
	{
		BallTexture tex;
		tex.Texture = ResourceLoader::LoadGraph(FOLDER_TEXTURE + fileName + ".png");

		nlohmann::json json;

		std::ifstream ifJson(FOLDER_JSON + "/" + fileName + ".json");
		ifJson >> json;
		ifJson.close();

		tex.FrameCountAll = json.at("FrameCountAll").get<int>();
		tex.FrameCountX = json.at("FrameCountX").get<int>();

		m_Textures.emplace(fileName, tex);
	}
}
