#pragma once
#include "src/common/performance_profiler/PerformanceProfiler.h"
#include <framework/gameobject.h>
#include <vector>

class PerformanceProfilerManager
{
private:
    static PerformanceProfilerManager* m_pInstance;
    PerformanceProfilerManager();
    ~PerformanceProfilerManager();
public:

    static PerformanceProfilerManager* GetInst();
    void Destroy();

    void Render();
    void OnEndFrame();

    void Add(PerformanceProfiler* profiler);
    void Delete(PerformanceProfiler* profiler);

private:
    std::vector<PerformanceProfiler*> m_Profilers;
};