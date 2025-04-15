#pragma once
// ���p����
#include "object3D.h"

// ���ėp
#include <string>
#include "config.h"

// ���ʂŕK�v�ȕ�
#include "lightDefine.h"

using namespace LightDefine;

/// <summary>
/// ���C�g�̃N���X
/// </summary>
class Light : public Object3D {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	Light(const LightInfo& info);
	virtual ~Light();

	//==========================================================================================
	// ���e��֐�

	virtual void Update() override;

	/// <summary>
	/// ���C�g�Ɋւ���X�V�������s��
	/// </summary>
	void LightUpdate();

	/// <summary>
	/// ���C�g�̃n���h���𐶐�����
	/// </summary>
	/// <param name="info">���C�g�̏��</param>
	void CreateLightHandle(const LightInfo& info);

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ��]�p�x��ݒ肷��(�ʓx�@)
	/// </summary>
	/// <param name="deg">��]�p�x(�ʓx�@)</param>
	inline void SetDegRotation(const Vector3& deg) { m_rotation = deg; }

	//==========================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// ���C�g�̏����擾����
	/// </summary>
	inline const LightInfo Info() const { return info; }

private:

#ifdef IMGUI

	std::string imguiLabel;	// ImGui�ɓo�^���閼�O
	void initImGuiNode();			// ImGui�̏���������
	void initImGuiNode_LightDir();	// ImGui�̏���������(���s����)
	void initImGuiNode_LightPoint();// ImGui�̏���������(�_����)
	void initImGuiNode_LightStop();	// ImGui�̏���������(�Ɩ�����)

#endif // IMGUI

protected:
	LightInfo info;	// ���C�g�̏��
	Vector3 m_rotation;	// ���C�g�̉�]�p�x(�x���@)
	float m_outAngle;	// �X�|�b�g���C�g�̃R�[���̊O���̊p�x(�x���@)
	float m_inAngle;	// �X�|�b�g���C�g�̃R�[���̓����̊p�x(�x���@)

};