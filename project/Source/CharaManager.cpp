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
	Function::DeletePointerVector(m_Charas);
}

void CharaManager::Update()
{
	for (auto it = m_Charas.begin(); it != m_Charas.end();)
	{
		// Šˆ“®‚µ‚Ä‚¢‚é‚È‚ç
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
		// Šˆ“®‚µ‚Ä‚¢‚é‚È‚ç
		if (it->IsActive())
		{
			it->Draw();
		}
	}
}

CharaBase* CharaManager::Create(CharaDefine::CharaTag tag, const Transform& trs) {

	if (m_Charas.size() >= CHARA_NUM)
		return nullptr;

	CharaBase* newChara = new CharaBase();
	int hModel = -1;

	switch (tag)
	{
	case CharaDefine::CharaTag::tPlayer:	hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Ch06_nonPBR.mv1");	break;
	case CharaDefine::CharaTag::tEnemy:		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Ch06_nonPBR.mv1");	break;
		break;
	}

	// ƒ‚ƒfƒ‹‚ª”½“]‚µ‚Ä‚¢‚é‚Ì‚ğ180“x‰ñ“]‚³‚¹‚Ä’¼‚·
	int origin = MV1SearchFrame(hModel, "mixamorig9:Hips");
	MV1SetFrameUserLocalMatrix(hModel, origin, MGetRotY(Math::PI) * MGetTranslate(Vector3(0.0f, 100.0f, 0.0f)));

	newChara->SetModel(hModel);
	newChara->SetTransform(trs);

	m_Charas.push_back(newChara);

	return newChara;
}

const CharaBase* CharaManager::CharaInst(int index) {

	if (index < 0 || index > m_Charas.size())
		return nullptr;

	return m_Charas[index];
}
