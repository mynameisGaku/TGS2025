#pragma once

// ���ėp
#include "src/util/math/Vector2.h"
#include "Anchor.h"
#include <list>

/// <summary>
/// ���W�A��]�A�g�k�����N���X
/// </summary>
/// <author> �~�b�` </author>
class RectTransform {
public:
	//==========================================================================================
	// �������o�ϐ�

	Vector2 position;	// ���W
	float rotation;		// ��]
	Vector2 scale;		// �g�k
	Anchor::Preset preset;	// �A���J�[�|�C���g

	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	RectTransform();
	RectTransform(const Vector2& position, const float& rotation = 0.0f, const Vector2& scale = Vector2::Ones, RectTransform* parent = nullptr);
	RectTransform(Anchor::Preset preset, const Vector2& position = Vector2::Zero, const float& rotation = 0.0f, const Vector2& scale = Vector2::Ones, RectTransform* _parent = nullptr);

	~RectTransform();

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �e��ݒ肷��
	/// </summary>
	/// <param name="_parent">�e�ɂ���g�����X�t�H�[��</param>
	void SetParent(RectTransform* _parent);

	/// <summary>
	/// �w�肵���O���[�o���ɂȂ�悤�ɁA���[�J����ύX����
	/// </summary>
	/// <param name="global">�O���[�o�����W</param>
	void SetGlobal(const RectTransform& global);

	//================================================================================
	// ���Q�b�^�[

	// �e�̉e�����܂߂��O���[�o�������擾(�e��X��10�A�q��X��20��������q�̃O���[�o�����W��X30�A�݂�����)
	// �e�����ݒ�Ȃ�Ӗ��Ȃ�
	const RectTransform Global() const;

	// ���g�̕����i���擾����
	RectTransform* Copy() const;

	//================================================================================
	// ���v�Z

	const RectTransform operator+(const RectTransform& trs) const;
	const RectTransform operator-(const RectTransform& trs) const;
	const RectTransform operator*(const float sca) const;
	const RectTransform operator/(const float sca) const;
	const RectTransform operator-() const;
	void operator+=(const RectTransform& trs);
	void operator-=(const RectTransform& trs);
	void operator*=(const float sca);

	//================================================================================
	// ���f�o�b�O

	/// <summary>
	/// �f�o�b�O�p�ɃA���J�[�|�C���g�̍��W�ɔ��F�̉~��`�悷��
	/// </summary>
	static void DrawTest();

private:
	RectTransform* parent;	// �e�g�����X�t�H�[���̃|�C���^
	std::list<RectTransform*> children;	// �q�g�����X�t�H�[���̃|�C���^�̃��X�g

	//================================================================================
	// ���e��֐�

	// �q��ǉ�
	inline void AddChild(RectTransform* child) { children.push_back(child); }

	// �q�����X�g�������
	void RemoveChild(RectTransform* child);

	/// <summary>
	/// �e�Ƃ̘A�g������
	/// </summary>
	/// <param name="_parent">�e�̃|�C���^</param>
	/// <param name="removeFromChildren">�e�̎q�����X�g�������</param>
	void RemoveParent(RectTransform* _parent, bool removeFromChildren = true);
};
