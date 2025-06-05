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
	/// �L�����𐶐�����
	/// </summary>
	/// <param name="tag">
	/// <para>�����������L�����̃^�O���w��</para>
	/// <para> (tag)�v���C���[	: tPlayer</para>
	/// <para> (tag)�G�l�~�[	: tEnemy </para>
	/// </param>
	/// <param name="trs">�������̃g�����X�t�H�[��</param>
	/// <returns>�������ꂽ�L�����̃|�C���^</returns>
	CharaBase* Create(const std::string& tag, const Transform& trs);
	const CharaBase* CharaInst(int index);

	/// <summary>
	/// �w�肵���L�����ɑ΂��āA�ł��߂��G�L�������擾����
	/// </summary>
	/// <param name="index">�L����ID</param>
	CharaBase* TargetChara(int index);

	/// <summary>
	/// �L�����̃v�[�����擾����
	/// </summary>
	/// <returns>�v�[��</returns>
	Pool<CharaBase>* GetCharaPool() const { return m_pPool; }

private:

	CharaBase* initfunc(uint32_t index, CharaBase* pChara);

	int						 m_Max;
	std::vector<std::string> m_Tags;
	std::unordered_map<std::string, int> m_hTrails;

#ifdef USE_POOL
	Pool<CharaBase>*		 m_pPool;
#else
	std::vector<CharaBase*>  m_Charas;
#endif
};