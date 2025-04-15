#pragma onces

// ���p����
#include "StateBase.h"

// ���ėp
#include "Util/Vector3.h"

/// <summary>
/// �J�����̃f�o�b�O���̃X�e�[�g
/// </summary>
class CameraState_Debug : public StateBase {
public:
	//=====================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	CameraState_Debug(StateManager* p);
	~CameraState_Debug();

	//=====================================================
	// ���e��֐�

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ���g�̃X�e�[�g�����삷�鎞�ɌĂ�
	/// </summary>
	void Resume() override;

	/// <summary>
	/// ���g�̃X�e�[�g����~���鎞�ɌĂ�
	/// </summary>
	void Suspend() override;

	/// <summary>
	/// �ړ�����
	/// </summary>
	void MoveProcess();

private:
	bool canMove;		// �ړ��\��
};