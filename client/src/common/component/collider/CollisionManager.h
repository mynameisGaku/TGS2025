#pragma once

// ◇継承元
#include "framework/gameObject.h"

class ColliderBase;

/// <summary>
/// 当たり判定の管理を行う
/// </summary>
class CollisionManager : public GameObject {
public:
	//==========================================================================================
	// ▼コンストラクタ・デストラクタ

	CollisionManager();
	~CollisionManager();

	//==========================================================================================
	// ▼各種関数

	void Update() override;
	void Draw() override;

	/// <summary>
	/// 管理者への登録を行う
	/// </summary>
	/// <param name="col">当たり判定の情報</param>
	void Add(ColliderBase* col);
	
	/// <summary>
	/// 管理者への登録解除を行う
	/// </summary>
	/// <param name="col">当たり判定の情報</param>
	void Remove(ColliderBase* col);

private:
	std::list<ColliderBase*> colliders;

};