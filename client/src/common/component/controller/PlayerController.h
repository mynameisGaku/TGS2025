#pragma once

// ���p����
#include "src/common/component/Component.h"

#include <string>
#include "src/util/input/KeyDefine.h"
#include "src/util/math/vector3.h"

class Chara;

class PlayerController : public Component {
public:
	//=======================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	PlayerController();
	~PlayerController();

	//=======================================================================================
	// ���e��֐�

	void Init(int padNumber);
	void Update() override;

	//=======================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���삷��p�b�h�̔ԍ���ݒ肷��
	/// </summary>
	/// <param name="_padNumber">�p�b�h�̔ԍ�(DX_INPUT_PAD�`)</param>
	inline void SetPadNumber(int _padNumber) { padNumber = _padNumber; }

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
	Chara* chara;	// �ۗL��(CharaBase)

	int padNumber;		// ���삵�Ă�p�b�h�̔ԍ�
};