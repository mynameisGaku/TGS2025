#include "UI_ButtonHint.h"
#include <src/util/enum/EnumUtil.h>
#include <src/util/screen/ScreenManager.h>
#include <src/util/file/resource_loader/resourceLoader.h>
#include <src/util/ui/UI_Manager.h>
#include <src/util/math/MathUtil.h>

UI_ButtonHint::UI_ButtonHint(const RectTransform& trs, int index)
{
	m_pButtonHintRef = &UI_ButtonHintRef::Inst();
	m_pButtonHintRef->Load();

	m_CharaIndex = index;

	Vector2 beginPos = ScreenManager::GetScreenBeginPos(m_CharaIndex);
	Vector2 endPos = ScreenManager::GetScreenEndPos(m_CharaIndex);

	RectTransform rectTrs = trs;
	rectTrs.anchor.SetBegin(beginPos);
	rectTrs.anchor.SetEnd(endPos);

	SetTransform(rectTrs);

	m_pInputRef = &InputRef::Inst();
	m_pInputRef->Load(true);

	// 画像のコピー & マップ登録
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

	m_Font.charSet = DEFAULT_CHARSET;	// 文字セット
	m_Font.size = 10;					// フォントサイズ
	m_Font.thick = 0;					// フォントの太さ
	m_Font.fontType = DX_FONTTYPE_NORMAL;	// フォントのタイプ
	m_Font.edgeSize = 0;				// 縁の大きさ
	m_Font.italic = 0;					// イタリック体
	m_Font.strData = FileUtil::Folder("data/font/", "sazanami_mincho.ttf", "さざなみ明朝");	// フォントの文字列データ
	m_hFont = Font::CreateFontToHandle(&m_Font);
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
	if (not m_pInputRef)
		return;
	UI_Canvas::Draw();
	const RectTransform globalTrs = rectTransform->Global();

	const float MaxScreenArea = 3840.0f + 1080.0f;
	float currentScreenArea = static_cast<float>(WindowSetting::Inst().width + WindowSetting::Inst().height);
	float scale = currentScreenArea / MaxScreenArea;

	MathUtil::AddIterationAssingRate(&scale, 2, 0.35f);
	scale = MathUtil::Clamp01(scale);

	for (auto& buttonHint : m_ButtonHints)
	{
		auto button = buttonHint.second;
		Vector2 pos = globalTrs.position + button.AnchorFromCenter + button.LocalPosition;

		if (not button.isActive)
			continue;

		if (not button.isPush)
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, button.ImageWidth, button.ImageHeight, globalTrs.scale.Average() * scale, 0.0f, button.hImage, true);
		else
			DrawRectRotaGraphF(pos.x, pos.y, 0, 0, button.ImageWidth, button.ImageHeight, globalTrs.scale.Average() * scale, 0.0f, button.hPushImage, true);

		int fsize = GetFontSizeToHandle(m_hFont);
		size_t length = button.DisplayString.length();
		float center = static_cast<float>(length) / 2.0f;
		float drawPosX = fsize * center;

		DrawFormatStringFToHandle(pos.x, pos.y + fsize * 1.8f, 0xffffff, m_hFont, button.DisplayString.c_str());
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
