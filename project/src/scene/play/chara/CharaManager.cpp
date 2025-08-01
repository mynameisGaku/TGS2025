#include "src/scene/play/chara/CharaManager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"

#ifndef USE_POOL
#include "src/util/ptr/PtrUtil.h"
#endif

#include "src/common/component/physics/Physics.h"
#include "src/common/component/collider/CollisionDefine.h"
#include "src/common/component/collider/ColliderCapsule.h"
#include "src/common/component/material_renderer/MaterialRenderer.h"

#include "src/reference/chara/CharaDefineRef.h"

CharaManager::CharaManager()
{
	CHARADEFINE_REF.Load(true);
	m_Max = CHARADEFINE_REF.Max;
	m_Tags = CHARADEFINE_REF.Tags;

	m_hTrails["Green"] = LoadGraph("data/img/trail/Trail_Green.png");
	m_hTrails["Blue"] = LoadGraph("data/img/trail/Trail_Blue.png");
	m_hTrails["Red"] = LoadGraph("data/img/trail/Trail_Red.png");
	m_hTrails["Plain"] = LoadGraph("data/img/trail/Trail_Plain.png");
	m_hTrails["Plain_Distortion"] = LoadGraph("data/img/trail/Trail_Plain_Distortion.png");
	m_hTrails["Plain_Distortion_Thin"] = LoadGraph("data/img/trail/Trail_Plain_Distortion_Thin.png");

#ifdef USE_POOL
	m_pPool = new Pool<Chara>(m_Max);
#else
	m_Charas.clear();
#endif
}

CharaManager::~CharaManager()
{
	for (auto& it : m_hTrails)
	{
		DeleteGraph(it.second);
	}
	m_hTrails.clear();
#ifdef USE_POOL
	for (auto& item : m_pPool->GetAllItems())
	{
		if (item == nullptr)
			continue;

		if (item->m_pObject != nullptr)
		{
			delete item->m_pObject;
			item->m_pObject = nullptr;
		}

		delete item;
		item = nullptr;
	}
	delete m_pPool;
#else
	PtrUtil::SafeDeleteVector(m_Charas);
#endif
}

void CharaManager::Update()
{
#ifdef USE_POOL
	auto items = m_pPool->GetAllItems();
	for (auto& item : items)
	{
		auto chara = item->m_pObject;

		if (chara == nullptr)
			continue;

		if (chara->IsActive())
		{
			chara->Update();
		}
		else
		{
			m_pPool->DeActive(item->m_Index);
		}
	}
#else
	for (auto it = m_Charas.begin(); it != m_Charas.end();)
	{
		// 活動しているなら
		if ((*it)->IsActive())
		{
			(*it)->Update();
			it++;
			continue;
		}

		delete* it;
		m_Charas.erase(it);
	}
#endif

	if (CheckHitKey(KEY_INPUT_R))
	{
		CHARADEFINE_REF.Load(true);
	}
}

void CharaManager::Draw()
{
#ifdef USE_POOL
	auto items = m_pPool->GetAllItems();
	for (auto& item : items)
	{
		auto chara = item->m_pObject;

		if (chara == nullptr)
			continue;

		if (chara->IsActive())
		{
			chara->Draw();
		}
	}
#else
	for (const auto& it : m_Charas)
	{
		// 活動しているなら
		if (it->IsActive())
		{
			it->Draw();
		}
	}
#endif
}

Chara* CharaManager::Create(const std::string& tag, const Transform& trs) 
{
	//////////////////////////////////////////////////////////////
	// 例外処理

#ifdef USE_POOL
	if (m_pPool->CheckActiveObjectByCapacity())
	{
		return nullptr;
	}
#else
	if (m_Charas.size() >= CHARA_NUM)
		return nullptr;
#endif
	Chara* newChara = nullptr;

	//////////////////////////////////////////////////////////////
	// インデックス取得・インスタンスの生成

#ifdef USE_POOL
	uint32_t index = m_pPool->GetIndex();

	newChara = m_pPool->Alloc();
#else
	newChara = new CharaBase();
#endif

	//////////////////////////////////////////////////////////////
	// 諸々の設定

	*newChara->transform = trs;

	int hModel = -1;
	// 当たり判定の構築
	ColDefine::ColBaseParam colParamChara;
	colParamChara.trs.scale = Vector3(70.0f);
	colParamChara.onlyOnce = false;

	if (tag == "Red")
	{
		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/NoFaceGuy_Original.mv1");

		colParamChara.tag = ColDefine::Tag::tChara;
		colParamChara.targetTags = { ColDefine::Tag::tChara, ColDefine::Tag::tBall, ColDefine::Tag::tTackle, ColDefine::Tag::tBlue, ColDefine::Tag::tGimmick };
	}
	else if (tag == "Blue")
	{
		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/NoFaceGuy_Original.mv1");

		colParamChara.tag = ColDefine::Tag::tChara;
		colParamChara.targetTags = { ColDefine::Tag::tChara, ColDefine::Tag::tBall, ColDefine::Tag::tTackle, ColDefine::Tag::tRed,ColDefine::Tag::tGimmick };
	}
	colParamChara.targetTags.push_back(ColDefine::Tag::tWindArea);

	//newChara->SetTrailImage(m_hTrails[tag]);
	newChara->SetTrailImage(m_hTrails["Plain_Distortion_Thin"]);

	// モデルが反転しているのを180度回転させて直す
	int origin = MV1SearchFrame(hModel, "mixamorig:Hips");
	MV1SetFrameUserLocalMatrix(hModel, origin, MGetRotY(MathUtil::PI));

	newChara->SetModel(hModel);
	newChara->SetTransform(trs);
	newChara->SetLocalMatrix(MGetScale(Vector3::Ones * 50.0f));

	// 物理挙動を設定
	newChara->AddComponent<Physics>()->Init(GRAVITY, FRICTION);

	// 当たり判定を設定
	ColliderCapsule* colliderChara = newChara->AddComponent<ColliderCapsule>();
	colliderChara->BaseInit(colParamChara);
	colliderChara->SetOffset(Vector3::SetY(130.0f));
	//colliderChara->SetDraw(true);

	MaterialRenderer* materialChara = newChara->AddComponent<MaterialRenderer>();
	materialChara->Init(newChara);
	materialChara->SetMaterialDifColor(0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f), 0.0f);

	newChara->m_Index = index;
	newChara->Init(tag);

	//////////////////////////////////////////////////////////////
	// リストへの登録

#ifdef USE_POOL
	m_pPool->SetObjectPointer(index, newChara);
#else
	m_Charas.push_back(newChara);
#endif

	return newChara;
}

Chara* CharaManager::Create(const std::string& tag, const Transform& trs, const User& user)
{
	auto c = Create(tag, trs);
	c->SetUser(user);
	return c;
}

const Chara* CharaManager::CharaInst(int index)
{
#ifdef USE_POOL
	if ((uint32_t)index > m_pPool->GetCapacity())
	{
		return nullptr;
	}

	return m_pPool->GetItem((uint32_t)index)->m_pObject;

#else
	if (index < 0 || index > m_Charas.size())
		return nullptr;

	return m_Charas[index];
#endif
}

Chara* CharaManager::NearestEnemy(int index, float distance) {

	const Chara* chara = CharaInst(index);
	if (chara == nullptr)
		return nullptr;

	for (const auto& it : m_pPool->GetAllItems()) {

		if (it->m_pObject == nullptr)
			continue;

		// 番号が同じもしくは、チームが同じ場合
		if (it->m_Index == index || it->m_pObject->m_CharaTag == chara->m_CharaTag)
			continue;

		// 距離計算や壁判定をいれる
		if ((chara->transform->position - it->m_pObject->transform->position).GetLengthSquared() >= distance * distance)
			continue;

		return it->m_pObject;
	}

	return nullptr;
}

Chara* CharaManager::GetFromUUID(const std::string& uuid)
{
#ifdef USE_POOL
    for (const auto& item : m_pPool->GetAllItems())
    {
        if (item->m_pObject == nullptr)
            continue;
        if (item->m_pObject->GetUser().UUID == uuid)
        {
            return item->m_pObject;
        }
    }
	return nullptr;
#endif
}

Chara* CharaManager::initfunc(uint32_t index, Chara* pChara)
{
	return nullptr;
}
