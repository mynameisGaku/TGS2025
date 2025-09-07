#include "UI_ButtonHintRef.h"
#include <src/util/file/json/settings_json.h>
#include <src/util/string/StringUtil.h>

UI_ButtonHintRef* UI_ButtonHintRef::instance = nullptr;

UI_ButtonHintRef& UI_ButtonHintRef::Inst()
{
	if (!instance) instance = new UI_ButtonHintRef;
	return *instance;
}

void UI_ButtonHintRef::Destroy()
{
	delete instance;
	instance = nullptr;
}

void UI_ButtonHintRef::Load(bool forceLoad)
{
	if (!forceLoad && m_WasLoad) return;

	ButtonPositions.clear();
	
	auto jsonLoader = Settings_json::Inst();
	jsonLoader->LoadSettingJson(PATH, FILEKEY);
	
	auto& jsonData = jsonLoader->GetJson(FILEKEY);
	for (const auto& buttonData : jsonData["KeyDatas"])
	{
		ButtonPosition buttonPosition{};
		for (const auto& name : buttonData["KeyNames"])
		{
			buttonPosition.KeyNames.push_back(name);
		}

		buttonPosition.AnchorFromCenter.x = buttonData["AnchorFromCenter"].value("x", 0.0f);
		buttonPosition.AnchorFromCenter.y = buttonData["AnchorFromCenter"].value("y", 0.0f);
		buttonPosition.LocalPosition.x = buttonData["LocalPosition"].value("x", 0.0f);
		buttonPosition.LocalPosition.y = buttonData["LocalPosition"].value("y", 0.0f);

		buttonPosition.DisplayString = StringUtil::UTF8ToShiftJis(buttonData["DisplayString"].value("jp", "ERR")); 

		ButtonPositions.push_back(buttonPosition);
	}
	m_WasLoad = true;
}

UI_ButtonHintRef::ButtonPosition UI_ButtonHintRef::GetData(const std::string& key)
{
	for (const auto& buttonPosition : ButtonPositions)
	{
		for (auto& keyName : buttonPosition.KeyNames)
		{
			if (keyName == key)
			{
				return buttonPosition;
			}
		}
	}
	return ButtonPosition();
}
