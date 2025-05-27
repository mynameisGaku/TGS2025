#pragma once
// ���p����
#include "src/util/object3D/Object3D.h"

// ���ėp
#include "src/util/file/csv/csvReader.h"
#include "src/util/fsm/TinyFSM.h"
#include "src/util/math/MathUtil.h"

// ���ʂŕK�v�ȕ�
#include "src/common/camera/CameraDefine.h"
#include "src/common/component/collider/CollisionDefine.h"

class StateManager;
class CharaBase;

namespace
{
	static const float MOVE_SPEED = 12.0f;	// ���ړ����x
	static const float SHIFT_SPEED = 6.0f;	// �c�ړ����x
	static const float ROT_SPEED = MathUtil::ToRadians(3.0f);	// ��]���x
}

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
	/// <para>�X�e�[�g��ύX���鏈��</para>
	/// <para>�����̐擪��"��"������̂�Y��Ȃ��ł��������B</para>
	/// </summary>
	/// <param name="state">�X�e�[�g�̊֐��|�C���^</param>
	void ChangeState(void(Camera::*state)(FSMSignal));

	/// <summary>
	/// �n�`�Ƃ߂荞�܂Ȃ��l�ɂ��鏈�� 
	/// </summary>
	void ColCheckToTerrain();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void MoveProcess();

	/// <summary>
	/// �}�E�X�ɂ��J��������
	/// </summary>
	/// <param name="type">�y0�z�̏ꍇ�AX���̂݁B�y1�z�̏ꍇ�AY���̂݁B�y���̑��zXY���B
	/// </param>
	void OperationByMouse(int type = -1);

	/// <summary>
	/// �X�e�B�b�N�ɂ��J��������
	/// </summary>
	/// <param name="type">�y0�z�̏ꍇ�AX���̂݁B�y1�z�̏ꍇ�AY���̂݁B�y���̑��zXY���B
	/// </param>
	void OperationByStick(int type = -1);

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
	/// �Ǐ]����L�����N�^�[�̔ԍ���ݒ肷��
	/// </summary>
	/// <param name="index">�L�����N�^�[�̔ԍ�</param>
	inline void SetHolderCharaIndex(int index) { m_CharaIndex = index; }

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
	inline TinyFSM<Camera>* State() const { return fsm; }

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

	CharaBase* TargetChara() const { return targetChara; }

	//================================================================================
	// ���X�e�[�g

	/// <summary>
	/// �f�o�b�O�X�e�[�g
	/// </summary>
	void DebugState(FSMSignal sig);

	/// <summary>
	/// �Ǐ]�X�e�[�g
	/// </summary>
	void ChaseState(FSMSignal sig);

	/// <summary>
	/// �_���X�e�[�g
	/// </summary>
	void AimState(FSMSignal sig);

private:
	//================================================================================
	// �������o�ϐ�

	TinyFSM<Camera>* fsm;

	Vector3 offset;		// �J�����̑��΍��W
	Vector3 offsetPrev;	// ��O�̃J�����̑��΍��W
	Vector3 target;		// �J�����̒����_
	Vector3 targetPrev;	// ��O�̃J�����̒����_
	ColDefine::Cone cameraCone;

	const Transform* holder;	// �J�����ۗ̕L��
	CsvReader* cameraWork;		// �J�������o���
	int m_CharaIndex;			// �L�����N�^�[�̔ԍ�

	CharaBase* targetChara;
};
