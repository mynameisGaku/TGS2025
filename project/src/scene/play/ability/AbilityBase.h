#pragma once
#include "src/util/object3D/Object3D.h"
#include "src/util/time/GameTime.h"

/// <summary>
/// 炎や氷などの、各種効果処理を行う
/// </summary>
class AbilityBase : public Object3D
{
public:
	AbilityBase() :
		m_Duration_Sec(0.0f)
	{
	}

	virtual ~AbilityBase() {}

	void Update()
	{
		if (m_Duration_Sec > 0.0f)
		{
			m_Duration_Sec -= GTime.DeltaTime();
			Detalis();
		}

		if (m_Duration_Sec <= 0.0f)
		{
			m_Duration_Sec = 0.0f;
			DestroyMe();
		}
	}

	/// <summary>
	/// 特殊効果(生存中毎フレーム呼び出される)
	/// </summary>
	virtual void Detalis() {}

protected:
	float m_Duration_Sec;

};