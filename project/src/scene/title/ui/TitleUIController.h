#pragma once
#include <unordered_map>
#include <string>

#include <framework/gameObject.h>
#include <vendor/nlohmann/json.hpp>
#include <functional>
#include "TitleUI.h"

class TitleUIGridCursor;
class TitleUICanvas;
class TitleUI;

class TitleUIController : public GameObject
{
public:
	TitleUIController();
	~TitleUIController();

	void LoadCanvasFromJson(const std::string& file);
	void LoadCanvasesFromJson(const std::string& path);

	void Update() override;
	void Draw() override;

	void Activate()		{ m_IsActive = true; }
	void Deactivate()	{ m_IsActive = false; }

	TitleUIGridCursor* GetGridCursor() const { return m_pGridCursor; }

	void TriggerEvent(TUI_EVENT& event, const nlohmann::json& argument);

private:
	void subscribeFunctions();

	void activateCanvas(nlohmann::json argument);
	void scaling(nlohmann::json argument);
	void gameStart(nlohmann::json argument);
	void exit(nlohmann::json argument);
	void setWindowMode(nlohmann::json argument);

	TitleUIGridCursor*																m_pGridCursor		{};
	std::unordered_map<std::string, std::function<void(const nlohmann::json&)>>		m_EventHandlers		{};
	std::unordered_map<std::string, TitleUICanvas*>									m_CanvasList		{};
	TitleUICanvas*																	m_pCurrentCanvas	{};
	bool																			m_IsActive			{};
};

#define SUBSCRIBE_FUNCTION(key, func) m_EventHandlers[key] = [this](const nlohmann::json& argument){this->func(argument);};