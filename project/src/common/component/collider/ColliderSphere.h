#pragma once
#include "colliderBase.h"

/// <summary>
/// ���`�����蔻��
/// </summary>
class ColliderSphere : public ColliderBase {
public:
	//==========================================================================================
	// ���R���X�g���N�^

	ColliderSphere();
	~ColliderSphere() {};

	//==========================================================================================
	// ���e��֐�

	void Draw() override;

	//==========================================================================================
	// ���Q�b�^�[

	// ���a���擾����
	inline float Radius() const { return transform->Global().scale.Average() / 2.0f; }

};