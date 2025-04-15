#pragma once
// ���p����
#include "Object3D.h"

// ���ėp
#include "../Source/Util/Transform.h"

// ���ʂŕK�v�ȕ�
#include "effectDefine.h"

/// <summary>
/// �G�t�F�N�g���Đ�����N���X
/// </summary>
class EffectBase  : public Object3D {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	EffectBase();
	~EffectBase();

	//================================================================================
	// ���e��֐�

	void Update() override;

	/// <summary>
	/// �G�t�F�N�g���Đ�����(2D)
	/// </summary>
	/// <param name="_id">�G�t�F�N�g�̎��</param>
	/// <param name="_handle">�G�t�F�N�g�̃��\�[�X</param>
	/// <param name="_transform">���W�E��]�E�g�k�̏��</param>
	/// <param name="_distinction">���ʃ��x��</param>
	/// <param name="_isLoop">���[�v�����邩</param>
	
	/// <summary>
	/// 2D�G�t�F�N�g���Đ�����
	/// </summary>
	/// <param name="info">�G�t�F�N�g�̏��</param>
	/// <param name="transform">���W�E��]�E�g�k�̏��</param>
	/// <param name="label">���ʃ��x��</param>
	/// <param name="isLoop">���[�v�Đ����s����</param>
	void Play2D(const EffectDefine::EffectInfo& info, const Transform& transform, const std::string& label = "", const bool& isLoop = false);

	/// <summary>
	/// 3D�G�t�F�N�g���Đ�����
	/// </summary>
	/// <param name="info">�G�t�F�N�g�̏��</param>
	/// <param name="transform">���W�E��]�E�g�k�̏��</param>
	/// <param name="label">���ʃ��x��</param>
	/// <param name="isLoop">���[�v�Đ����s����</param>
	void Play3D(const EffectDefine::EffectInfo& info, const Transform& transform, const std::string& label = "", const bool& isLoop = false);

	/// <summary>
	/// �G�t�F�N�g�̍Đ����~�߂�
	/// </summary>
	void Stop();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���W�E��]�E�g�k��ݒ肷��
	/// </summary>
	/// <param name="transform">���W�E��]�E�g�k�̏��</param>
	void SetTransform(const Transform& transform);

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	/// <param name="pos">���W�̏��</param>
	void SetPosition(const Vector3& pos);

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="rot">��]�̏��</param>
	void SetRotation(const Vector3& rot);

	/// <summary>
	/// �g�k��ݒ肷��
	/// </summary>
	/// <param name="scale">�g�k�̏��</param>
	void SetScale(const Vector3& scale);

	/// <summary>
	/// �Đ����x��ݒ肷��
	/// </summary>
	/// <param name="speed">�Đ����x</param>
	void SetPlaySpeed(const float& speed);

	/// <summary>
	/// �F�ƕs�����x��ݒ肷��
	/// </summary>
	/// <param name="r">��</param>
	/// <param name="g">��</param>
	/// <param name="b">��</param>
	/// <param name="a">�s�����x</param>
	void SetRGBA(const int& r, const int& g, const int& b, const int& a);

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �G�t�F�N�g���Đ�����Ă��邩���擾����
	/// </summary>
	bool IsPlaying() const;

	/// <summary>
	/// ���g�������Ă���G�t�F�N�g�̏����擾����
	/// </summary>
	inline EffectDefine::EffectInfo Info() const { return info; }

	/// <summary>
	/// ���ʃ��x�����擾����
	/// </summary>
	inline std::string Label() const { return label; }

	/// <summary>
	/// �������`�F�b�N���s��
	/// </summary>
	/// <param name="typeName">���O</param>
	/// <param name="label">���ʃ��x��</param>
	/// <returns>�����Ă����true</returns>
	bool CheckConsistency(const std::string& typeName, const std::string& label) const;

private:
	//================================================================================
	// �������o�ϐ�

	EffectDefine::EffectInfo info;		// ���g�̃G�t�F�N�g���
	std::string label;	// ���ʃ��x��
};