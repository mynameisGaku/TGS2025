#pragma once
#include "src/util/transform/Transform.h"

/// <summary>
/// ボールを投げるときロックオンできるターゲット
/// </summary>
/// <author>佐藤紘斗</author>
class BallTarget
{
public:
	BallTarget();
	BallTarget(const Vector3& position);

	virtual void Update() {}
	void Draw() {}

	void SetPosition(const Vector3& position) { m_Position = position; }
	const Vector3& Position() const { return m_Position; }

	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool IsActive() const { return m_IsActive; }
private:
	Vector3 m_Position;	// ターゲットの位置
	bool m_IsActive;	// ターゲットがアクティブかどうか
};

/// <summary>
/// 親とオフセットで座標を更新できるターゲット
/// </summary>
/// <author>佐藤紘斗</author>
class BallTarget_WithParent : public BallTarget
{
public:
	BallTarget_WithParent(const Vector3& offset, Transform* parent, bool doAutoUpdate = false);

	void Update() override;
	void UpdatePosition();
private:
	Vector3 m_Offset;		// 親からのオフセット座標
	Transform* m_Parent;	// 親のTransform
	bool m_DoAutoUpdate;	// 親の座標更新を自動で行うかどうか
};
