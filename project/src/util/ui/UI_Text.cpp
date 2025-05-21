#include "src/util/ui/UI_Text.h"

// �����o�E�@�\
#include "src/util/sound/SoundManager.h"

namespace {

	static const float ADJUST = 4.0f;	// �����l
}

UI_Text::UI_Text(const std::string& _text, const RectTransform& trs, const FontInfo& _fontInfo, const float& _scrollTotalTime, const std::string& _scrollSE) {

	Object2D::SetTransform(trs);
	SetText(_text, _fontInfo);

	scrollTime = _scrollTotalTime;
	scrollTotalTime = _scrollTotalTime;
	scrollSE = _scrollSE;

	afterScrollText = 0;
}

UI_Text::~UI_Text() {

}

void UI_Text::Update() {

	if (IsActive() == false || isUpdate == false)
		return;

	ScrollTextProcess();
	DrawTexts();
	ApplySubtone();

	UI_Canvas::Update();
}

void UI_Text::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	UI_Canvas::Draw();

	DrawTexts();
}

void UI_Text::DrawTexts() {

	RectTransform globalTrs = rectTransform->Global();	// �O���[�o���ȍ��W�E��]�E�g�k���
	Vector2 devi = DisplacementByAnchorPoint();	// �A���J�[�|�C���g�ɂ����W�̃Y���l

	FontInfo scaledFont = fontInfo;	// �g�嗦��K�������t�H���g���
	scaledFont.size = static_cast<int>(fontInfo.size * globalTrs.scale.Average());	// �g�嗦��K�������t�H���g�̑傫��

	// �t�H���g�f�[�^�𐶐�����
	Font::CreateFontToHandle(&scaledFont);

	// ���݂̃t�H���g�̑傫���Ɗg�嗦��K�������t�H���g�̑傫�����r����
	if (fontInfo.size != scaledFont.size)
		devi += Vector2((scaledFont.size - fontInfo.size) * -1.0f);

	// �s�����x��ݒ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// �������蒆�̏ꍇ
	if (IsScroll()) {

		// �\�����镶����
		int textScroll = length - static_cast<int>(length * ScrollRate());

		for (int i = 0; i < textScroll; i++)
			DrawFormatStringFToHandle(letters[i].x + devi.x, letters[i].y + devi.y, fontInfo.color, fontInfo.handle, letters[i].str.c_str());
	}
	else {
		Vector2 pos = globalTrs.position + devi;	// �`����W
		DrawStringFToHandle(pos.x, pos.y, text.c_str(), fontInfo.color, fontInfo.handle);

		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		//DrawBoxAA(pos.x, pos.y, pos.x + textSize.x, pos.y + textSize.y, 0xFF0000, true);
	}

	if (needAfterImage) {

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, easingAlphaAftImg.current);

		scaledFont.size = static_cast<int>(fontInfo.size * easingScaleAftImg.current.Average());	// �g�嗦��K�������t�H���g�̑傫��

		// �t�H���g�f�[�^�𐶐�����
		Font::CreateFontToHandle(&scaledFont);

		// ���݂̃t�H���g�̑傫���Ɗg�嗦��K�������t�H���g�̑傫�����r����
		Vector2 fontSizeDef = Vector2((scaledFont.size - fontInfo.size) * -5.f, (scaledFont.size - fontInfo.size) * -0.75f);
		devi = DisplacementByAnchorPoint();

		Vector2 pos = globalTrs.position + devi + fontSizeDef + easingMoveAftImg.current;	// �`����W
		DrawStringFToHandle(pos.x, pos.y, text.c_str(), scaledFont.color, scaledFont.handle);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UI_Text::ScrollTextProcess() {

	scrollTime = max(scrollTime - GTime.deltaTime, 0.0f);

	// �X�N���[������������������
	const int textScroll = length - static_cast<int>(length * ScrollRate());

	// �X�N���[�������A�܂��X�N���[������Ă��Ȃ����A�������X�y�[�X�ł͂Ȃ��ꍇ
	if (IsScroll() && afterScrollText < textScroll && text[textScroll] != 0x20) {
		afterScrollText = textScroll;
		SoundManager::Play(scrollSE, "TextScroll");
	}
}

void UI_Text::ApplySubtone() {

	// �T�u�g�[�����ғ����Ă��Ȃ��ꍇ
	if ((subtone != nullptr && subtone->IsActive()) == false)
		return;

	RectTransform globalTrs = rectTransform->Global();	// �O���[�o���ȍ��W�E��]�E�g�k���
	Vector2 devi = DisplacementByAnchorPoint();	// �A���J�[�|�C���g�ɂ����W�̃Y���l

	Vector2 begin = Vector2::Zero;	// �T�u�g�[���̎n�_���W(����)
	Vector2 end = Vector2::Zero;		// �T�u�g�[���̏I�_���W(�E��)

	// �T�u�g�[���ɉ摜���ݒ肳��Ă���ꍇ
	if (subtone->Image() != -1) {
		begin = globalTrs.position + Vector2(-textSize.x * 0.5f, -textSize.y);
		end = globalTrs.position + Vector2(textSize.x * 0.5f, textSize.y);
	}
	// �T�u�g�[���ɉ摜���ݒ肳��Ă��Ȃ��ꍇ
	else {
		begin = Vector2(globalTrs.position.x + devi.x, globalTrs.position.y + devi.y);
		end = Vector2(globalTrs.position.x + devi.x + textSize.x, globalTrs.position.y + devi.y + textSize.y);
	}

	subtone->SetPoint(begin, end);
}

void UI_Text::SetText(const std::string& _text) {

	SetText(_text, fontInfo);
}

void UI_Text::SetText(const std::string& _text, const FontInfo& _font) {

	text = _text;	// �������ύX
	length = Len();
	fontInfo = _font;

	Font::CreateFontToHandle(&fontInfo);	// �t�H���g����ύX

	textSize.x = static_cast<float>(GetDrawStringWidthToHandle(text.c_str(), Len(), fontInfo.handle));
	textSize.y = fontInfo.size;

	RectTransform globalTrs = rectTransform->Global();	// �O���[�o���ȍ��W�E��]�E�g�k���
	Vector2 diff = Vector2::Zero;	// ���_���當�����W�ւ̃x�N�g��

	letters.clear();	// �ۑ����Ă���������������

	// �ꕶ�����ۑ�
	for (int i = 0; i < Len(); i++) {

		std::string letter;

		if (static_cast<unsigned char>(text[i]) >= 0x80) {
			letter = text.substr(i, 2);	// �S�p�Ȃ�2�����ۑ�
			length--;
			i++;
		}
		else {
			letter = text.substr(i, 1);		// ���p�Ȃ�1�����ۑ�
		}

		letters.push_back(String::Letter(letter, globalTrs.position.x + diff.x, globalTrs.position.y + diff.y));

		// ���̕����̕`����W�����炷
		diff.x += GetDrawStringWidthToHandle(letter.c_str(), static_cast<int>(letter.length()), fontInfo.handle);

		// ���s�̏ꍇ
		if (text.substr(i, 1) == "\n") {
			diff.x = 0.0f;
			diff.y += textSize.y + 4.0f;
		}
	}
}

void UI_Text::SetSubtone(const UI_Subtone& sub) {

	UI_Canvas::SetSubtone(sub);

	ApplySubtone();
}

Vector2 UI_Text::DisplacementByAnchorPoint() {

	Vector2 offset = Vector2::Zero;	// ��

	// �A���J�[�|�C���g�ɂ���ĉ��̒��S�_���Y����
	switch (Anchor::PlacementHorizontal(rectTransform->preset)) {
	case Anchor::Placement::LEFT:	offset.x = 0.0f;				break;
	case Anchor::Placement::MIDDLE:	offset.x = -textSize.x * 0.5f;	break;
	case Anchor::Placement::RIGHT:	offset.x = -textSize.x;			break;
	default:						offset.x = 0.0f;				break;
	}

	// �A���J�[�|�C���g�ɂ���ďc�̒��S�_���Y����
	switch (Anchor::PlacementVertical(rectTransform->preset)) {
	case Anchor::Placement::UP:		offset.y = 0.0f;						break;
	case Anchor::Placement::MIDDLE:	offset.y = -textSize.y * 0.5f - ADJUST;	break;
	case Anchor::Placement::DOWN:	offset.y = -textSize.y;					break;
	default:						offset.y = 0.0f;						break;
	}

	return offset;
}
