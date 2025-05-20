#pragma once
#include "../Library/gameObject.h"
#include "../Library/myDxLib.h"
#include "../Source/Util/RectTransform.h"

/// <summary>
/// 2D�I�u�W�F�N�g�Ɋւ������N���X
/// <remarks>�g�����X�t�H�[����n���h���̏���ێ����Ă���</remarks>
/// </summary>
class Object2D : public GameObject {
public:
	//==========================================================================================
	// ��public�ϐ�

	RectTransform* rectTransform;	// ���W�E��]�E�g�k�̏��

	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	Object2D();

	/// <summary>
	/// ������RectTranform�ŏ��������s��
	/// </summary>
	/// <param name="rectTransform">���W�E��]�E�g�k�̏��</param>
	Object2D(const RectTransform& rectTransform);

	/// <summary>
	/// ������RectTranform�ŏ��������s��
	/// </summary>
	/// <param name="image">�`��摜</param>
	/// <param name="rectTransform">���W�E��]�E�g�k�̏��</param>
	Object2D(const int& image, const RectTransform& rectTransform);

	virtual ~Object2D();

	//==========================================================================================
	// ���e��֐�

	virtual void Draw() override;

	//==========================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// ���W�E��]�E�g�k�̏���ݒ肷��
	/// </summary>
	void SetTransform(const RectTransform& trs);

	/// <summary>
	/// �摜��ݒ肷��(���̑��̐ݒ�͕ύX���Ȃ�)
	/// </summary>
	/// <param name="handle">�摜�̃n���h��</param>
	void SetImage(const int& handle);

	/// <summary>
	/// �摜��ݒ肷��(���̑��̐ݒ���ύX����)
	/// </summary>
	/// <param name="handle">�摜�̃n���h��</param>
	/// <param name="clipStart">�؂蔲���J�n�n�_</param>
	/// <param name="clipSize">�؂蔲���̑傫��</param>
	/// <param name="center">�摜�̒��S�_</param>
	void SetImage(const int& handle, const Vector2& clipStart, const Vector2& clipSize, const Vector2& center);

	/// <summary>
	/// �؂蔲���J�n�_��ݒ肷��
	/// </summary>
	/// <param name="value">�؂蔲�����J�n������W(����)</param>
	inline void SetClipStart(const Vector2& value) { clipStart = value; }

	/// <summary>
	/// �؂蔲���̑傫����ݒ肷��
	/// </summary>
	/// <param name="value">�؂蔲���̑傫��</param>
	inline void SetClipSize(const Vector2& value) { clipSize = value; }

	/// <summary>
	/// �`�掞�A�摜�̂ǂ̈ʒu�𒆐S�Ƃ��邩��ݒ肷��(0..1)
	/// </summary>
	/// <param name="value">0.5f�̏ꍇ�A�摜�̒��S��`�掞�̒��S�Ƃ���</param>
	inline void SetCenter(const Vector2& value) { center = value; }

	//==========================================================================================
	// ���Q�b�^�[

	// �摜�f�[�^���擾����
	inline int Image() const { return hImage; }

	// �g�嗦���܂߂��A��ʂɑ΂���摜�̕`��T�C�Y���擾����
	inline Vector2 ImageSize() const { return Vector2(imageSize.x * rectTransform->scale.x, imageSize.y * rectTransform->scale.y); }

	// �A���J�[�|�C���g�ɂ��Y���l���擾����
	virtual Vector2 DisplacementByAnchorPoint();

protected:
	//==========================================================================================
	// �������o�ϐ�

	int hImage;			// �摜�̃n���h��

	Vector2 imageSize;	// �摜�T�C�Y
	Vector2 clipStart;	// �؂蔲���J�n�_
	Vector2 clipSize;	// �؂蔲���̑傫��
	Vector2 center;		// �`�掞�A�摜�̂ǂ̈ʒu�𒆐S�Ƃ��邩��ݒ肷��(0..1)
};