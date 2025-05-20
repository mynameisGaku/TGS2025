#include "LoadScreen.h"
#include "../Library/resourceLoader.h"
#include "Font.h"

LoadScreen::LoadScreen() {

	useFont.color = GetColor(255, 255, 255);
	useFont.strData.tag = "�����Ȃݖ���";
	useFont.size = 28;
	useFont.edgeSize = 2;
	useFont.fontType = DX_FONTTYPE_ANTIALIASING_EDGE_4X4;
	useFont.italic = 0;
	Font::CreateFontToHandle(&useFont);

	fadeEasing.SetEasing(0.0f, 0.0f, 0.0f, EasingType::Linear, false);

	rate = 0.0f;
	textWidth = 0.0f;
	loadText = LOADING_TEXT;

	hBackground = -1;
	isPushFadeOut = false;
}

LoadScreen::~LoadScreen() {

	ResourceLoader::DeleteGraph(hBackground);
	hBackground = -1;
}

void LoadScreen::Update() {

	fadeEasing.Update();

	GameObject::Update();
}

void LoadScreen::Draw() {

	GameObject::Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(fadeEasing.current));

	DrawBackGround();
	DrawLoadText();
	DrawLoadGauge();
	DrawButtonTip();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void LoadScreen::DrawBackGround() {

	const float SCROOL_SPEED = 6.0f;

	// ���w�i�摜���ݒ肳��Ă�����A�摜��\��
	if (hBackground != -1) {

		Vector2 begin = bgPos;	// �`��n�_���W
		Vector2 end	  = bgPos + Vector2(Screen::WIDTH_F, Screen::HEIGHT_F);	// �`��I�_���W

		switch (scrollType) {
		case LoadScreen::ScrollType::stUp:		// ����X�N���[������

			bgPos.y -= SCROOL_SPEED;
			begin = Vector2(bgPos.x,					bgPos.y + Screen::HEIGHT_F);
			end	  = Vector2(bgPos.x + Screen::WIDTH_F,	bgPos.y + Screen::HEIGHT_F * 2.0f);

			DrawExtendGraphF(begin.x, begin.y, end.x, end.y, hBackground, false);
			break;

		case LoadScreen::ScrollType::stDown:	// �����X�N���[������

			bgPos.y += SCROOL_SPEED;
			begin = Vector2(bgPos.x,					bgPos.y - Screen::HEIGHT_F);
			end	  = Vector2(bgPos.x + Screen::WIDTH_F,	bgPos.y);

			DrawExtendGraphF(begin.x, begin.y, end.x, end.y, hBackground, false);
			break;

		case LoadScreen::ScrollType::stLeft:	// �����X�N���[������

			bgPos.x -= SCROOL_SPEED;
			begin = Vector2(bgPos.x + Screen::WIDTH_F,			bgPos.y);
			end	  = Vector2(bgPos.x + Screen::WIDTH_F * 2.0f,	bgPos.y + Screen::HEIGHT_F);

			DrawExtendGraphF(begin.x, begin.y, end.x, end.y, hBackground, false);
			break;

		case LoadScreen::ScrollType::stRight:	// ���E�X�N���[������
			
			bgPos.x += SCROOL_SPEED;
			begin = Vector2(bgPos.x - Screen::WIDTH_F,	bgPos.y);
			end	  = Vector2(bgPos.x,					bgPos.y + Screen::HEIGHT_F);

			DrawExtendGraphF(begin.x, begin.y, end.x, end.y, hBackground, false);
			break;

		default:
			break;
		}

		if (bgPos.x >= Screen::WIDTH_F)
			bgPos.x -= Screen::WIDTH_F;
		else if (bgPos.x <= Screen::WIDTH_F * -1.0f)
			bgPos.x += Screen::WIDTH_F;

		if (bgPos.y >= Screen::HEIGHT_F)
			bgPos.y -= Screen::HEIGHT_F;
		else if (bgPos.y <= Screen::HEIGHT_F * -1.0f)
			bgPos.y += Screen::HEIGHT_F;

		begin = bgPos;
		end = bgPos + Vector2(Screen::WIDTH_F, Screen::HEIGHT_F);
		
		DrawExtendGraphF(begin.x, begin.y, end.x, end.y, hBackground, false);
	}
	// ���ݒ肳��Ă��Ȃ�������AWindow�̊���w�i�F�œh��Ԃ�
	else {
		int r, g, b;
		GetBackgroundColor(&r, &g, &b);
		DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(r, g, b), true);
	}
}

void LoadScreen::DrawLoadText() {

	// �����[�h���e�L�X�g�̕\��
	if (rate < 1.0f) {
		DrawFormatStringFToHandle(DRAW_TEXT_POS.x, DRAW_TEXT_POS.y, TEXT_COLOR, useFont.handle, (loadText + "%%").c_str(), rate * 100.0f);
	}
	// �����[�h�����e�L�X�g�̕\��
	else {
		DrawFormatStringFToHandle(DRAW_TEXT_POS.x, DRAW_TEXT_POS.y, TEXT_COLOR, useFont.handle, LOAD_END_TEXT.c_str());
	}
}

void LoadScreen::DrawLoadGauge() {

	Vector2 begin = GAUGE_POS;				// �Q�[�W�̎n�_(����)
	Vector2 end = GAUGE_POS + GAUGE_SIZE;	// �Q�[�W�̏I�_(�E��)
	Vector2 adjust = Vector2(2.0f);			// �����l

	// �����~���̕`��
	DrawBoxAA(begin.x, begin.y, end.x, end.y, GetColor(255, 255, 255), true);
	
	// �����̕`��
	begin += adjust;
	end	  -= adjust;
	DrawBoxAA(begin.x, begin.y, end.x, end.y, GetColor(0, 0, 0), true);

	// ���Q�[�W(����)
	begin += adjust;
	end	  -= adjust;
	DrawBoxAA(begin.x, begin.y, end.x, end.y, GetColor(100, 100, 100), true);
	
	// ���Q�[�W(����)
	DrawBoxAA(begin.x, begin.y, end.x - GAUGE_SIZE.x * (1.0f - rate), end.y, GAUGE_COLOR, true);
}

void LoadScreen::DrawButtonTip() {

	// �����[�h�I�����Ƀ{�^���q���g��\������
	if (isPushFadeOut && rate >= 1.0f) {
		int width = GetDrawStringWidthToHandle(BUTTON_TIP.c_str(), static_cast<int>(BUTTON_TIP.length()), useFont.handle);
		DrawFormatStringToHandle(Screen::WIDTH - width - 12, Screen::HEIGHT - 32, TEXT_COLOR, useFont.handle, BUTTON_TIP.c_str());
	}
}

void LoadScreen::SetRate(const float& _rate) {

	rate = _rate;
	
	if (rate < 1.0f) {
		loadText = Function::FormatToString(LOADING_TEXT.c_str(), rate * 100.0f);
		const int len = static_cast<int>(loadText.length());
		textWidth = static_cast<float>(GetDrawStringWidth(loadText.c_str(), len));
	}
	else {
		loadText = "";
	}
}

void LoadScreen::SetBackGround(const std::string& filename, const ScrollType& scroll) {

	if (hBackground > -1)
		ResourceLoader::DeleteGraph(hBackground);

	hBackground = ResourceLoader::LoadGraph(filename);
	scrollType = scroll;
}

bool LoadScreen::IsLoadEnd() const {

	if (isPushFadeOut)
		return (rate >= 1.0f && CheckHitKey(KEY_INPUT_Z) == 1);

	return (rate >= 1.0f);
}
