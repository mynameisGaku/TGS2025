#pragma once
#include "framework/gameObject.h"

class BallTarget;
class BallTarget_WithParent;
template <class C>
class Pool;
class CharaManager;

/// <summary>
/// ボールターゲットを管理
/// </summary>
/// <author>佐藤紘斗</author>
class BallTargetManager : public GameObject
{
public:
	BallTargetManager();
	~BallTargetManager();
	void Update() override;
	void Draw() override;

	// ボールターゲットを生成
	BallTarget* Create();

	/// <summary>
	/// ボールターゲットを取得
	/// </summary>
	/// <param name="index">プールのインデックス</param>
	/// <returns>ボールターゲット</returns>
	BallTarget* Get(uint32_t index) const;


	/// <summary>
	/// 指定したキャラに対して、最も近く、味方ではないボールターゲットを取得する
	/// </summary>
	/// <param name="index">キャラID</param>
	/// <param name="distance">ロックオン制限距離</param>
	BallTarget* GetNearest(int index, float distance) const;
private:
	Pool<BallTarget>* m_Pool;
	CharaManager* m_pCharaManager;
};
