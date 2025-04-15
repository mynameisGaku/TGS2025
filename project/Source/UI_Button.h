#pragma once
// ���p����
#include "UI_Canvas.h"

// �����o�E�@�\
#include "KeyDefine.h"
#include <functional>

/// <summary>
/// �{�^���\���ƃ{�^�������������s���N���X
/// </summary>
/// <author> �~�b�` </author>
class UI_Button : public UI_Canvas {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// ���z�{�^���A���������ŏ���������
	/// </summary>
	/// <param name="image">�{�^���Ɏg�p����摜�n���h��</param>
	/// <param name="trs">���W�E��]�E�g�k�̏��</param>
	/// <param name="input">���z�{�^��</param>
	/// <param name="timing">�{�^�������������Ăԃ^�C�~���O</param>
	/// <param name="func">�{�^����������</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	UI_Button(const int& image, const RectTransform& trs, const std::string& inputName, const std::vector<KeyDefine::TouchPhase>& timing, const std::function<void()>& func, const int& padNumber = DX_INPUT_PAD1);
	
	/// <summary>
	/// ���͂̎�ށA���������ŏ���������
	/// </summary>
	/// <param name="imagePath">�{�^���Ɏg�p����摜�̃p�X</param>
	/// <param name="trs">�`����W</param>
	/// <param name="key">���͂̎��</param>
	/// <param name="timing">�{�^�������������Ăԃ^�C�~���O</param>
	/// <param name="func">�{�^����������</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	UI_Button(const int& image, const RectTransform& trs, const std::vector<KeyDefine::KeyCode>& keyCode, const std::vector<KeyDefine::TouchPhase>& timing, const std::function<void()>& func, const int& padNumber = DX_INPUT_PAD1);

	/// <summary>
	/// UI_Button�𐶐�����
	/// </summary>
	/// <param name="imagePath">�{�^���Ɏg�p����摜�̃p�X</param>
	/// <param name="trs">�`����W</param>
	UI_Button(const int& image, const RectTransform& trs);
	
	~UI_Button();

	//==========================================================================================
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
	/// ��������Ɖ�������
	/// </summary>
	/// <param name="phase">�����^�C�~���O</param>
	/// <returns>���������s�����ꍇ�Atrue��Ԃ�</returns>
	bool PushProcess(const KeyDefine::TouchPhase& phase);

	/// <summary>
	/// �}�E�X�ɂ�鉟������Ɖ�������
	/// </summary>
	/// <param name="phase">�����^�C�~���O</param>
	/// <returns>���������s�����ꍇ�Atrue��Ԃ�</returns>
	bool PushProcessMouse(const KeyDefine::TouchPhase& phase);

	/// <summary>
	/// �����ꂽ�ۂɉ摜���Â��`�悷��
	/// </summary>
	void DrawDarken();

	/// <summary>
	/// ���͎�ނ��L�[�{�[�h�E�p�b�h�ƃ}�E�X�ɕ��ނ���
	/// </summary>
	void SeparateInputTypes();

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �}�E�X�̃N���b�N�ɔ������邩��ݒ肷��
	/// </summary>
	void SetIsMouseClick(const bool& value);

	/// <summary>
	/// �{�^�����������ۂɉ摜���Â��`�悷�邩�ݒ肷��
	/// </summary>
	inline void SetIsPushedDarkness(const bool& value) { isPushedDarkenes = value; }

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �{�^���ƃ}�E�X�Ƃ̓����蔻��
	/// </summary>
	bool ColCheck_Mouse();

private:
	/// <summary>
	/// UI_Button�𐶐�����
	/// </summary>
	/// <param name="image">�{�^���Ɏg�p����摜�n���h��</param>
	/// <param name="trs">�`����W</param>
	/// <param name="input">���z�{�^��</param>
	/// <param name="key">���͂̎��</param>
	/// <param name="timing">�{�^�������������Ăԃ^�C�~���O</param>
	/// <param name="func">�{�^����������</param>
	/// <param name="padNumber">�p�b�h�̔ԍ�</param>
	UI_Button(const int& image, const RectTransform& trs, const std::string& inputName, const std::vector<KeyDefine::KeyCode>& keyCode, const std::vector<KeyDefine::TouchPhase>& timing, const std::function<void()>& func, const int& padNumber = DX_INPUT_PAD1);

	int padNumber;			// �p�b�h�̔ԍ�
	bool isPushed;			// ������Ă��邩
	bool isPushedDarkenes;	// �����ꂽ�ۂɉ摜���Â����邩
	bool isMouseClick;		// �}�E�X�̃N���b�N�ɔ������邩

	std::function<void()> action;				// �{�^����������
	std::vector<KeyDefine::TouchPhase> timing;	// �����������Ăԃ^�C�~���O

	std::string inputName;		// ���z�{�^���̖��O
	std::vector<KeyDefine::KeyCode> allKeyCode;		// �S�Ă̓��͎��
	std::vector<KeyDefine::KeyCode> keyCodes;		// �L�[�{�[�h�E�p�b�h�̓��͎��
	std::vector<KeyDefine::KeyCode> mouseKeyCodes;	// �}�E�X�̓��͎��
};
