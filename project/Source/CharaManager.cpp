#include "CharaManager.h"
#include "Library/resourceLoader.h"
#include "Util/Utils.h"

#include "Component/Physics.h"
#include "Component/CollisionDefine.h"
#include "Component/ColliderCapsule.h"

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
			it++;
			continue;
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
	// “–‚½‚è”»’è‚Ì\’z
	ColDefine::ColBaseParam colParamChara;
	colParamChara.trs.scale = Vector3(70.0f);
	colParamChara.onlyOnce = false;

	switch (tag)
	{
	case CharaDefine::CharaTag::tRed:
		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Ch06_nonPBR.mv1");

		colParamChara.tag = ColDefine::Tag::tCharaRed;
		colParamChara.targetTags = { ColDefine::Tag::tCharaBlue, ColDefine::Tag::tBallBlue };
		break;

	case CharaDefine::CharaTag::tBlue:
		hModel = ResourceLoader::MV1LoadModel("data/model/Chara/Ch06_nonPBR.mv1");

		colParamChara.tag = ColDefine::Tag::tCharaBlue;
		colParamChara.targetTags = { ColDefine::Tag::tCharaRed, ColDefine::Tag::tBallRed };
		break;
	}

	// ƒ‚ƒfƒ‹‚ª”½“]‚µ‚Ä‚¢‚é‚Ì‚ð180“x‰ñ“]‚³‚¹‚Ä’¼‚·
	int origin = MV1SearchFrame(hModel, "mixamorig9:Hips");
	MV1SetFrameUserLocalMatrix(hModel, origin, MGetRotY(Math::PI) * MGetTranslate(Vector3(0.0f, 100.0f, 0.0f)));

	newChara->SetModel(hModel);
	newChara->SetTransform(trs);

	// •¨—‹““®‚ðÝ’è
	newChara->AddComponent<Physics>()->Init(GRAVITY, FRICTION);

	// “–‚½‚è”»’è‚ðÝ’è
	ColliderCapsule* colliderChara = newChara->AddComponent<ColliderCapsule>();
	colliderChara->BaseInit(colParamChara);
	colliderChara->SetOffset(V3::SetY(130.0f));
	colliderChara->SetDraw(true);

	newChara->Init(tag);

	m_Charas.push_back(newChara);

	return newChara;
}

const CharaBase* CharaManager::CharaInst(int index) {

	if (index < 0 || index > m_Charas.size())
		return nullptr;

	return m_Charas[index];
}
