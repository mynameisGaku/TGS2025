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
	/// �L�����𐶐�����
	/// </summary>
	/// <param name="tag">
	/// <para>�����������L�����̃^�O���w��</para>
	/// <para> (tag)�v���C���[	: tPlayer</para>
	/// <para> (tag)�G�l�~�[	: tEnemy </para>
	/// </param>
	/// <param name="trs">�������̃g�����X�t�H�[��</param>
	/// <returns>�������ꂽ�L�����̃|�C���^</returns>
	Chara* Create(const std::string& tag, const Transform& trs);
	
	/// <summary>
	/// �L�����𐶐�����
	/// </summary>
	/// <param name="tag">
	/// <para>�����������L�����̃^�O���w��</para>
	/// <para> (tag)�v���C���[	: tPlayer</para>
	/// <para> (tag)�G�l�~�[	: tEnemy </para>
	/// </param>
	/// <param name="user">���[�U�[���</param>
	/// <param name="trs">�������̃g�����X�t�H�[��</param>
	/// <returns>�������ꂽ�L�����̃|�C���^</returns>
	Chara* Create(const std::string& tag, const Transform& trs, const User& user);


	const Chara* CharaInst(int index);

	/// <summary>
	/// �w�肵���L�����ɑ΂��āA�ł��߂��G�L�������擾����
	/// </summary>
	/// <param name="index">�L����ID</param>
	Chara* NearestEnemy(int index);

	/// <summary>
	/// �L�����̃v�[�����擾����
	/// </summary>
	/// <returns>�v�[��</returns>
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