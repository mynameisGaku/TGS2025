#include "EnemyManager.h"
#include "src/scene/play/enemy/Enemy.h"
#include "src/util/file/resource_loader/ResourceLoader.h"


namespace
{
	static const int COUNT_MAX = 128;
}

EnemyManager::EnemyManager() : 
	m_NetRef(NetworkRef::Inst())
{
	m_Model = ResourceLoader::MV1LoadModel("data/Model/Enemy/Bat/Bat.mv1");

	m_Pool = new Pool<Enemy>(COUNT_MAX);
}

EnemyManager::~EnemyManager()
{
	// ���\�[�X���[�_�[�͒P�̂Ńf���[�g�ł��Ȃ����ǁA�ꉞ�����t�������܂�
	ResourceLoader::MV1DeleteModel(m_Model);

	for (auto& item : m_Pool->GetAllItems())
	{
		delete item->m_pObject;
		delete item;
	}
	
	delete m_Pool;
}

void EnemyManager::Update()
{
	auto items = m_Pool->GetAllItems();
	for (auto& item : items)
	{
		Enemy* enemy = item->m_pObject;

		if (enemy == nullptr)
			continue;

		if (enemy->IsActive())
		{
			enemy->Update();
		}
		else
		{
			m_Pool->DeActive(item->m_Index);
		}
	}
}

void EnemyManager::Draw()
{
	auto items = m_Pool->GetAllItems();
	for (auto& item : items)
	{
		Enemy* enemy = item->m_pObject;

		if (enemy == nullptr)
			continue;

		if (enemy->IsActive())
		{
			enemy->Draw();
		}
	}
}

Enemy* EnemyManager::Create()
{
	// ���ɃA�N�e�B�u���ő吔�Ȃ琶�����Ȃ�
	if (m_Pool->CheckActiveObjectByCapacity()) return nullptr;

	uint32_t index = m_Pool->GetIndex();
	Enemy* enemy = m_Pool->Alloc();

	enemy->SetModel(m_Model);
	enemy->SetTransform(Transform(Vector3(), Vector3(), Vector3(5, 5, 5)));

	m_Pool->SetObjectPointer(index, enemy);

	return enemy;
}
