#pragma once
#include <framework/gameObject.h>
#include <string>

class PerformanceProfiler
{
public:
    PerformanceProfiler(const std::string& name);
    ~PerformanceProfiler();

    void BeginProfiling();
    void EndProfiling();
    void CalcPerformance();
    double GetResult();
    std::string GetName() const;
    void Activate() { m_IsActive = true; }
    void Deactivate() { m_IsActive = false; }
    bool IsActive() const { return m_IsActive; }
private:
    LARGE_INTEGER   m_Freq{};
    LARGE_INTEGER   m_Start{};
    LARGE_INTEGER   m_End{};
    int             m_TimeRecordCounter   = 0;
    double          m_Tick                = 0.0;
    bool            m_IsActive            = false;
    std::string     m_Name="";
};