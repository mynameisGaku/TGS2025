#include "src/common/game/GameManager.h"
#include "src/config/imgui/imguiConfig.h"
#include "vendor/ImGui/imgui.h"
#include <algorithm>

#include "src/util/time/GameTime.h" 

GameManager::GameManager()
{
    init();
}

GameManager::~GameManager()
{
}

void GameManager::Update()
{
    DrawImGuiDebugger();
}

void GameManager::DrawImGuiDebugger()
{
#ifdef IMGUI  
#ifdef _DEBUG  
    ImGui::Begin("Game Management");

    ImGui::Text("Current Game Mode: %s", m_CurrentGameMode.c_str());

    if (ImGui::BeginCombo("Select Game Mode", m_CurrentGameMode.c_str()))
    {
        for (const auto& modeName : m_GameModeNames)
        {
            bool isSelected = (m_CurrentGameMode == modeName);
            if (ImGui::Selectable(modeName.c_str(), isSelected))
            {
                m_CurrentGameMode = modeName;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Text("Current Stage Name: %s", m_CurrentStageName.c_str());

    if (ImGui::BeginCombo("Select Stage Name", m_CurrentStageName.c_str()))
    {
        for (const auto& stageName : m_StageNames)
        {
            bool isSelected = (m_CurrentStageName == stageName);
            if (ImGui::Selectable(stageName.c_str(), isSelected))
            {
                m_CurrentStageName = stageName;
            }
            if (isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::End();
#endif  
#endif  
}

GAME_MODE_DESC GameManager::GetCurrentGameModeData()
{
    return m_GameModeDescs[m_CurrentGameMode];
}

void GameManager::SetGameResult(const ResultData& data)
{
	m_ResultDataPrev = m_ResultData;
    m_ResultData = data;
}

std::string GameManager::GetCurrentStageName() const
{
    return m_CurrentStageName;
}

void GameManager::init()
{
    GAME_REF.Load();

    m_GameModeDescs     = GAME_REF.GameModeDescs;
    m_GameModeNames     = GAME_REF.GameModeNames;
    m_TeamNames         = GAME_REF.TeamNames;
    m_StageNames        = GAME_REF.StageNames;
    m_CurrentStageName  = GAME_REF.DefaultStageName;

    std::sort(m_GameModeNames.begin(), m_GameModeNames.end());
}