#pragma once
#include "colliderBase.h"

/// <summary>
/// �J�v�Z�������蔻��
/// </summary>
class ColliderCapsule : public ColliderBase {
public:
	//==========================================================================================
	// ���R���X�g���N�^

	ColliderCapsule();
	~ColliderCapsule() {};

	//==========================================================================================
	// ���e��֐�

	void Draw() override;

	//==========================================================================================
	// ���Z�b�^�[

	inline void SetOffset(Vector3 _offset) { offset = _offset; }

	//==========================================================================================
	// ���Q�b�^�[

	// ���a���擾����
	inline float Radius() const { return transform->Global().scale.Average() / 2.0f; }

	inline Vector3 Offset() const { return offset; }

private:
	Vector3 offset;

};