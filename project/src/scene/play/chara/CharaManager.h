#pragma once
#include "framework/gameObject.h"
#include "src/util/transform/Transform.h"
#include <vector>

#include "src/scene/play/chara/CharaBase.h"
#include "src/util/pool/Pool.h"

#define USE_POOL

class CharaManager : public GameObject {
public:

	CharaManager();
	virtual ~CharaManager();

	void Update() override;
	void Draw() override;

	/// <summary>
	/// キャラを生成する
	/// </summary>
	/// <param name="tag">
	/// <para>生成したいキャラのタグを指定</para>
	/// <para> (tag)プレイヤー	: tPlayer</para>
	/// <para> (tag)エネミー	: tEnemy </para>
	/// </param>
	/// <param name="trs">生成時のトランスフォーム</param>
	/// <returns>生成されたキャラのポインタ</returns>
	CharaBase* Create(const std::string& tag, const Transform& trs);
	const CharaBase* CharaInst(int index);

	/// <summary>
	/// 指定したキャラに対して、最も近い敵キャラを取得する
	/// </summary>
	/// <param name="index">キャラID</param>
	const CharaBase* TargetChara(int index);

private:

	CharaBase* initfunc(uint32_t index, CharaBase* pChara);

	int						 m_Max;
	std::vector<std::string> m_Tags;

#ifdef USE_POOL
	Pool<CharaBase>*		 m_pPool;
#else
	std::vector<CharaBase*>  m_Charas;
#endif
};