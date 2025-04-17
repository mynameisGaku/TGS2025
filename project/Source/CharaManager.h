#pragma once
#include "Library/gameObject.h"
#include "Util/Transform.h"
#include <vector>

#include "CharaDefine.h"
#include "CharaBase.h"

class CharaManager : public GameObject {
public:

	CharaManager();
	virtual ~CharaManager();

	void Update() override;
	void Draw() override;

	void Create(CharaDefine::CharaTag tag, const Transform& trs);
	const CharaBase* CharaInst(int index);

private:
	//Pool<CharaBase*> m_CharaPool;
	std::vector<CharaBase*> m_Charas;
};