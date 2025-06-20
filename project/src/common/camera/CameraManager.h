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

	/// <summary>
	/// �J�����̐������s��
	/// </summary>
	/// <returns>���������J�����̎���</returns>
	Camera* CreateCamera();

	/// <summary>
	/// �J�����̍ď��������s��
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	void ResetCamera(int number);

	/// <summary>
	/// �J�����̔ԍ������������m�F����B
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	/// <param name="number"></param>
	/// <returns>�ԍ�����v�����J���������݂���ꍇtrue</returns>
	bool CheckNumber(int number);

	/// <summary>
	/// �J�����̃X�e�[�g��ύX����
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	/// <param name="state">�ύX����X�e�[�g</param>
	void ChangeStateCamera(int number, void(Camera::* state)(FSMSignal));

	/// <summary>
	/// �J�����`����w�肳�ꂽ�ʒu�ƃT�C�Y�ŕ������܂�
	/// </summary>
	/// <param name="x">�����̈�̍������X���W</param>
	/// <param name="y">�����̈�̍������Y���W</param>
	/// <param name="w">�����̈�̕�</param>
	/// <param name="h">�����̈�̍���</param>
	void CameraScreenDivision(int x, int y, int w, int h);

	/// <summary>
	/// �J�����`����w�肳�ꂽ�ʒu�ƃT�C�Y�ŕ������Ă���A�`�悵�܂�
	/// </summary>
	/// <param name="x">�����̈�̍������X���W</param>
	/// <param name="y">�����̈�̍������Y���W</param>
	/// <param name="w">�����̈�̕�</param>
	/// <param name="h">�����̈�̍���</param>
	/// <param name="number">�`����s���J�����̔ԍ�</param>
	void CameraScreenDivisionDraw(int x, int y, int w, int h, int number);

	/// <summary>
	/// ��ʕ�����K�p����
	/// </summary>
	void ApplyScreenDivision();

	/// <summary>
	/// ����̉�ʃT�C�Y�ɖ߂�
	/// </summary>
	void DefaultScreenSize();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �J�������[�N��ݒ肷��
	/// </summary>
	/// <param name="number">�J�����̔ԍ�</param>
	/// <param name="type">�J�������[�N�̎��</param>
	void SetCameraWork(int number, const std::string& type);

	/// <summary>
	/// ��ʕ����������s������ݒ肷��
	/// </summary>
	void SetIsScreenDivision(bool value);

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
	Camera* GetCamera(int number);

	/// <summary>
	/// �S�ẴJ�����̃��X�g���擾����
	/// </summary>
	std::vector<Camera*> AllCameras();

	/// <summary>
	/// ��ʂ���������Ă��邩�擾����
	/// </summary>
	bool IsScreenDivision();

	/// <summary>
	/// ��ʕ����̊J�n�n�_���擾����
	/// </summary>
	Vector2 GetScreenDivisionPos();

	/// <summary>
	/// ��ʕ����̑傫�����擾����
	/// </summary>
	Vector2 GetScreenDivisionSize();

	/// <summary>
	/// ��ʕ����̊J�n�n�_���擾����
	/// </summary>
	Vector2 GetScreenDivisionPos_CameraIndex(int index);

	/// <summary>
	/// ��ʕ����̒��S���W���擾����
	/// </summary>
	Vector2 GetScreenDivisionCenter();

	Vector2 GetDivedByCameraNum();

	//================================================================================
	// ���f�o�b�O�@�\

	/// <summary>
	/// �X�e�[�g��ύX����(string�^)
	/// </summary>
	/// <param name="state">�ύX����X�e�[�g��</param>
	//void CameraChangeStateTheString(const std::string& state);

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