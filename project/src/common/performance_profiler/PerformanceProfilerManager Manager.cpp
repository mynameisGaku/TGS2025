#include "src/common/performance_profiler/PerformanceProfilerManager Manager.h"
#include "vendor/ImGui/imgui.h"

PerformanceProfilerManager* PerformanceProfilerManager::m_pInstance = nullptr;

PerformanceProfilerManager::PerformanceProfilerManager()
{
}

PerformanceProfilerManager::~PerformanceProfilerManager()
{
}

PerformanceProfilerManager* PerformanceProfilerManager::GetInst()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new PerformanceProfilerManager;
    }
    return m_pInstance;
}

void PerformanceProfilerManager::Destroy()
{
    if (m_pInstance != nullptr)
    {
        delete m_pInstance;
    }
    m_pInstance = NULL;
}

void PerformanceProfilerManager::Render()
{
    ImGui::Begin("Performance Profile Debugger");
    for (auto& it : m_Profilers)
    {
        if (ImGui::TreeNode(it->GetName().c_str()))
        {
            ImGui::Text("Tick: %lf ms", it->GetResult());
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void PerformanceProfilerManager::OnEndFrame()
{
    for (auto& it : m_Profilers)
    {
        it->CalcPerformance();
    }
}

void PerformanceProfilerManager::Add(PerformanceProfiler* profiler)
{
    m_Profilers.push_back(profiler);
}

void PerformanceProfilerManager::Delete(PerformanceProfiler* profiler)
{
    for (auto it = m_Profilers.begin(); it != m_Profilers.end(); )
    {
        auto p = (*it);
        if (p != profiler)
        {
            it++;
            continue;
        }

        it = m_Profilers.erase(it);
        break;
    }
}
