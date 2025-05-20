#include "UI_Canvas.h"
#include "UI_Manager.h"

// ���ėp
#include "../Library/myDxLib.h"

UI_Canvas::UI_Canvas() {

	//================================================================================
	// ����ʃT�C�Y�ɉ����Ċg�k�{����ω�������

	ScaleWithScreenSize();

	//================================================================================
	// ���C�[�W���O����

	easingMove	= EasingVec2();
	easingScale = EasingVec2();
	easingRot	= EasingFloat();
	easingAlpha = EasingInt();

	//================================================================================
	// ���_�ŋ���

	blinkTime = 0.0f;
	blinkCounter = 0.0f;
	blinkSpeed = Math::DegToRad(1.0f);

	needBlink = false;

	//================================================================================
	// ���c������

	easingMoveAftImg	= EasingVec2();
	easingScaleAftImg	= EasingVec2();
	easingRotAftImg		= EasingFloat();
	easingAlphaAftImg	= EasingInt();

	needAfterImage = false;
	
	//================================================================================
	// ���ϐ�'s
	
	hImage = -1;
	priority = 0;
	alpha = 255;

	subtone = nullptr;

	isUpdate = true;
	isDraw = true;

	UI_Manager::Add(this);
}

UI_Canvas::~UI_Canvas() {

	UI_Manager::Detach(this);
	Function::DeletePointer(subtone);
}

void UI_Canvas::Update() {

	if (IsActive() == false || isUpdate == false)
		return;

	// �摜�T�C�Y�ɉ����Ċg�k�{����ω�������
	ScaleWithScreenSize();

	// �_�ŏ���
	BlinkUpdate();

	// ��ԏ���
	EasingUpdate();

	// �c���`�揈��
	AfterImageUpdate();

	Object2D::Update();
}

void UI_Canvas::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	if (subtone != nullptr)
		subtone->Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	Object2D::Draw();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UI_Canvas::BlinkUpdate() {

	//================================================================================
	// ���_�ŏ���

	if (needBlink == false || blinkTimeMax == 0.0f)
		return;

	//================================================================================
	// �����ԉ��Z
	
	blinkTime += Time::DeltaTimeLapseRate();
	if (blinkTime >= blinkTimeMax)
		blinkTime = 0.0f;

	//================================================================================
	// ���{���v�Z

	float rate = blinkTime / (blinkTimeMax * 0.5f);	// �{��

	if (rate >= 1.0f)
		rate = 1.0f - rate;

	alpha = static_cast<int>(75 * rate) + 180;
}

void UI_Canvas::EasingUpdate() {

	//================================================================================
	// ����ԏ���

	EasingFunc::Process(&easingMove.info,	&rectTransform->position,	easingMove.end,		easingMove.begin);
	EasingFunc::Process(&easingRot.info,	&rectTransform->rotation,	easingRot.end,		easingRot.begin);
	EasingFunc::Process(&easingScale.info,	&rectTransform->scale,		easingScale.end,	easingScale.begin);
	EasingFunc::Process(&easingAlpha.info,	&alpha,						easingAlpha.end,	easingAlpha.begin);
}

void UI_Canvas::AfterImageUpdate() {

	//================================================================================
	// ���c���`�揈��

	if (needAfterImage == false)
		return;

	easingMoveAftImg.Update();
	easingRotAftImg.Update();
	easingScaleAftImg.Update();
	easingAlphaAftImg.Update();

	if (IsAllEasingRunAfterImage() == false)
		needAfterImage = false;
}

void UI_Canvas::ScaleWithScreenSize() {

	// ��ʏ����擾
	GetScreenState(&screenX, &screenY, &colorBitDepth);

	// �擾������ʏ��ƁA����̉�ʃT�C�Y�Ŋg�k�{����ω�������
	scaleWithScreenSize = Vector2(static_cast<float>(screenX) / Screen::WIDTH, static_cast<float>(screenY) / Screen::HEIGHT);
}

void UI_Canvas::SubtoneAdaptation(const UI_Subtone& sub) {

	Function::DeletePointer(subtone);
	subtone = new UI_Subtone(sub);

	RectTransform globalTrs = rectTransform->Global();
	Vector2 devi = DisplacementByAnchorPoint();

	subtone->SetPoint(globalTrs.position + devi, globalTrs.position + devi + ImageSize());
}

void UI_Canvas::SetPriority(const int& value) {

	priority = value;
	UI_Manager::Sort();
}

void UI_Canvas::SetSubtone(const UI_Subtone& sub) {

	Function::DeletePointer(subtone);
	subtone = new UI_Subtone(sub);
}

bool UI_Canvas::IsAllEasingRun() const {

	bool isMove  = easingMove.info.isActive;
	bool isRot	 = easingRot.info.isActive;
	bool isScale = easingScale.info.isActive;
	bool isAlpha = easingAlpha.info.isActive;

	return (isMove || isRot || isScale || isAlpha);
}

bool UI_Canvas::IsAllEasingRunAfterImage() const {

	bool isMove  = easingMoveAftImg.info.isActive;
	bool isRot	 = easingRotAftImg.info.isActive;
	bool isScale = easingScaleAftImg.info.isActive;
	bool isAlpha = easingAlphaAftImg.info.isActive;

	return (isMove || isRot || isScale || isAlpha);
}
