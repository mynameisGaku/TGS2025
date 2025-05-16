#include "CharaManager.h"
#include "Library/resourceLoader.h"
#include "Util/Utils.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Component/ColliderCapsule.h"

#include "CharaDefineRef.h"

CharaManager::CharaManager()
{
	CHARADEFINE_REF.Load(true);
	m_Max = CHARADEFINE_REF.Max;
	m_Tags = CHARADEFINE_REF.Tags;

#ifdef USE_POOL
	m_pPool = new Pool<CharaBase>(m_Max);
#else
	m_Charas.clear();
#endif
}

CharaManager::~CharaManager()
{
#ifdef USE_POOL
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
	Function::DeletePointerVector(m_Charas);
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
		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Character.mv1");

		colParamChara.tag = ColDefine::Tag::tCharaRed;
		colParamChara.targetTags = { ColDefine::Tag::tCharaBlue, ColDefine::Tag::tBallBlue };
	}
	else if (tag == "Blue")
	{
		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Character.mv1");

		colParamChara.tag = ColDefine::Tag::tCharaBlue;
		colParamChara.targetTags = { ColDefine::Tag::tCharaRed, ColDefine::Tag::tBallRed };
	}

	// ���f�������]���Ă���̂�180�x��]�����Ē���
	int origin = MV1SearchFrame(hModel, "mixamorig:Hips");
	MV1SetFrameUserLocalMatrix(hModel, origin, MGetRotY(Math::PI) * MGetTranslate(Vector3(0.0f, 100.0f, 0.0f)));

	newChara->SetModel(hModel);
	newChara->SetTransform(trs);

	// ����������ݒ�
	newChara->AddComponent<Physics>()->Init(GRAVITY, FRICTION);

	// �����蔻���ݒ�
	ColliderCapsule* colliderChara = newChara->AddComponent<ColliderCapsule>();
	colliderChara->BaseInit(colParamChara);
	colliderChara->SetOffset(V3::SetY(130.0f));
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

const CharaBase* CharaManager::TargetChara(int index) {

	const CharaBase* chara = CharaInst(index);
	if (chara == nullptr)
		return nullptr;

	for (const auto& it : m_pPool->GetAllItems()) {
		// �ԍ��������������́A�`�[���������ꍇ
		if (it->m_Index == index || it->m_pObject->m_CharaTag == chara->m_CharaTag)
			continue;

		// �����v�Z��ǔ���������

		return it->m_pObject;
	}

	return nullptr;
}

CharaBase* CharaManager::initfunc(uint32_t index, CharaBase* pChara)
{
	return nullptr;
}
