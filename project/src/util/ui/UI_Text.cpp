#include "src/util/ui/UI_Text.h"

// ◇演出・機能
#include "src/util/sound/SoundManager.h"

namespace {

	static const float ADJUST = 4.0f;	// 調整値
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

	RectTransform globalTrs = rectTransform->Global();	// グローバルな座標・回転・拡縮情報
	Vector2 devi = DisplacementByAnchorPoint();	// アンカーポイントによる座標のズレ値

	FontInfo scaledFont = fontInfo;	// 拡大率を適応したフォント情報
	scaledFont.size = static_cast<int>(fontInfo.size * globalTrs.scale.Average());	// 拡大率を適応したフォントの大きさ

	// フォントデータを生成する
	Font::CreateFontToHandle(&scaledFont);

	// 現在のフォントの大きさと拡大率を適応したフォントの大きさを比較する
	if (fontInfo.size != scaledFont.size)
		devi += Vector2((scaledFont.size - fontInfo.size) * -1.0f);

	// 不透明度を設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// 文字送り中の場合
	if (IsScroll()) {

		// 表示する文字数
		int textScroll = length - static_cast<int>(length * ScrollRate());

		for (int i = 0; i < textScroll; i++)
			DrawFormatStringFToHandle(letters[i].x + devi.x, letters[i].y + devi.y, fontInfo.color, fontInfo.handle, letters[i].str.c_str());
	}
	else {
		Vector2 pos = globalTrs.position + devi;	// 描画座標
		DrawStringFToHandle(pos.x, pos.y, text.c_str(), fontInfo.color, fontInfo.handle);

		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		//DrawBoxAA(pos.x, pos.y, pos.x + textSize.x, pos.y + textSize.y, 0xFF0000, true);
	}

	if (needAfterImage) {

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, easingAlphaAftImg.current);

		scaledFont.size = static_cast<int>(fontInfo.size * easingScaleAftImg.current.Average());	// 拡大率を適応したフォントの大きさ

		// フォントデータを生成する
		Font::CreateFontToHandle(&scaledFont);

		// 現在のフォントの大きさと拡大率を適応したフォントの大きさを比較する
		Vector2 fontSizeDef = Vector2((scaledFont.size - fontInfo.size) * -5.f, (scaledFont.size - fontInfo.size) * -0.75f);
		devi = DisplacementByAnchorPoint();

		Vector2 pos = globalTrs.position + devi + fontSizeDef + easingMoveAftImg.current;	// 描画座標
		DrawStringFToHandle(pos.x, pos.y, text.c_str(), scaledFont.color, scaledFont.handle);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UI_Text::ScrollTextProcess() {

	scrollTime = max(scrollTime - GTime.deltaTime, 0.0f);

	// スクロールが完了した文字数
	const int textScroll = length - static_cast<int>(length * ScrollRate());

	// スクロール中かつ、まだスクロールされていないかつ、文字がスペースではない場合
	if (IsScroll() && afterScrollText < textScroll && text[textScroll] != 0x20) {
		afterScrollText = textScroll;
		SoundManager::Play(scrollSE, "TextScroll");
	}
}

void UI_Text::ApplySubtone() {

	// サブトーンが稼働していない場合
	if ((subtone != nullptr && subtone->IsActive()) == false)
		return;

	RectTransform globalTrs = rectTransform->Global();	// グローバルな座標・回転・拡縮情報
	Vector2 devi = DisplacementByAnchorPoint();	// アンカーポイントによる座標のズレ値

	Vector2 begin = Vector2::Zero;	// サブトーンの始点座標(左上)
	Vector2 end = Vector2::Zero;		// サブトーンの終点座標(右下)

	// サブトーンに画像が設定されている場合
	if (subtone->Image() != -1) {
		begin = globalTrs.position + Vector2(-textSize.x * 0.5f, -textSize.y);
		end = globalTrs.position + Vector2(textSize.x * 0.5f, textSize.y);
	}
	// サブトーンに画像が設定されていない場合
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

	text = _text;	// 文字列を変更
	length = Len();
	fontInfo = _font;

	Font::CreateFontToHandle(&fontInfo);	// フォント情報を変更

	textSize.x = static_cast<float>(GetDrawStringWidthToHandle(text.c_str(), Len(), fontInfo.handle));
	textSize.y = fontInfo.size;

	RectTransform globalTrs = rectTransform->Global();	// グローバルな座標・回転・拡縮情報
	Vector2 diff = Vector2::Zero;	// 原点から文字座標へのベクトル

	letters.clear();	// 保存していた文字を初期化

	// 一文字ずつ保存
	for (int i = 0; i < Len(); i++) {

		std::string letter;

		if (static_cast<unsigned char>(text[i]) >= 0x80) {
			letter = text.substr(i, 2);	// 全角なら2文字保存
			length--;
			i++;
		}
		else {
			letter = text.substr(i, 1);		// 半角なら1文字保存
		}

		letters.push_back(String::Letter(letter, globalTrs.position.x + diff.x, globalTrs.position.y + diff.y));

		// 次の文字の描画座標をずらす
		diff.x += GetDrawStringWidthToHandle(letter.c_str(), static_cast<int>(letter.length()), fontInfo.handle);

		// 改行の場合
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

	Vector2 offset = Vector2::Zero;	// 差

	// アンカーポイントによって横の中心点をズラす
	switch (Anchor::PlacementHorizontal(rectTransform->preset)) {
	case Anchor::Placement::LEFT:	offset.x = 0.0f;				break;
	case Anchor::Placement::MIDDLE:	offset.x = -textSize.x * 0.5f;	break;
	case Anchor::Placement::RIGHT:	offset.x = -textSize.x;			break;
	default:						offset.x = 0.0f;				break;
	}

	// アンカーポイントによって縦の中心点をズラす
	switch (Anchor::PlacementVertical(rectTransform->preset)) {
	case Anchor::Placement::UP:		offset.y = 0.0f;						break;
	case Anchor::Placement::MIDDLE:	offset.y = -textSize.y * 0.5f - ADJUST;	break;
	case Anchor::Placement::DOWN:	offset.y = -textSize.y;					break;
	default:						offset.y = 0.0f;						break;
	}

	return offset;
}
