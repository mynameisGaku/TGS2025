#pragma once
// ���p����
#include "../Library/sceneBase.h"

// ���ėp
#include <list>

// ��UI�֘A
class UI_Canvas;

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene : public SceneBase {
public:
	//==================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	TitleScene(std::string name);
	~TitleScene();

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