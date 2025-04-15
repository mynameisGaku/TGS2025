#include "CharaManager.h"
#include "Library/resourceLoader.h"
#include "Util/Utils.h"

using namespace CharaDefine;

CharaManager::CharaManager()
{
	m_Charas.clear();
}

CharaManager::~CharaManager()
{
	Function::DeletePointerList(m_Charas);
}

void CharaManager::Update()
{
	for (auto it = m_Charas.begin(); it != m_Charas.end();)
	{
		// �������Ă���Ȃ�
		if ((*it)->IsActive())
		{
			(*it)->Update();
			return;
		}

		delete* it;
		m_Charas.erase(it);
	}
}

void CharaManager::Draw()
{
	for (const auto& it : m_Charas)
	{
		// �������Ă���Ȃ�
		if (it->IsActive())
		{
			it->Draw();
		}
	}
}

void CharaManager::Create(CharaDefine::CharaTag tag, const Transform& trs) {

	if (m_Charas.size() >= CHARA_NUM)
		return;

	CharaBase* newChara = new CharaBase();
	int hModel = -1;

	switch (tag)
	{
	case CharaDefine::CharaTag::tPlayer:	hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Ch06_nonPBR.mv1");	break;
	case CharaDefine::CharaTag::tEnemy:		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Ch06_nonPBR.mv1");	break;
		break;
	}

	newChara->SetModel(hModel);
	newChara->SetTransform(trs);

	m_Charas.push_back(newChara);
}
