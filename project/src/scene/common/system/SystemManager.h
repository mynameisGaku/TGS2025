#pragma once
#include "../Library/gameObject.h"

class LoadScreen;

/// <summary>
/// �ėp�����̊Ǘ����s��
/// </summary>
class SystemManager : public GameObject {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	SystemManager();
	~SystemManager();

	//================================================================================
	// ���e��֐�

	void Start() override;
	void Update() override;
	void Draw() override;

	/// <summary>
	/// �ŏ��ɍs���`�揈��
	/// </summary>
	void DrawBefore();

	/// <summary>
	/// ���[�h����
	/// </summary>
	void LoadUpdate();

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���[�h���I��������
	/// </summary>
	/// <returns>�I�����Ă����true</returns>
	bool IsLoadEnd() const;

#ifdef _DEBUG

	//================================================================================
	// ���f�o�b�O

	/// <summary>
	/// �f�o�b�O�@�\���s��
	/// </summary>
	void Debug();

#endif // _DEBUG

private:

	/// <summary>
	/// ���[�h���s���ėp����
	/// </summary>
	enum LoadingType {
		ltNone = -1,
		ltFont,
		ltImGuiManager,
		ltInputManager,
		ltSoundManager,
		ltEffectManager,
		ltFader,
		ltCameraManager,
		ltLightManager,
		ltShadowMap,
		ltStage,
		ltStageManager,
		ltUI_Manager,
		ltMax
	};
	LoadingType loadIndex;	// ���[�h���s���Ă���ėp�����̔ԍ�

	LoadScreen* loadScreen;	// ���[�h���
	bool isLoading;	// ���[�h����
};

void DrawBefore();