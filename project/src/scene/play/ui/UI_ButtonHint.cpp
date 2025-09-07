#include "UI_ButtonHint.h"
#include <src/util/enum/EnumUtil.h>
#include <src/common/camera/CameraManager.h>
#include <src/util/file/resource_loader/resourceLoader.h>
#include <src/util/ui/UI_Manager.h>
#include <src/util/math/MathUtil.h>
#include <src/util/color/RGBColor.h>
#include <src/reference/ui/UI_ChatBarRef.h>

namespace
{
	static const float HEIGHT = 40.0f;	// •\Ž¦—Ìˆæ‚Ì‚‚³
	static const RGBColor BACK_COLOR = RGBColor(100);	// ”wŒiF
	static const float BUTTON_CHATBAR_OFFSET_X = 100.0f;
}

UI_ButtonHint::UI_ButtonHint(const RectTransform& trs, int index)
{
	m_pButtonHintRef = &UI_ButtonHintRef::Inst();
	m_pButtonHintRef->Load();

	m_CharaIndex = index;

	//Vector2 beginPos = CameraManager::GetDrawingAreaPos_CameraIndex(m_CharaIndex);
	//Vector2 endPos = beginPos + CameraManager::GetDrawingAreaSize_CameraIndex(m_CharaIndex);

	//RectTransform rectTrs = trs;
	//rectTrs.anchor.SetBegin(beginPos);
	//rectTrs.anchor.SetEnd(endPos);

	//SetTransform(rectTrs);

	SetTransform(trs);
	UI_Manager::SetAnchorPositionByScreenSplit(this, index);

	m_pInputRef = &InputRef::Inst();
	m_pInputRef->Load(true);

	// ‰æ‘œ‚ÌƒRƒs[ & ƒ}ƒbƒv“o˜^
	for (auto& phys : m_pInputRef->PhysicalKeys)
	{
		BUTTON button{};
		auto image_pair = InputManager::GetImagePair(phys.KeyName);
		button.hImage = image_pair.first;
		button.hPushImage = image_pair.second;
		GetGraphSize(button.hImage, &button.ImageWidth, &button.ImageHeight);
		button.isPush = false;
		button.isActive = false;
		auto data = m_pButtonHintRef->GetData(phys.KeyName);
		button.AnchorFromCenter = data.AnchorFromCenter;
		button.LocalPosition = data.LocalPosition;
		button.DisplayString = data.DisplayString;
		
		m_ButtonHints[phys.KeyName] = button;
	}

	m_Font = Font::BasicFont();
	m_Font.SetCharSet(DEFAULT_CHARSET).SetSize(14).SetFontType(DX_FONTTYPE_NORMAL);
	m_hFont = Font::Create(m_Font, "ButtonHint");
}

UI_ButtonHint::~UI_ButtonHint()
{
}

void UI_ButtonHint::Update()
{
	UI_Manager::SetAnchorPositionByScreenSplit(this, m_CharaIndex);

	UI_Canvas::Update();

	if (InputManager::Push("Reload", m_CharaIndex))
	{
		m_pButtonHintRef->Load(true);

		for (auto& phys : m_pInputRef->PhysicalKeys)
		{
			BUTTON button{};
			auto image_pair = InputManager::GetImagePair(phys.KeyName);
			button.hImage = image_pair.first;
			button.hPushImage = image_pair.second;
			GetGraphSize(button.hImage, &button.ImageWidth, &button.ImageHeight);
			button.isPush = false;
			button.isActive = false;
			auto data = m_pButtonHintRef->GetData(button.DisplayString);
			button.AnchorFromCenter = data.AnchorFromCenter;
			button.LocalPosition = data.LocalPosition;
			button.DisplayString = data.DisplayString;
			m_ButtonHints[phys.KeyName] = button;
		}
	}
}

void UI_ButtonHint::Draw()
{
	drawBack();

	if (not m_pInputRef)
		return;
	UI_Canvas::Draw();
	const RectTransform globalTrs = rectTransform->Global();

	//const float MaxScreenArea = 3840.0f + 1080.0f;
	//float currentScreenArea = static_cast<float>(WindowSetting::Inst().width + WindowSetting::Inst().height);
	//float scale = currentScreenArea / MaxScreenArea;
	float scale = 0.4f;

	MathUtil::AddIterationAssingRate(&scale, 2, 0.35f);
	scale = MathUtil::Clamp01(scale);

	for (auto& buttonHint : m_ButtonHints)
	{
		auto button = buttonHint.second;

		if (not button.isActive)
			continue;

		Vector2 pos = globalTrs.position + button.AnchorFromCenter + button.LocalPosition;
		pos.y -= HEIGHT / 2.0f;

		if (not button.isPush)
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, button.ImageWidth, button.ImageHeight, globalTrs.scale.Average() * scale, 0.0f, button.hImage, true);
		else
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, button.ImageWidth, button.ImageHeight, globalTrs.scale.Average() * scale, 0.0f, button.hPushImage, true);

		int fsize = GetFontSizeToHandle(m_hFont);
		size_t length = button.DisplayString.length();
		float center = static_cast<float>(length) / 2.0f;
		float drawPosX = fsize * center;

		DrawFormatStringFToHandle(pos.x + button.ImageWidth * scale * 0.5f, pos.y - fsize * 0.5f, 0xffffff, m_hFont, button.DisplayString.c_str());
	}

	Vector2 begin = CameraManager::GetDrawingAreaPos_CameraIndex(m_CharaIndex);
	Vector2 size = CameraManager::GetDrawingAreaSize_CameraIndex(m_CharaIndex);

	{
		auto button = m_ButtonHints["LeftTrigger"];

		Vector2 pos;
		pos.x = begin.x + size.x / 2.0f - UI_CHATBAR_REF.BackScale.x / 2.0f + BUTTON_CHATBAR_OFFSET_X;
		pos.y = begin.y + size.y + UI_CHATBAR_REF.PositionY;

		if (not button.isPush)
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, button.ImageWidth, button.ImageHeight, globalTrs.scale.Average() * scale, 0.0f, button.hImage, true);
		else
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, button.ImageWidth, button.ImageHeight, globalTrs.scale.Average() * scale, 0.0f, button.hPushImage, true);
	}

	{
		auto button = m_ButtonHints["RightTrigger"];

		Vector2 pos;
		pos.x = begin.x + size.x / 2.0f + UI_CHATBAR_REF.BackScale.x / 2.0f - BUTTON_CHATBAR_OFFSET_X;
		pos.y = begin.y + size.y + UI_CHATBAR_REF.PositionY;

		if (not button.isPush)
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, button.ImageWidth, button.ImageHeight, globalTrs.scale.Average() * scale, 0.0f, button.hImage, true);
		else
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, button.ImageWidth, button.ImageHeight, globalTrs.scale.Average() * scale, 0.0f, button.hPushImage, true);
	}
}

void UI_ButtonHint::PushKey(const std::string& key)
{
	if(not m_ButtonHints.contains(key))
		return;

	BUTTON& button = m_ButtonHints[key];
	
	if (not button.isActive)
		return;

	button.isPush = true;
}

void UI_ButtonHint::PushKey(const KeyDefine::KeyCode& code)
{
	std::string key = EnumUtil::ToString(code);
	PushKey(key);
}

void UI_ButtonHint::ReleaseKey(const std::string& key)
{
	if (not m_ButtonHints.contains(key))
		return;

	BUTTON& button = m_ButtonHints[key];

	if (not button.isActive)
		return;

	button.isPush = false;
}

void UI_ButtonHint::ReleaseKey(const KeyDefine::KeyCode& code)
{
	std::string key = EnumUtil::ToString(code);
	ReleaseKey(key);
}

void UI_ButtonHint::Activate(const std::string& key)
{
	if (not m_ButtonHints.contains(key))
		return;
	BUTTON& button = m_ButtonHints[key];
	button.isActive = true;
	button.isPush = false;
}

void UI_ButtonHint::Deactivate(const std::string& key)
{
	if (not m_ButtonHints.contains(key))
		return;
	BUTTON& button = m_ButtonHints[key];
	button.isActive = false;
	button.isPush = false;
}

void UI_ButtonHint::drawBack()
{
	Vector2 pos = CameraManager::GetDrawingAreaPos_CameraIndex(m_CharaIndex);
	Vector2 size = CameraManager::GetDrawingAreaSize_CameraIndex(m_CharaIndex);

	DrawBox(pos.x, pos.y + size.y - HEIGHT, pos.x + size.x, pos.y + size.y, BACK_COLOR.GetColorInt(), true); // ”wŒi‚ð•`‰æ
}
