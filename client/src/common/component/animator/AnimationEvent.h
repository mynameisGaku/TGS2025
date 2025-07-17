#pragma once

// ���p����
#include "framework/gameObject.h"

class Object3D;
class Animator;

/// <summary>
/// �A�j���[�V�����Ŏg�p����R�}���h���Ǘ�����
/// </summary>
class AnimationEvent : public GameObject {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	AnimationEvent(Animator* animator);
	~AnimationEvent();

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
	/// �R�}���h���s����
	/// </summary>
	void CommandProcess();
	
	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �R�}���h��ݒ肷��
	/// </summary>
	/// <param name="commandType">�R�}���h�̎��</param>
	/// <param name="commandContents">�R�}���h�̎��s���e</param>
	/// <param name="executeAnimFrame">�R�}���h�����s����A�j���[�V�����t���[��</param>
	void SetCommand(std::string commandType, std::string commandContents, float executeAnimFrame);

	inline void SetCommandType(std::string type) { commandType = type; }
	inline void SetCommandContents(std::string contents) { commandContents = contents; }
	inline void SetExecuteAnimFrame(float frame) { executeAnimFrame = frame; }
	inline void SetUseCommand(bool value) { canUseCommande = value; }

	//==========================================================================================
	// ���Q�b�^�[

	inline std::string CommandType() const { return commandType; }
	inline std::string CommandContents() const { return commandContents; }
	inline float ExecuteAnimFrame() const { return executeAnimFrame; }
	inline bool CanUseCommand() const { return canUseCommande; }

private:
	Object3D* parent;			// �e�I�u�W�F�N�g
	Animator* animator;			// �A�j���[�V�����Đ��@�\
	std::string commandType;	// �R�}���h�̎��
	std::string commandContents;// �R�}���h�̎��s���e
	float executeAnimFrame;		// �R�}���h�����s����A�j���[�V�����t���[��
	bool canUseCommande;		// �R�}���h�̎��s����
};