#include "src/scene/play/chara/CharaManager.h"
#include "src/util/file/resource_loader/ResourceLoader.h"

#ifndef USE_POOL
#include "src/util/ptr/PtrUtil.h"
#endif

#include "src/common/component/physics/Physics.h"
#include "src/common/component/collider/CollisionDefine.h"
#include "src/common/component/collider/ColliderCapsule.h"

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
	m_pPool = new Pool<CharaBase>(m_Max);
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
		// �������Ă���Ȃ�
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
		// �������Ă���Ȃ�
		if (it->IsActive())
		{
			it->Draw();
		}
	}
#endif
}

CharaBase* CharaManager::Create(const std::string& tag, const Transform& trs) 
{
	//////////////////////////////////////////////////////////////
	// ��O����

#ifdef USE_POOL
	if (m_pPool->CheckActiveObjectByCount(m_pPool->GetCapacity()))
	{
		return nullptr;
	}
#else
	if (m_Charas.size() >= CHARA_NUM)
		return nullptr;
#endif
	CharaBase* newChara = nullptr;

	//////////////////////////////////////////////////////////////
	// �C���f�b�N�X�擾�E�C���X�^���X�̐���

#ifdef USE_POOL
	uint32_t index = m_pPool->GetIndex();

	newChara = m_pPool->Alloc();
#else
	newChara = new CharaBase();
#endif

	//////////////////////////////////////////////////////////////
	// ���X�̐ݒ�

	*newChara->transform = trs;

	int hModel = -1;
	// �����蔻��̍\�z
	ColDefine::ColBaseParam colParamChara;
	colParamChara.trs.scale = Vector3(70.0f);
	colParamChara.onlyOnce = false;

	if (tag == "Red")
	{
		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Ch06_nonPBR.mv1");

		colParamChara.tag = ColDefine::Tag::tChara;
		colParamChara.targetTags = { ColDefine::Tag::tChara, ColDefine::Tag::tBall, ColDefine::Tag::tTackle, ColDefine::Tag::tBlue };
	}
	else if (tag == "Blue")
	{
		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Ch06_nonPBR.mv1");

		colParamChara.tag = ColDefine::Tag::tChara;
		colParamChara.targetTags = { ColDefine::Tag::tChara, ColDefine::Tag::tBall, ColDefine::Tag::tTackle, ColDefine::Tag::tRed };
	}
	colParamChara.targetTags.push_back(ColDefine::Tag::tWindArea);

	//newChara->SetTrailImage(m_hTrails[tag]);
	newChara->SetTrailImage(m_hTrails["Plain_Distortion_Thin"]);

	// ���f�������]���Ă���̂�180�x��]�����Ē���
	int origin = MV1SearchFrame(hModel, "mixamorig9:Hips");
	MV1SetFrameUserLocalMatrix(hModel, origin, MGetRotY(MathUtil::PI) * MGetTranslate(Vector3(0.0f, 100.0f, 0.0f)));

	newChara->SetModel(hModel);
	newChara->SetTransform(trs);

	// ����������ݒ�
	newChara->AddComponent<Physics>()->Init(GRAVITY, FRICTION);

	// �����蔻���ݒ�
	ColliderCapsule* colliderChara = newChara->AddComponent<ColliderCapsule>();
	colliderChara->BaseInit(colParamChara);
	colliderChara->SetOffset(Vector3::SetY(130.0f));
	//colliderChara->SetDraw(true);

	newChara->m_Index = index;
	newChara->Init(tag);

	//////////////////////////////////////////////////////////////
	// ���X�g�ւ̓o�^

#ifdef USE_POOL
	m_pPool->SetObjectPointer(index, newChara);
#else
	m_Charas.push_back(newChara);
#endif

	return newChara;
}

CharaBase* CharaManager::Create(const std::string& tag, const Transform& trs, const User& user)
{
	auto c = Create(tag, trs);
	c->SetUser(user);
	return c;
}

const CharaBase* CharaManager::CharaInst(int index)
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

CharaBase* CharaManager::NearestEnemy(int index) {

	const CharaBase* chara = CharaInst(index);
	if (chara == nullptr)
		return nullptr;

	for (const auto& it : m_pPool->GetAllItems()) {

		if (it->m_pObject == nullptr)
			continue;

		// �ԍ��������������́A�`�[���������ꍇ
		if (it->m_Index == index || it->m_pObject->m_CharaTag == chara->m_CharaTag)
			continue;

		// �����v�Z��ǔ���������

		return it->m_pObject;
	}

	return nullptr;
}

CharaBase* CharaManager::GetFromUUID(UINT uuid)
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

CharaBase* CharaManager::initfunc(uint32_t index, CharaBase* pChara)
{
	return nullptr;
}
