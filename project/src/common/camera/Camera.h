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
#include "src/common/component/shake/Shake.h"
#include "src/util/math/Vector2.h"
#include "src/common/network/user/User.h"

class StateManager;
class Chara;
class NetworkManager;

/// <summary>
/// �J�����̃N���X
/// </summary>
class Camera : public Object3D {
public:

	/// <summary>
	/// �J�����̎��_�ړ����@
	/// </summary>
	enum class ViewPointShift {
		None = -1,	// ��
		Horizontal,	// �������̂�
		Vertical,	// �������̂�
		All			// �������E���������܂�
	};

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
	/// �`��̈��K�p����
	/// </summary>
	void ApplyDrawArea() const;

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���΍��W��ݒ肷��
	/// </summary>
	/// <param name="_offset">���΍��W</param>
	inline void SetOffset(const Vector3& _offset) { m_Offset = _offset; }

	/// <summary>
	/// �����_��ݒ肷��
	/// </summary>
	/// <param name="_target">�����_��ݒ肷��</param>
	inline void SetTarget(const Vector3& _target) { m_Target = _target; }

	/// <summary>
	/// �ۗL�҂�ݒ肷��
	/// </summary>
	inline void SetHolderTrs(const Transform* trs) { m_pHolder = trs; }

	/// <summary>
	/// �ۗL��(���[�U�[)��ݒ肷��
	/// </summary>
	/// <param name="user"></param>
	inline void SetUser(const User& user) { m_User = user; }

	/// <summary>
	/// �`����s�����ǂ�����ݒ肷��
	/// </summary>
	inline void SetIsView(bool view) { m_IsView = view; }

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

	/// <summary>
	/// �A�j���[�V������ݒ肷��B�J�n�n�_����I���n�_�ֈړ����āA�I���n�_����J�n�n�_�֖߂铮����A�w�肳�ꂽ�b���ōs��
	/// </summary>
	void SetAnimation(const CameraDefine::CameraAnimData& animData);

	inline void SetFollowerChara(const Chara* chara) { m_pFollowerChara = chara; }

	/// <summary>
	/// �`��͈͂�ݒ肷��
	/// </summary>
	/// <param name="x">�`��J�n�n�_(X��)</param>
	/// <param name="y">�`��J�n�n�_(Y��)</param>
	/// <param name="w">�`��̑傫��(X��)</param>
	/// <param name="h">�`��̑傫��(Y��)</param>
	void SetDrawArea(int x, int y, int w, int h);

	/// <summary>
	/// ����̕`��͈͂�ݒ肷��
	/// </summary>
	void SetDrawAreaDefault();

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �X�e�[�g�p�^�[���̏����擾����
	/// </summary>
	inline TinyFSM<Camera>* State() const { return m_Fsm; }

	/// <summary>
	/// ���΍��W���擾����
	/// </summary>
	inline const Vector3 Offset() const { return m_Offset; }

	/// <summary>
	/// ��]�s������������΍��W���擾����
	/// </summary>
	inline const Vector3 OffsetRotAdaptor() const { return m_Offset * transform->Global().RotationMatrix(); }

	/// <summary>
	/// �����_���擾����
	/// </summary>
	inline const Vector3 Target() const { return m_Target; }

	/// <summary>
	/// �J�����ۗ̕L�҂̃g�����X�t�H�[�����擾����
	/// </summary>
	/// <returns></returns>
	inline const Transform* HolderTrs() const { return m_pHolder; }

	/// <summary>
	/// �J�������[�N�̏����擾����
	/// </summary>
	/// <returns>�J�������[�N����Csv�f�[�^</returns>
	inline const std::vector<CameraDefine::PerformanceData> GetPerformanceDatas() const { return m_PerformanceDatas; }

	/// <summary>
	/// �J�����̈ʒu(��΍��W)
	/// </summary>
	const Vector3 WorldPos() const;

	/// <summary>
	/// �����_�֌������x�N�g��
	/// </summary>
	const Vector3 TargetLay() const;

	/// <summary>
	/// �������Ă���L�����N�^�[
	/// </summary>
	inline const Chara* TargetChara() const { return m_pTargetChara; }

	/// <summary>
	/// �`�悪�I���������ǂ����𔻒�
	/// </summary>
	inline bool IsDrawEnd() const { return m_DrawFlag; }

	/// <summary>
	/// �`����s����
	/// </summary>
	inline bool IsView() const { return m_IsView; }

	/// <summary>
	/// �J�����̐��ʂɂ��邩�ǂ�����Ԃ�
	/// </summary>
	bool IsFrontView(const Vector3& pos) const;

	/// <summary>
	/// �J�����̉E���ɂ��邩�ǂ�����Ԃ�
	/// </summary>
	bool IsRightView(const Vector3& pos) const;

	/// <summary>
	/// ���̃J���������L���Ă��郆�[�U�[���擾����
	/// </summary>
	User* GetUser();

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

	/// <summary>
	/// ���o�X�e�[�g
	/// </summary>
	void PerformanceState(FSMSignal sig);

private:

	/// <summary>
	/// �J�����`�揈��
	/// </summary>
	void rendering();

	/// <summary>
	/// �n�`�Ƃ߂荞�܂Ȃ��l�ɂ��鏈�� 
	/// </summary>
	Vector3 colCheckToTerrain(const Vector3& begin, const Vector3& end, Vector3* hitPos);
	Vector3 collCheckCapsule_Hitpos(const Vector3& begin, const Vector3& end, Vector3* hitPos);

	/// <summary>
	/// �ړ�����
	/// </summary>
	void moveProcess();

	/// <summary>
	/// �}�E�X�ɂ��J��������
	/// </summary>
	/// <param name="type">�J�����̎��_�ړ����@</param>
	void operationByMouse(ViewPointShift type);

	/// <summary>
	/// �X�e�B�b�N�ɂ��J��������
	/// </summary>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	/// <param name="type">�J�����̎��_�ړ����@</param>
	void operationByStick(int padNumber, ViewPointShift type);

	/// <summary>
	/// ���o�̍X�V����
	/// </summary>
	void updateAnimation();

	/// <summary>
	/// ���z�J�����̕`��ʒu���J�v�Z���ŕ`��
	/// </summary>
	void drawVirtualCamera();

	//================================================================================
	// �������o�ϐ�

	TinyFSM<Camera>* m_Fsm;

	Vector3 m_PositionPrev;	// ��ԑO�̃J�����̍��W
	Vector3 m_RotationPrev;	// ��ԑO�̃J�����̉�]

	Vector3 m_Offset;		// �J�����̑��΍��W
	Vector3 m_OffsetPrev;	// ��ԑO�̃J�����̑��΍��W
	
	Vector3 m_Target;		// �J�����̒����_
	Vector3 m_TargetPrev;	// ��ԑO�̃J�����̒����_

	MATRIX m_CameraRotMat;	// �J�����̉�]�s��

	ColDefine::Cone m_CameraCone;	// �J�����̎���p���

	CameraDefine::CameraAnimData m_AnimData;	// �J�����A�j���[�V�����̃f�[�^

	Shake* m_pShake;	// �V�F�C�N�R���|�[�l���g

	std::vector<CameraDefine::PerformanceData> m_PerformanceDatas;	// ���o�̃f�[�^
	const Transform* m_pHolder;	// �J�����ۗ̕L��
	int m_CharaIndex;			// �L�����N�^�[�̔ԍ�
	User m_User;				// ���̃J���������L���Ă��郆�[�U�[

	float m_EasingTime;				// �C�[�W���O�p�^�C�}�[
	float m_TargetTransitionTime;	// �������Ă���L�����Ɉ����t���܂ł̎���

	int screenPosX, screenPosY;		// �`��͈�(�n�_)
	int screenSizeX, screenSizeY;	// �`��͈�(�傫��)

	bool m_IsView;		// �`�悵�Ă��邩
	bool m_DrawFlag;	// �`�悪�������Ă��邩

	const Chara* m_pFollowerChara;	// �ǔ����Ă���L����
    const Chara* m_pTargetChara;	// �������Ă���L����

    NetworkManager* m_pNetworkManager; // ���݂̃N���C�A���g��UUID�����炤���߂̃l�g�}�l
};
