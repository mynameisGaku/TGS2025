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

	void SetPosition(const Vector3& position) { m_Position = position; }
	const Vector3& Position() const { return m_Position; }
private:
	Vector3 m_Position;
};

/// <summary>
/// 親とオフセットで座標を更新できるターゲット
/// </summary>
/// <author>佐藤紘斗</author>
class BallTarget_WithParent : public BallTarget
{
public:
	BallTarget_WithParent(const Vector3& offset, Transform* parent);

	void UpdatePosition();
private:
	Vector3 m_Offset;
	Transform* m_Parent;
};
