#pragma once
#include "Library/gameObject.h"
#include "Util/Transform.h"

#include "CharaDefine.h"
#include "CharaBase.h"

class CharaManager : public GameObject {
public:

	CharaManager();
	virtual ~CharaManager();

	void Update() override;
	void Draw() override;

	void Create(CharaDefine::CharaTag tag, const Transform& trs);

private:
	//Pool<CharaBase*> m_CharaPool;
	std::list<CharaBase*> m_Charas;
};