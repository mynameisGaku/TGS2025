// GameTime.cpp
#include "GameTime.h"
#include <algorithm>
#include <deque>

GameTime* GameTime::instance = nullptr;

bool GameTime::m_Initialized = false;
std::chrono::high_resolution_clock::time_point GameTime::m_StartTime;
std::chrono::high_resolution_clock::time_point GameTime::m_LastTime;
std::chrono::high_resolution_clock::time_point GameTime::m_LevelLoadTime;

float GameTime::deltaTime = 0.0f;
float GameTime::m_DeltaTime = 0.0f;
float GameTime::m_UnscaledDeltaTime = 0.0f;
float GameTime::m_SmoothDeltaTime = 0.0f;
float GameTime::m_FixedDeltaTime = 1.0f / 60.0f;
float GameTime::m_TimeScale = 1.0f;
float GameTime::m_MaximumDeltaTime = 0.3333f;
int GameTime::m_FrameCount = 0;
int GameTime::m_HitStop = 0;
bool GameTime::m_InFixedTimeStep = false;

static std::deque<float> deltaHistory;

void GameTime::initialize()
{
    m_StartTime = std::chrono::high_resolution_clock::now();
    m_LastTime = m_StartTime;
    m_LevelLoadTime = m_StartTime;
    m_Initialized = true;
    m_FrameCount = 0;
    m_SmoothDeltaTime = 0.0f;
    m_HitStop = 0;
}

void GameTime::Update()
{
    if (!m_Initialized)
        initialize();

    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = currentTime - m_LastTime;

    // deltaTime制限（最大値補正）
    m_UnscaledDeltaTime = std::min(delta.count(), m_MaximumDeltaTime);
    m_DeltaTime = m_UnscaledDeltaTime * m_TimeScale;
    deltaTime = m_DeltaTime;
    m_LastTime = currentTime;
    m_FrameCount++;

    // delta履歴から平均を出す（SmoothDeltaTime用）
    const size_t windowSize = 10;
    deltaHistory.push_back(m_DeltaTime);
    if (deltaHistory.size() > windowSize)
        deltaHistory.pop_front();

    float sum = 0.0f;
    for (float dt : deltaHistory) sum += dt;
    m_SmoothDeltaTime = sum / deltaHistory.size();

    m_HitStop = std::max(m_HitStop - 1, 0);
}

void GameTime::BeginFixedUpdate()
{
    m_InFixedTimeStep = true;
}

void GameTime::EndFixedUpdate()
{
    m_InFixedTimeStep = false;
}

void GameTime::SetLevelLoaded()
{
    if (!m_Initialized)
        initialize();
    m_LevelLoadTime = std::chrono::high_resolution_clock::now();
}

float GameTime::DeltaTime() { return m_DeltaTime; }
float GameTime::UnscaledDeltaTime() { return m_UnscaledDeltaTime; }
float GameTime::FixedDeltaTime() { return m_FixedDeltaTime; }
float GameTime::FixedUnscaledDeltaTime() { return m_FixedDeltaTime; }
float GameTime::TotalTime() { return UnscaledTime() * m_TimeScale; }

float GameTime::UnscaledTime()
{
    if (!m_Initialized) initialize();
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - m_StartTime;
    return elapsed.count();
}

float GameTime::RealtimeSinceStartup() { return UnscaledTime(); }

float GameTime::TimeSinceLevelLoad()
{
    if (!m_Initialized) initialize();
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - m_LevelLoadTime;
    return elapsed.count() * m_TimeScale;
}

int GameTime::FrameCount() { return m_FrameCount; }
bool GameTime::InFixedTimeStep() { return m_InFixedTimeStep; }
float GameTime::SmoothDeltaTime() { return m_SmoothDeltaTime; }
float GameTime::MaximumDeltaTime() { return m_MaximumDeltaTime; }

void GameTime::SetTimeScale(float scale) { m_TimeScale = scale; }
float GameTime::GetTimeScale() { return m_TimeScale; }
void GameTime::SetFixedDeltaTime(float fixed) { m_FixedDeltaTime = fixed; }
void GameTime::SetMaximumDeltaTime(float max) { m_MaximumDeltaTime = max; }

void GameTime::SetHitStop(int frame)
{
    if (m_HitStop < frame)
        m_HitStop = frame;
}

int GameTime::HitStop()
{
    return m_HitStop;
}

bool GameTime::IsHitStop()
{
    return (m_HitStop > 0);
}
