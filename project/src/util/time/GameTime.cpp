#include "GameTime.h"
#include <algorithm>
#include <deque>
#include <cmath>

GameTime* GameTime::instance = nullptr;

bool GameTime::m_Initialized = false;
std::chrono::high_resolution_clock::time_point GameTime::m_StartTime;
std::chrono::high_resolution_clock::time_point GameTime::m_LastTime;
std::chrono::high_resolution_clock::time_point GameTime::m_LevelLoadTime;

float GameTime::deltaTime = 0.0f;
float GameTime::m_DeltaTime = 0.0f;
float GameTime::m_UnscaledDeltaTime = 0.0f;
float GameTime::m_SmoothDeltaTime = 0.0f;
float GameTime::m_FixedDeltaTime = 1.0f / 60.0f; // 60Hz
float GameTime::timeScale = 1.0f;
float GameTime::m_MaximumDeltaTime = 0.3333f;
int   GameTime::m_FrameCount = 0;
bool  GameTime::m_InFixedTimeStep = false;

float GameTime::m_NextStepDeltaTime = 0.0f;
bool  GameTime::m_UseNextStepDeltaTime = false;
float GameTime::m_NextStepTimeScale = 0.0f;
bool  GameTime::m_UseNextStepTimeScale = false;

float GameTime::m_HitStopRemainingSec = 0.0f;

float GameTime::m_FixedAccumulator = 0.0f;
float GameTime::m_FixedAlpha = 0.0f;

static std::deque<float> deltaHistory;

void GameTime::initialize()
{
    m_StartTime = std::chrono::high_resolution_clock::now();
    m_LastTime = m_StartTime;
    m_LevelLoadTime = m_StartTime;
    m_Initialized = true;
    m_FrameCount = 0;
    m_SmoothDeltaTime = 0.0f;
    m_HitStopRemainingSec = 0.0f;
    m_FixedAccumulator = 0.0f;
    m_FixedAlpha = 0.0f;
}

void GameTime::Update()
{
    if (!m_Initialized)
        initialize();

    std::chrono::duration<float> delta;
    auto currentTime = std::chrono::high_resolution_clock::now();

    if (m_UseNextStepDeltaTime)
    {
        delta = std::chrono::duration<float>(m_NextStepDeltaTime);
        m_UseNextStepDeltaTime = false;
    }
    else
    {
        delta = currentTime - m_LastTime;
    }

    // Unscaled Δt(上限補正)
    m_UnscaledDeltaTime = std::min(delta.count(), m_MaximumDeltaTime);

    // HitStop 残り時間を減算(Unscaledで進める)
    m_HitStopRemainingSec = std::max(m_HitStopRemainingSec - m_UnscaledDeltaTime, 0.0f);

    // timeScale の決定
    const bool stepping = (m_UseNextStepTimeScale || m_UseNextStepDeltaTime);
    float scale = (m_UseNextStepTimeScale ? m_NextStepTimeScale : timeScale);
    m_UseNextStepTimeScale = false;

    // HitStop中はスケール0(ただし手動ステップ中は例外)
    if (IsHitStopping() && !stepping)
    {
        scale = 0.0f;
    }

    // Scaled Δt を反映
    m_DeltaTime = m_UnscaledDeltaTime * scale;
    deltaTime = m_DeltaTime;
    m_LastTime = currentTime;
    m_FrameCount++;

    // SmoothDeltaTime(移動平均)
    const size_t windowSize = 10;
    deltaHistory.push_back(m_DeltaTime);
    if (deltaHistory.size() > windowSize)
        deltaHistory.pop_front();

    float sum = 0.0f;
    for (float dt : deltaHistory)
        sum += dt;
    m_SmoothDeltaTime = sum / static_cast<float>(deltaHistory.size());

    // Fixed 蓄積(Scaled Δt を加算)
    m_FixedAccumulator += m_DeltaTime;
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

float GameTime::DeltaTime()
{
    return m_DeltaTime;
}

float GameTime::UnscaledDeltaTime()
{
    return m_UnscaledDeltaTime;
}

float GameTime::FixedDeltaTime()
{
    // 物理式などが「スケールされたΔt」を欲しい場合に利用
    return m_FixedDeltaTime * timeScale;
}

float GameTime::FixedUnscaledDeltaTime()
{
    // 物理ステップ自体は一定Δt
    return m_FixedDeltaTime;
}

float GameTime::TotalTime()
{
    return UnscaledTime() * timeScale;
}

int GameTime::TotalTimeInt()
{
    return static_cast<int>(TotalTime());
}

float GameTime::UnscaledTime()
{
    if (!m_Initialized)
        initialize();
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - m_StartTime;
    return elapsed.count();
}

float GameTime::RealtimeSinceStartup()
{
    return UnscaledTime();
}

float GameTime::TimeSinceLevelLoad()
{
    if (!m_Initialized)
        initialize();
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = now - m_LevelLoadTime;
    return elapsed.count() * timeScale;
}

int GameTime::FrameCount()
{
    return m_FrameCount;
}

bool GameTime::InFixedTimeStep()
{
    return m_InFixedTimeStep;
}

float GameTime::SmoothDeltaTime()
{
    return m_SmoothDeltaTime;
}

float GameTime::MaximumDeltaTime()
{
    return m_MaximumDeltaTime;
}

void GameTime::SetTimeScale(float scale)
{
    timeScale = std::max(0.0f, scale);
}

float GameTime::GetTimeScale()
{
    return timeScale;
}

void GameTime::SetFixedDeltaTime(float fixed)
{
    m_FixedDeltaTime = std::max(0.000001f, fixed);
}

void GameTime::SetMaximumDeltaTime(float max)
{
    m_MaximumDeltaTime = std::max(0.0f, max);
}

void GameTime::SetNextStepDeltaTime(float delta)
{
    m_NextStepDeltaTime = std::max(0.0f, delta);
    m_UseNextStepDeltaTime = true;
}

void GameTime::SetNextStepTimeScale(float scale)
{
    m_NextStepTimeScale = std::max(0.0f, scale);
    m_UseNextStepTimeScale = true;
}

void GameTime::SetHitStop(float seconds)
{
    SetHitStopSeconds(seconds);
}

void GameTime::SetHitStopSeconds(float seconds)
{
    m_HitStopRemainingSec = std::max(0.0f, seconds);
}

void GameTime::SetHitStopFrames(int frames)
{
    m_HitStopRemainingSec = std::max(0, frames) * m_FixedDeltaTime;
}

bool GameTime::IsHitStopping()
{
    return m_HitStopRemainingSec > 0.0f;
}

float GameTime::GetHitStopRemainingSeconds()
{
    return m_HitStopRemainingSec;
}

int GameTime::GetHitStopRemainingFrames()
{
    return static_cast<int>(std::ceil(m_HitStopRemainingSec / m_FixedDeltaTime));
}

int GameTime::RunFixedUpdateLoop(const std::function<void()>& fixedUpdate, int maxSteps)
{
    if (!fixedUpdate || maxSteps <= 0)
        return 0;

    int steps = 0;
    const float step = m_FixedDeltaTime;

    while (m_FixedAccumulator >= step && steps < maxSteps)
    {
        BeginFixedUpdate();
        fixedUpdate();
        EndFixedUpdate();

        m_FixedAccumulator -= step;
        steps++;
    }

    // 次のFixedまでの進捗(0~1)
    m_FixedAlpha = std::clamp(m_FixedAccumulator / step, 0.0f, 1.0f);
    return steps;
}

float GameTime::FixedInterpolationAlpha()
{
    return m_FixedAlpha;
}
