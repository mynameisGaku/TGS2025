#pragma once
#include "framework/gameObject.h"

class BallTarget;
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

	/// <summary>
	/// ボールターゲットを生成
	/// </summary>
	/// <param name="position">位置</param>
	/// <returns>生成したボールターゲット</returns>
	BallTarget* Create(const Vector3& position);

	/// <summary>
	/// ボールターゲットを取得
	/// </summary>
	/// <param name="index">インデックス</param>
	/// <returns>ボールターゲット</returns>
	BallTarget* GetBallTarget(uint32_t index);
private:
	Pool<BallTarget>* m_Pool;
};
