#pragma once
// ���p����
#include "src/util/object2D/Object2D.h"

// ���ėp
#include "src/util/math/Vector2.h"

/// <summary>
/// �T�u�g�[���̏��
/// </summary>
class UI_Subtone : public Object2D {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// �T�u�g�[����ݒ肷��
	/// </summary>
	UI_Subtone();

	/// <summary>
	/// �T�u�g�[����ݒ肷��
	/// </summary>
	/// <param name="image">�摜</param>
	/// <param name="color">�摜���w�肳��Ă��Ȃ��ꍇ�A�\������F</param>
	/// <param name="alpha">�s�����x</param>
	/// <param name="active">�@�\���Ă��邩</param>
	/// <param name="begin">�`��n�_���W(����)</param>
	/// <param name="end">�`��I�_���W(�E��)</param>
	UI_Subtone(const int& image, const int& color, const int& alpha, const bool& active, const Vector2& begin, const Vector2& end);

	~UI_Subtone();

	//================================================================================
	// ���e��֐�

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw(const Vector2& begin, const Vector2& end);

	/// <summary>
	/// �n�_�E�I�_���獶��A�E��A�����A�E���̍��W��ݒ肷��
	/// </summary>
	void SetPoint(const Vector2& begin, const Vector2& end);

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �摜���擾����
	/// </summary>
	/// <returns></returns>
	inline int Image() const { return hImage; }

	/// <summary>
	/// �摜���ݒ肳��Ă��Ȃ��ꍇ�ɕ\������F���擾����
	/// </summary>
	inline int Color() const { return color; }

	/// <summary>
	/// �s�����x���擾����
	/// </summary>
	inline int Alpha() const { return alpha; }

	/// <summary>
	/// �`�悷��n�_���W���擾����(����)
	/// </summary>
	inline Vector2 BeginPos() const { return begin; }

	/// <summary>
	/// �`�悷��I�_���W���擾����(�E��)
	/// </summary>
	inline Vector2 EndPos() const { return end; }

private:
	int color;		// �F
	int alpha;		// �s�����x

	Vector2 begin;	// �n�_���W
	Vector2 end;	// �I�_���W

	static const int POINT_DIR_NUM = 4;	// �`�悷��ۂ̌���

	// �l�p�`�̓_�̐F
	const unsigned int PointColor[4] = {
		GetColor(255, 255, 255),// ����
		GetColor(255,   0,   0),// �E��
		GetColor(  0, 255,   0),// �E��
		GetColor(  0,   0, 255)	// ����
	};

	Vector2 point[4];	// �摜�`��ʒu(0 = ����A1 = �E��A2 = �����A3 = �E��)
};