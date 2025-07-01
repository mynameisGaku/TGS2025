#include "UI_ButtonHint.h"
#include "src/util/enum/EnumUtil.h"
#include <src/util/screen/ScreenManager.h>
#include <src/util/file/resource_loader/resourceLoader.h>

UI_ButtonHint::UI_ButtonHint(const RectTransform& trs, int index)
{
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
		button.isPush = false;
		button.isActive = false;
		m_ButtonHints[phys.KeyName] = button;
	}
}

UI_ButtonHint::~UI_ButtonHint()
{
}

void UI_ButtonHint::Draw()
{
	if (not m_pInputRef)
		return;
	UI_Canvas::Draw();
	const RectTransform globalTrs = rectTransform->Global();
	for (auto& buttonHint : m_ButtonHints)
	{
		auto button = buttonHint.second;

		if (not button.isActive)
			continue;

		if (not button.isPush)
			DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, button.hImage, true);
		else
			DrawRotaGraphF(globalTrs.position.x, globalTrs.position.y, globalTrs.scale.Average(), 0.0f, button.hPushImage, true);
	}
}

void UI_ButtonHint::Push(const std::string& key)
{
	if(not m_ButtonHints.contains(key))
		return;

	BUTTON& button = m_ButtonHints[key];
	
	if (not button.isActive)
		return;

	button.isPush = true;
}

void UI_ButtonHint::Push(const KeyDefine::KeyCode& code)
{
	std::string key = EnumUtil::ToString(code);
	Push(key);
}

void UI_ButtonHint::Release(const std::string& key)
{
	if (not m_ButtonHints.contains(key))
		return;

	BUTTON& button = m_ButtonHints[key];

	if (not button.isActive)
		return;

	button.isPush = false;
}

void UI_ButtonHint::Release(const KeyDefine::KeyCode& code)
{
	std::string key = EnumUtil::ToString(code);
	Release(key);
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
