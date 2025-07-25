#pragma once
#include "framework/gameObject.h"

class BallTarget;
class BallTarget_WithParent;
template <class C>
class Pool;

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
	BallTarget* Get(uint32_t index);
private:
	Pool<BallTarget>* m_Pool;
};
