#pragma once
#include "src/common/camera/Camera.h"

using namespace CameraDefine;

/// <summary>
/// �J�����̊Ǘ����s��
/// </summary>
namespace CameraManager {

	//================================================================================
	// ���e��֐�

	void Init();
	void Update();
	void Draw();
	void Release();

	void CreateCamera(bool view);

	/// <summary>
	/// �J�����̍ď��������s��
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	void ResetCamera(const int& number);

	/// <summary>
	/// �J�����̔ԍ������������m�F����B
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	/// <param name="number"></param>
	/// <returns>�ԍ�����v�����J���������݂���ꍇtrue</returns>
	bool CheckNumber(const int& number);

	/// <summary>
	/// �J�����̃X�e�[�g��ύX����
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	/// <param name="state">�ύX����X�e�[�g</param>
	void ChangeStateCamera(const int& number, void(Camera::* state)(FSMSignal));

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �J�������[�N��ݒ肷��
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	/// <param name="type">�J�������[�N�̎��</param>
	void SetCameraWork(const int& number, const std::string& type);

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���C���J�������擾����
	/// </summary>
	/// <returns></returns>
	Camera* MainCamera();

	/// <summary>
	/// �J�����̏����擾����
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	/// <returns>�J�����̃|�C���^�[</returns>
	Camera* GetCamera(const int& number);

	/// <summary>
	/// �S�ẴJ�����̃��X�g���擾����
	/// </summary>
	std::vector<Camera*> AllCameras();

	//================================================================================
	// ���f�o�b�O�@�\

	/// <summary>
	/// �X�e�[�g��ύX����(string�^)
	/// </summary>
	/// <param name="state">�ύX����X�e�[�g��</param>
	void CameraChangeStateTheString(const std::string& state);


#ifdef _DEBUG
#ifndef IMGUI
#define IMGUI
#endif
#endif
#ifdef IMGUI

	/// <summary>
	/// �f�o�b�O�Ŏg�p����ImGui�֘A�̋@�\������������
	/// </summary>
	void InitImGuiNode();

	/// <summary>
	/// �f�o�b�O�Ŏg�p����ImGui�֘A�̋@�\���X�V����
	/// </summary>
	void UpdateImGuiNode();

#endif // IMGUI

}