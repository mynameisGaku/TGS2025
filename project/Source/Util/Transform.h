#pragma once

// ���ėp
#include "Vector3.h"
#include <list>

/// <summary> 
/// Transform�̕���
/// </summary>
enum class TrsType {
	tNone = -1,
	tPos,	// ���W
	tRot,	// ��]
	tScale,	// �g�k
	tAll,	// �S��
};

/// <summary>
/// ���W�A��]�A�g�k�����N���X
/// </summary>
class Transform {
public:
	//==========================================================================================
	// �������o�ϐ�

	Vector3 position;	// ���W
	Vector3 rotation;	// ��]
	Vector3 scale;		// �g�k

	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	// �g�����X�t�H�[���̏�����
	Transform() : Transform(V3::ZERO, V3::ZERO, V3::ZERO, nullptr) {}

	/// <summary>
	/// �g�����X�t�H�[���̏�����
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="rotation">��]</param>
	/// <param name="scale">�g�k</param>
	/// <param name="parent">�e�̃g�����X�t�H�[��</param>
	Transform(const Vector3& position, const Vector3& rotation = V3::ZERO, const Vector3& scale = V3::ONE, Transform* parent = nullptr);

	~Transform();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �e��ݒ肷��
	/// </summary>
	/// <param name="parent">�e�ɂ���g�����X�t�H�[��</param>
	void SetParent(Transform* parent);

	/// <summary>
	/// �w�肵���O���[�o���ɂȂ�悤�ɁA���[�J����ύX����
	/// </summary>
	/// <param name="global">�O���[�o�����W</param>
	void SetGlobal(const Transform& global);

	//================================================================================
	// ���Q�b�^�[

	// �e�̉e�����܂߂��O���[�o�������擾(�e��X��10�A�q��X��20��������q�̃O���[�o�����W��X30�A�݂�����)
	// �e�����ݒ�Ȃ�Ӗ��Ȃ�
	const Transform Global() const;

	// �e�̃g�����X�t�H�[��
	const Transform Parent() const;

	// ���g�̎����W�A��]�A�g�k��񂩂�s����擾
	// ���f���̕`��̎��Ɏg�p
	const MATRIX Matrix() const;

	// ��]�̍s�񂾂��擾
	// �L�����̌����Ă�������x�N�g���ŗ~�����Ƃ��ȂǂɎg�p
	const MATRIX RotationMatrix() const;

	// ��]���Q�Ƃ��A�E���w���P�ʃx�N�g�����擾
	const Vector3 Right() const;

	// ��]���Q�Ƃ��A���ʂ��w���P�ʃx�N�g�����擾
	const Vector3 Up() const;

	// ��]���Q�Ƃ��A����w���P�ʃx�N�g�����擾
	const Vector3 Forward() const;

	// ��]���e��0�`2�΂̊Ԃɒ����Ď擾
	const Vector3 LimitedRotation() const;

	// �e���ݒ肳��Ă�����True���擾����
	inline const bool IsSetParent() const { return (parent != nullptr); }

	// ���g�̕����i���擾����
	Transform* Copy() const;

	//================================================================================
	// ���v�Z

	const Transform operator+(const Transform& trs) const;
	const Transform operator-(const Transform& trs) const;
	const Transform operator*(const float sca) const;
	const Transform operator/(const float sca) const;
	const Transform operator-() const;
	void operator+=(const Transform& trs);
	void operator-=(const Transform& trs);
	void operator*=(const float sca);

private:
	//================================================================================
	// �������o�ϐ�

	Transform* parent;	// �e�g�����X�t�H�[���̃|�C���^
	std::list<Transform*> children;	// �q�g�����X�t�H�[���̃|�C���^�̃��X�g

	//================================================================================
	// ���e��֐�

	// �q��ǉ�
	inline void AddChild(Transform* child) { children.push_back(child); }

	// �q�����X�g�������
	void RemoveChild(Transform* child);

	/// <summary>
	/// �e�Ƃ̘A�g������
	/// </summary>
	/// <param name="parent">�e�̃|�C���^</param>
	/// <param name="removeFromChildren">�e�̎q�����X�g�������</param>
	void RemoveParent(Transform* parent, bool removeFromChildren = true);
};
