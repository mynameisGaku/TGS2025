#pragma once

// ���p����
#include "Component.h"

// ���ėp
#include "../Util/Utils.h"
#include "../Util/Transform.h"

namespace {

	// ����̏d�͉����x
	static const Vector3 GRAVITY = V3::SetY(Math::GRAVITY_3D);

	// ����̖��C�W��
	static const Vector3 FRICTION = Vector3(0.9f);
}

/// <summary>
/// ���x���Ǘ�����R���|�[�l���g
/// </summary>
class Physics : public Component {
public:
	//==========================================================================================
	// �������o�ϐ�

	// ���x�B���t���[�����x���������W���ړ�����
	Vector3 velocity;

	// �p���x�B���t���[�����x��������]����
	Vector3 angularVelocity;

	// ��R�́B���x�ɉe������Ȃ��ړ����s�����Ɏg�p����
	Vector3 resistance;

	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	Physics();
	~Physics();

	//==========================================================================================
	// ���e��֐�

	/// <summary>
	/// �������������s��
	/// </summary>
	/// <param name="gravity">�d�͉����x</param>
	/// <param name="friction">���C�W��</param>
	void Init(const Vector3& gravity, const Vector3& friction);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �d�͉����x�����̍X�V
	/// </summary>
	void GravityUpdate();

	/// <summary>
	/// ���������̍X�V
	/// </summary>
	void VelocityUpdate();

	/// <summary>
	/// ��R�����̍X�V
	/// </summary>
	void ResistanceUpdate();

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �d�͉����x��ݒ肷��
	/// </summary
	inline void SetGravity(const Vector3& _gravity) { gravity = _gravity; }

	/// <summary>
	/// ���C�W����ݒ肷��
	/// </summary>
	inline void SetFriction(const Vector3& _friction) { friction = _friction; }

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// 1�t���[���O�̃g�����X�t�H�[���̃R�s�[���擾����
	/// </summary>
	inline const Transform* const LastTransform() const { return lastTransform; }

	/// <summary>
	/// �d�͉����x���擾����
	/// </summary>
	inline Vector3 const Gravity() const { return gravity; }

	/// <summary>
	/// ���C�W�����擾����
	/// </summary>
	inline Vector3 const Friction() const { return friction; }

	/// <summary>
	/// ���������̑��x���擾����
	/// </summary>
	inline Vector3 const FlatVelocity() const { return Vector3(velocity.x, 0, velocity.z); }

	/// <summary>
	/// ���������̑��x���擾����
	/// </summary>
	inline Vector3 const UpVelocity() const { return Vector3(0, velocity.y, 0); }

private:
	//==========================================================================================
	// �������o�ϐ�

	// �����N����g�����X�t�H�[���̃|�C���^
	// ���x�ɉ����ă����N�����g�����X�t�H�[���̍��W�𖈃t���[���ω�������
	// ���̃N���X����new�Adelete�͂��Ȃ�
	Transform* pTransform;

	// 1�t���[���O�̃g�����X�t�H�[���̃R�s�[
	// �����蔻��ȂǂŁA�t���[���Ԃ̕�ԂɎg��
	Transform* lastTransform;

	// �d�͉����x�B�t���[�����ɑ��x�ɉ��Z�����
	// ���ʂ̉������d�͂Ȃ�y�����}�C�i�X�ɂ��đ���0�ɂ���Ηǂ�
	Vector3 gravity;

	// ���C�W��
	Vector3 friction;

	// �������̋�C��R
	float fallingAirResistance;
};
