#pragma once
#include "framework/gameObject.h"
#include "src/util/pool/Pool.h"
#include "src/reference/network/NetworkRef.h"

class Enemy;

/// <summary>
/// �G�}�l�[�W���[
/// </summary>
/// <author>�����h�l</author>
class EnemyManager : public GameObject
{
public:
	EnemyManager();
	~EnemyManager();
	void Update() override;
	void Draw() override;

	Enemy* Create();
private:
	int m_Model;

	Pool<Enemy>* m_Pool;
	NetworkRef& m_NetRef;
};
