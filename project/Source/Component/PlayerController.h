#pragma once

// ���p����
#include "Component.h"

#include <string>
#include "../KeyDefine.h"
#include "../Util/Vector3.h"

class CharaBase;

class PlayerController : public Component {
public:
	//=======================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	PlayerController();
	~PlayerController();

	//=======================================================================================
	// ���e��֐�

	void Init(int padNumber, float moveSpeed, float rotSpeed);
	void Update() override;

	//=======================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���삷��p�b�h�̔ԍ���ݒ肷��
	/// </summary>
	/// <param name="_padNumber">�p�b�h�̔ԍ�(DX_INPUT_PAD�`)</param>
	inline void SetPadNumber(int _padNumber) { padNumber = _padNumber; }

	/// <summary>
	/// �ړ����x��ݒ肷��
	/// </summary>
	/// <param name="_moveSpeed">�ړ����x</param>
	inline void SetMoveSpeed(float _moveSpeed) { moveSpeed = _moveSpeed; }

	/// <summary>
	/// ��]���x��ݒ肷��
	/// </summary>
	/// <param name="_rotSpeed">��]���x</param>
	inline void SetRotSpeed(float _rotSpeed) { rotSpeed = _rotSpeed; }

	//=======================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���삵�Ă���p�b�h�ԍ����擾����
	/// </summary>
	inline const int PadNumber() const { return padNumber; }

	/// <summary>
	/// ���삵�Ă���p�b�h�̃X�e�B�b�N�̌X�����擾����
	/// </summary>
	Vector3 AnalogStick();

	/// <summary>
	/// �{�^����������Ă��邩���擾����
	/// </summary>
	/// <param name="keyCode">���͎��</param>
	/// <param name="toushPhase">���͏�</param>
	bool IsPressButton(KeyDefine::KeyCode keyCode, KeyDefine::TouchPhase toushPhase);

	/// <summary>
	/// �{�^����������Ă��邩���擾����
	/// </summary>
	/// <param name="input">���z�{�^����</param>
	/// <param name="toushPhase">���͏�</param>
	bool IsPressButton(const std::string& input, KeyDefine::TouchPhase toushPhase);

	/// <summary>
	/// ����\��
	/// </summary>
	bool CanMove() const;

	/// <summary>
	/// �ړ����͂��s���Ă��邩
	/// </summary>
	bool IsMoveButton() const;

private:
	CharaBase* chara;	// �ۗL��(CharaBase)

	int padNumber;		// ���삵�Ă�p�b�h�̔ԍ�
	float moveSpeed;	// �ړ����x
	float rotSpeed;		// ��]���x
};