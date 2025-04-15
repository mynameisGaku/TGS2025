#pragma once
// ���p����
#include "object3D.h"

// ���ėp
#include "../Library/csvReader.h"

// ���ʂŕK�v�ȕ�
#include "cameraDefine.h"

class StateManager;

/// <summary>
/// �J�����̃N���X
/// </summary>
class Camera : public Object3D {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	Camera();
	~Camera();
	
	//================================================================================
	// ���e��֐�

	/// <summary>
	/// �ď�����
	/// </summary>
	void Reset();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �X�e�[�g��ύX���鏈��
	/// </summary>
	/// <param name="id">�X�e�[�g�̔ԍ�</param>
	void ChangeState(const CameraDefine::State& id);

	/// <summary>
	/// �n�`�Ƃ߂荞�܂Ȃ��l�ɂ��鏈�� 
	/// </summary>
	void ColCheckToTerrain();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���΍��W��ݒ肷��
	/// </summary>
	/// <param name="_offset">���΍��W</param>
	inline void SetOffset(const Vector3& _offset) { offset = _offset; }

	/// <summary>
	/// �����_��ݒ肷��
	/// </summary>
	/// <param name="_target">�����_��ݒ肷��</param>
	inline void SetTarget(const Vector3& _target) { target = _target; }

	/// <summary>
	/// �ۗL�҂�ݒ肷��
	/// </summary>
	inline void SetHolderTrs(const Transform* trs) { holder = trs; }

	/// <summary>
	/// �N���X����ɕۗL�҂�ݒ肷��
	/// </summary>
	/// <typeparam name="C">�ۗL���������N���X</typeparam>
	template<class C>
	inline void SetHolder() {

		C* c = FindGameObject<C>();
		if (c == nullptr) {
			SetHolderTrs(nullptr);
			return;
		}

		Object3D* obj = dynamic_cast<Object3D*>(c);
		if (obj == nullptr) {
			SetHolderTrs(nullptr);
			return;
		}

		SetHolderTrs(obj->transform);
	}

	/// <summary>
	/// ���o��ݒ肷��
	/// </summary>
	/// <param name="perfType">���o�̎��</param>
	void SetPerformance(const std::string& perfType);

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �X�e�[�g�p�^�[���̏����擾����
	/// </summary>
	inline StateManager* State() const { return stateManager; }

	/// <summary>
	/// ���΍��W���擾����
	/// </summary>
	inline Vector3 Offset() const { return offset; }

	/// <summary>
	///���΍��W�̃A�h���X���擾���� 
	/// </summary>
	inline Vector3& OffsetPtr() { return offset; }

	/// <summary>
	/// ��]�s������������΍��W���擾����
	/// </summary>
	inline Vector3 OffsetRotAdaptor() const { return offset * transform->Global().RotationMatrix(); } 

	/// <summary>
	/// �����_���擾����
	/// </summary>
	inline Vector3 Target() const { return target; }

	/// <summary>
	/// �����_�̃A�h���X���擾����
	/// </summary>
	inline Vector3& TargetPtr() { return target; }

	/// <summary>
	/// �J�����ۗ̕L�҂̃g�����X�t�H�[�����擾����
	/// </summary>
	/// <returns></returns>
	inline const Transform* HolderTrs() const { return holder; }

	/// <summary>
	/// �J�������[�N�̏����擾����
	/// </summary>
	/// <returns>�J�������[�N����Csv�f�[�^</returns>
	inline CsvReader* CameraWorkData() const { return cameraWork; }

	/// <summary>
	/// �J�����̈ʒu(��΍��W)
	/// </summary>
	Vector3 WorldPos() const;

	/// <summary>
	/// �����_�֌������x�N�g��
	/// </summary>
	Vector3 TargetLay() const;

private:
	//================================================================================
	// �������o�ϐ�

	StateManager* stateManager;	// �X�e�[�g���Ǘ�����

	Vector3 offset;	// �J�����̑��΍��W
	Vector3 target;	// �J�����̒����_

	const Transform* holder;	// �J�����ۗ̕L��
	CsvReader* cameraWork;		// �J�������o���
};
