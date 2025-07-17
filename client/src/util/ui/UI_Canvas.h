#pragma once
// ���p����
#include "src/util/object2D/Object2D.h"

// �����o�E�@�\
#include "src/util/easing/Easing.h"

// ���ʂŕK�v�ȕ�
#include "src/util/ui/UI_Subtone.h"
#include "src/util/ui/UI_Define.h"

/// <summary>
/// UI�Ɋւ�����N���X
/// </summary>
/// <author> �~�b�` </author>
class UI_Canvas : public Object2D {
public:
	//================================================================================
	// ���񋓌^

	/// <summary>
	/// �C�[�W���O�����̎��
	/// </summary>
	enum EasingMovement {
		eMove,	// �ړ�
		eRot,	// ��]
		eScale,	// �g�k
		eAlpha,	// �s�����x
		eMax
	};

	//================================================================================
	// ���R���X�g���N�^

	UI_Canvas();
	virtual ~UI_Canvas();

	//================================================================================
	// ���e��֐�

	virtual void Update() override;
	virtual void Draw() override;

	/// <summary>
	/// �c���`�揈��
	/// </summary>
	virtual void DrawAfterImage() {};

	/// <summary>
	/// �_�ŏ���
	/// </summary>
	void BlinkUpdate();

	/// <summary>
	/// ��ԏ���
	/// </summary>
	void EasingUpdate();

	/// <summary>
	/// �c���`�揈��
	/// </summary>
	void AfterImageUpdate();

	/// <summary>
	/// ��ʃT�C�Y�ɉ����Ċg�k�{����ω�������
	/// </summary>
	void ScaleWithScreenSize();

	/// <summary>
	/// �T�u�g�[�����摜�ɍ��킹�Ē�������
	/// </summary>
	void SubtoneAdaptation(const UI_Subtone& sub);

	//================================================================================
	// ���Z�b�^�[

	/// <summary>
	/// �s�����x��ݒ肷��
	/// </summary>
	inline void SetAlpha(int _alpha) { alpha = _alpha; }

	/// <summary>
	/// �`��D��x��ݒ肷��
	/// </summary>
	void SetPriority(int value);

	/// <summary>
	/// �T�u�g�[����ݒ肷��
	/// </summary>
	virtual void SetSubtone(const UI_Subtone& sub);

	/// <summary>
	/// �X�V�������s������ݒ肷��
	/// </summary>
	inline void SetIsUpdate(bool value) { isUpdate = value; }

	/// <summary>
	/// �`�揈�����s������ݒ肷��
	/// </summary>
	inline void SetIsDraw(bool value) { isDraw = value; }

	/// <summary>
	/// �_�ŏ������s������ݒ肷��
	/// </summary>
	inline void SetNeedBlink(bool value) { needBlink = value; }

	/// <summary>
	/// �c���������s������ݒ肷��
	/// </summary>
	inline void SetAfterImage(bool value) { needAfterImage = value; }

	/// <summary>
	/// �_�ł��鎞�Ԃ�ݒ肷��(�����̕b���œ_�ł̈�A�̗�����s��)
	/// </summary>
	inline void SetBlinkTime(float sec) { blinkSpeed = sec; }

	/// <summary>
	/// �C�[�W���O������ݒ肷��
	/// </summary>
	/// <param name="useType">�����̎��</param>
	/// <param name="begin">�n�_</param>
	/// <param name="end">�I�_</param>
	/// <param name="totalTime">���ʎ���</param>
	/// <param name="type">�C�[�W���O�̎��</param>
	template<typename Ty>
	void SetEasing(EasingMovement move, const Ty& begin, const Ty& end, float totalTime, EasingType type = EasingType::Linear);

	/// <summary>
	/// �C�[�W���O������ݒ肷��
	/// </summary>
	/// <param name="useType">�����̎��</param>
	/// <param name="begin">�n�_</param>
	/// <param name="end">�I�_</param>
	/// <param name="totalTime">���ʎ���</param>
	/// <param name="type">�C�[�W���O�̎��</param>
	template<typename Ty>
	void SetAfterImageEasing(EasingMovement move, const Ty& begin, const Ty& end, float totalTime, EasingType type = EasingType::Linear);

	//================================================================================
	// ���Q�b�^�[

	/// <summary>
	/// �摜���擾����
	/// </summary>
	inline int Image() const { return hImage; }

	/// <summary>
	/// �s�����x���擾����
	/// </summary>
	inline int Alpha() const { return alpha; }

	/// <summary>
	/// �`��D��x���擾����
	/// </summary>
	inline int Priority() const { return priority; }

	/// <summary>
	/// �X�V�������s�������擾����
	/// </summary>
	inline bool IsUpdate() const { return (IsActive() && isUpdate); }

	/// <summary>
	/// �`�揈�����s�������擾����
	/// </summary>
	inline bool IsDraw() const { return (IsActive() && isDraw); }

	/// <summary>
	/// �_�ŏ������s�������擾����
	/// </summary>
	inline bool NeedBlink() const { return needBlink; }

	/// <summary>
	/// �T�u�g�[�����擾����
	/// </summary>
	inline UI_Subtone* Subtone() const { return subtone; }

	/// <summary>
	/// �C�[�W���O�������ғ����Ă��邩
	/// </summary>
	/// <returns>�C�[�W���O��������ł��ғ����Ă����True</returns>
	bool IsAllEasingRun() const;

	/// <summary>
	/// �c���C�[�W���O�������ғ����Ă��邩
	/// </summary>
	/// <returns>�c���C�[�W���O��������ł��ғ����Ă����True</returns>
	bool IsAllEasingRunAfterImage() const;

protected:
	//================================================================================
	// ���ϐ�'s

	int alpha;			// �s�����x
	int priority;		// �`��D��x
	bool isUpdate;		// �X�V�������s����
	bool isDraw;		// �`�揈�����s����
	UI_Subtone* subtone;// �T�u�g�[���̏��

	//================================================================================
	// ����ʏ��

	Vector2 scaleWithScreenSize;// ��ʃT�C�Y�ɉ����Ċg�k�{����ω�������
	int screenX, screenY;		// ��ʃT�C�Y
	int colorBitDepth;			// �J���[�r�b�g

	//================================================================================
	// ���C�[�W���O����

	EasingVec2 easingMove;	// �ړ��p�C�[�W���O���
	EasingVec2 easingScale;	// �g�k�p�C�[�W���O���
	EasingFloat easingRot;	// ��]�p�C�[�W���O���
	EasingInt easingAlpha;	// �s�����x�p�C�[�W���O���

	//================================================================================
	// ���_�ŋ���

	float blinkTime;	// �_�Ŏ���
	float blinkTimeMax;	// �ő�_�Ŏ���

	float blinkCounter;	// �_�ŗp
	float blinkSpeed;	// �_�ő��x

	bool needBlink;		// �_�ł����邩

	//================================================================================
	// ���c������

	EasingVec2 easingMoveAftImg;	// �ړ��p�C�[�W���O���
	EasingVec2 easingScaleAftImg;	// �g�k�p�C�[�W���O���
	EasingFloat easingRotAftImg;	// ��]�p�C�[�W���O���
	EasingInt easingAlphaAftImg;	// �s�����x�p�C�[�W���O���

	bool needAfterImage;	// �c����\�����邩
};

/// <summary>
/// �C�[�W���O������ݒ肷��
/// </summary>
/// <param name="useType">�����̎��</param>
/// <param name="begin">�n�_</param>
/// <param name="end">�I�_</param>
/// <param name="totalTime">���ʎ���</param>
/// <param name="type">�C�[�W���O�̎��</param>
template<typename Ty>
inline void UI_Canvas::SetEasing(EasingMovement move, const Ty& begin, const Ty& end, float totalTime, EasingType type) {

	switch (move) {
	case EasingMovement::eMove:
		easingMove.begin = begin;
		easingMove.end = end;
		easingMove.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingMove.info, &rectTransform->position, easingMove.end, easingMove.begin);
		break;

	case EasingMovement::eRot:
		easingRot.begin = begin;
		easingRot.end = end;
		easingRot.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingRot.info, &rectTransform->rotation, easingRot.end, easingRot.begin);
		break;

	case EasingMovement::eScale:
		easingScale.begin = begin;
		easingScale.end = end;
		easingScale.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingScale.info, &rectTransform->scale, easingScale.end, easingScale.begin);
		break;

	case EasingMovement::eAlpha:
		easingAlpha.begin = begin;
		easingAlpha.end = end;
		easingAlpha.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingAlpha.info, &alpha, easingAlpha.end, easingAlpha.begin);
		break;
	}
}

/// <summary>
/// �C�[�W���O������ݒ肷��
/// </summary>
/// <param name="useType">�����̎��</param>
/// <param name="begin">�n�_</param>
/// <param name="end">�I�_</param>
/// <param name="totalTime">���ʎ���</param>
/// <param name="type">�C�[�W���O�̎��</param>
template<typename Ty>
inline void UI_Canvas::SetAfterImageEasing(EasingMovement move, const Ty& begin, const Ty& end, float totalTime, EasingType type) {

	switch (move) {
	case EasingMovement::eMove:
		easingMoveAftImg.begin = begin;
		easingMoveAftImg.end = end;
		easingMoveAftImg.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingMoveAftImg.info, &easingMoveAftImg.current, easingMoveAftImg.end, easingMoveAftImg.begin);
		break;

	case EasingMovement::eRot:
		easingRotAftImg.begin = begin;
		easingRotAftImg.end = end;
		easingRotAftImg.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingRotAftImg.info, &easingRotAftImg.current, easingRotAftImg.end, easingRotAftImg.begin);
		break;

	case EasingMovement::eScale:
		easingScaleAftImg.begin = begin;
		easingScaleAftImg.end = end;
		easingScaleAftImg.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingScaleAftImg.info, &easingScaleAftImg.current, easingScaleAftImg.end, easingScaleAftImg.begin);
		break;

	case EasingMovement::eAlpha:
		easingAlphaAftImg.begin = begin;
		easingAlphaAftImg.end = end;
		easingAlphaAftImg.info = EasingInfo(0.0f, totalTime, true, GameTime::AdditionMethod::Rate, type);
		EasingFunc::Process(&easingAlphaAftImg.info, &easingAlphaAftImg.current, easingAlphaAftImg.end, easingAlphaAftImg.begin);
		break;
	}
}