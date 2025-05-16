#pragma once
// ���p����
#include "object3D.h"

// ���ėp
#include "../Library/csvReader.h"
#include "Source/TinyFSM.h"

// ���ʂŕK�v�ȕ�
#include "cameraDefine.h"

class StateManager;

namespace
{
	static const float MOVE_SPEED = 12.0f;	// ���ړ����x
	static const float SHIFT_SPEED = 6.0f;	// �c�ړ����x
	static const float ROT_SPEED = Math::DegToRad(3.0f);	// ��]���x
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

	void Reset();
	void Update() override;
	void Draw() override;

	/// <summary>
	/// <para>�X�e�[�g��ύX���鏈��</para>
	/// <para>�����̐擪��"��"������̂�Y��Ȃ��ł��������B</para>
	/// </summary>
	/// <param name="state">�X�e�[�g�̊֐��|�C���^</param>
	void ChangeState(void(Camera::*state)(FSMSignal));

	// �n�`�Ƃ߂荞�܂Ȃ��l�ɂ��鏈�� 
	void ColCheckToTerrain();

	//================================================================================
	// ���Z�b�^�[

	// ���΍��W��ݒ肷��
	inline void SetOffset(const Vector3& _offset) { offset = _offset; }

	// �ۗL�҂�ݒ肷��
	inline void SetHolderTrs(const Transform* trs) { holder = trs; }

	// �Ǐ]����L�����N�^�[�̔ԍ���ݒ肷��
	inline void SetHolderCharaIndex(int index) { m_CharaIndex = index; }

	//================================================================================
	// ���Q�b�^�[

	// �X�e�[�g�p�^�[���̏����擾����
	inline TinyFSM<Camera>* State() const { return fsm; }

	// ���΍��W���擾����
	inline Vector3 Offset() const { return offset; }

	// ��]�s������������΍��W���擾����
	inline Vector3 OffsetRotAdaptor() const { return offset * transform->Global().RotationMatrix(); } 

	// �J�����ۗ̕L�҂̃g�����X�t�H�[�����擾����
	inline const Transform* HolderTrs() const { return holder; }

	// �J�����̈ʒu(��΍��W)
	Vector3 WorldPos() const;

	// �����_�֌������x�N�g��
	Vector3 TargetLay() const;

	//================================================================================
	// ���X�e�[�g

	void DebugState(FSMSignal sig);
	void ChaseState(FSMSignal sig);
	void AimState(FSMSignal sig);

public:
	Vector3 offset;		// �J�����̑��΍��W
private:

	TinyFSM<Camera>* fsm;	// �X�e�[�g�}�l�[�W���[

	Vector3 offsetPrev;	// ��O�̃J�����̑��΍��W

	const Transform* holder;	// �J�����ۗ̕L��
	int m_CharaIndex;			// �L�����N�^�[�̔ԍ�

	// �ړ�����
	void moveProcess();

	/// <summary>
	/// �}�E�X�ɂ��J��������
	/// </summary>
	/// <param name="type">�y0�z�̏ꍇ�AX���̂݁B�y1�z�̏ꍇ�AY���̂݁B�y���̑��zXY���B
	/// </param>
	void operationByMouse(int type = -1);

	/// <summary>
	/// �X�e�B�b�N�ɂ��J��������
	/// </summary>
	/// <param name="type">�y0�z�̏ꍇ�AX���̂݁B�y1�z�̏ꍇ�AY���̂݁B�y���̑��zXY���B
	/// </param>
	void operationByStick(int type = -1);
};
