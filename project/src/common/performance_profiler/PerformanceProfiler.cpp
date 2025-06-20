#include "src/common/performance_profiler/PerformanceProfiler.h"
#include "src/common/performance_profiler/PerformanceProfilerManager Manager.h"

PerformanceProfiler::PerformanceProfiler(const std::string& name)
{
	m_Name = name;
	PerformanceProfilerManager::GetInst()->Add(this);
}

PerformanceProfiler::~PerformanceProfiler()
{
	PerformanceProfilerManager::GetInst()->Delete(this);
}

void PerformanceProfiler::BeginProfiling()
{
	QueryPerformanceFrequency(&m_Freq);
	QueryPerformanceCounter(&m_Start);
}

void PerformanceProfiler::EndProfiling()
{
	QueryPerformanceCounter(&m_End);
}

void PerformanceProfiler::CalcPerformance()
{
	if (++m_TimeRecordCounter % 60 == 0)
		m_Tick = static_cast<double>(m_End.QuadPart - m_Start.QuadPart) * 1000.0 / m_Freq.QuadPart;
}

double PerformanceProfiler::GetResult()
{
	return m_Tick;
}

std::string PerformanceProfiler::GetName() const
{
	return m_Name;
}
