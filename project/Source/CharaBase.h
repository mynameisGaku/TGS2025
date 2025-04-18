#pragma once
#include "Object3D.h"
#include <Library/time.h>

class CharaStamina;

/// <summary>
/// �L�����N�^�[�Ɋւ�����N���X
/// </summary>
class CharaBase : public Object3D
{
public:

	CharaBase();
	virtual ~CharaBase();

	virtual void Update() override;

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData"></param>
	void CollisionEvent(const CollisionData& colData) override;

	/// <summary>
	/// �n�ʂƂ̓����蔻��
	/// </summary>
	void HitGroundProcess();

private:
	CharaStamina* m_Stamina; // �X�^�~�i�̃|�C���^�[
};