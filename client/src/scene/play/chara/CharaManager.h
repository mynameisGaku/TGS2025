#pragma once
#include "framework/gameObject.h"
#include "src/util/transform/Transform.h"
#include <vector>

#include "src/scene/play/chara/Chara.h"
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
	Chara* Create(const std::string& tag, const Transform& trs);
	
	/// <summary>
	/// キャラを生成する
	/// </summary>
	/// <param name="tag">
	/// <para>生成したいキャラのタグを指定</para>
	/// <para> (tag)プレイヤー	: tPlayer</para>
	/// <para> (tag)エネミー	: tEnemy </para>
	/// </param>
	/// <param name="user">ユーザー情報</param>
	/// <param name="trs">生成時のトランスフォーム</param>
	/// <returns>生成されたキャラのポインタ</returns>
	Chara* Create(const std::string& tag, const Transform& trs, const User& user);


	const Chara* CharaInst(int index);

	/// <summary>
	/// 指定したキャラに対して、最も近い敵キャラを取得する
	/// </summary>
	/// <param name="index">キャラID</param>
	Chara* NearestEnemy(int index);

	/// <summary>
	/// キャラのプールを取得する
	/// </summary>
	/// <returns>プール</returns>
	Pool<Chara>* GetCharaPool() const { return m_pPool; }

    Chara* GetFromUUID(const std::string& uuid);

private:

	Chara* initfunc(uint32_t index, Chara* pChara);

	int						 m_Max;
	std::vector<std::string> m_Tags;
	std::unordered_map<std::string, int> m_hTrails;

#ifdef USE_POOL
	Pool<Chara>*		 m_pPool;
#else
	std::vector<CharaBase*>  m_Charas;
#endif
};