#pragma once
#include "framework/gameObject.h"

class Enemy;

/// <summary>
/// “Gƒ}ƒl[ƒWƒƒ[
/// </summary>
/// <author>²“¡h“l</author>
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

	std::list<Enemy*> m_Enemies;
};
