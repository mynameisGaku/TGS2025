#pragma once
// ���p����
#include "src/util/object3D/Object3D.h"

// ���ėp
#include "src/util/easing/Easing.h"
#include "src/util/transform/Transform.h"
#include "src/util/transform/RectTransform.h"

// ���ʂŕK�v�ȕ�
#include "src/util/fx/effect/EffectDefine.h"

/// <summary>
/// �G�t�F�N�g���Đ�����N���X
/// </summary>
class EffectBase : public Object3D {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	EffectBase();
	~EffectBase();

	//================================================================================
	// ���e��֐�

	void Update() override;

	/// <summary>
	/// 3D�G�t�F�N�g���Đ�����
	/// </summary>
	/// <param name="info">�G�t�F�N�g�̏��</param>
	/// <param name="transform">���W�E��]�E�g�k�̏��</param>
	/// <param name="label">���ʃ��x��</param>
	/// <param name="isLoop">���[�v�Đ����s����</param>
	void Play3D(const EffectDefine::EffectInfo& info, const Transform& transform, const std::string& label = "", const bool& isLoop = false);

	/// <summary>
	/// 2D�G�t�F�N�g���Đ�����
	/// </summary>
	/// <param name="info">�G�t�F�N�g�̏��</param>
	/// <param name="transform">���W�E��]�E�g�k�̏��</param>
	/// <param name="label">���ʃ��x��</param>
	/// <param name="isLoop">���[�v�Đ����s����</param>
	void Play2D(const EffectDefine::EffectInfo& info, const RectTransform& transform, const std::string& label = "", const bool& isLoop = false);

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
	void SetTransform3D(const Transform& transform);
	void SetTransform2D(const RectTransform& transform);

	/// <summary>
	/// ���W��ݒ肷��
	/// </summary>
	/// <param name="pos">���W�̏��</param>
	void SetPosition3D(const Vector3& pos);
	void SetPosition2D(const Vector2& pos);

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="rot">��]�̏��</param>
	void SetRotation3D(const Vector3& rot);
	void SetRotation2D(float rot);

	/// <summary>
	/// �g�k��ݒ肷��
	/// </summary>
	/// <param name="scale">�g�k�̏��</param>
	void SetScale3D(const Vector3& scale);
	void SetScale2D(const Vector2& scale);

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

	EffectDefine::EffectInfo info;	// ���g�̃G�t�F�N�g���
	std::string label;				// ���ʃ��x��

	EasingInt alphaEasing;
};