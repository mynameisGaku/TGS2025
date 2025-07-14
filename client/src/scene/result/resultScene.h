#pragma once
#include "framework/SceneBase.h"
#include <list>

/// <summary>
/// ���U���g�V�[��
/// </summary>
class ResultScene : public SceneBase {
public:
	//==================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	ResultScene(const std::string& name);
	~ResultScene();

	//==================================================
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
	/// �v���C�O�̍X�V����
	/// </summary>
	void BeforePlayUpdate() override;

	/// <summary>
	/// �v���C���̍X�V����
	/// </summary>
	void InPlayUpdate() override;

	/// <summary>
	/// �v���C��̍X�V����
	/// </summary>
	void AfterPlayUpdate() override;
};