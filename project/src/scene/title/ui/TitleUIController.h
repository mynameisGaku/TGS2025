#pragma once
#include <unordered_map>
#include <string>

#include <framework/gameObject.h>

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

	void TriggerEvent(const std::string& eventName);

private:
	TitleUIGridCursor*										m_pGridCursor		{};
	std::unordered_map<std::string, std::function<void()>>	m_EventHandlers		{};
	std::unordered_map<std::string, TitleUICanvas*>			m_CanvasList		{};
	TitleUICanvas*											m_pCurrentCanvas	{};
	bool													m_IsActive			{};
};