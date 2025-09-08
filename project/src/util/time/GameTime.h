#pragma once
#include <chrono>
#include <functional>

/// <summary>
/// UnityのTimeクラスに基づいた時間管理クラス
/// </summary>
class GameTime
{
public:

    enum class AdditionMethod
    {
        None = 0,   // 加算しない
        Usual,      // 加算する(時間経過率を含まない)
        Rate        // 加算する(時間経過率を含む)
    };

    static void Update();

    static void BeginFixedUpdate();

    static void EndFixedUpdate();

    static void SetLevelLoaded();

    static float DeltaTime();

    static float UnscaledDeltaTime();

    static float FixedDeltaTime();

    static float FixedUnscaledDeltaTime();

    static float TotalTime();

    static int   TotalTimeInt();

    static float UnscaledTime();

    static float RealtimeSinceStartup();

    static float TimeSinceLevelLoad();

    static int FrameCount();

    static bool InFixedTimeStep();

    static float SmoothDeltaTime();

    static float MaximumDeltaTime();

    static void SetTimeScale(float scale);

    static float GetTimeScale();

    static void SetFixedDeltaTime(float fixedDeltaSeconds);

    static void SetMaximumDeltaTime(float maxDelta);

    static void SetNextStepDeltaTime(float deltaSeconds);

    static void SetNextStepTimeScale(float scale);

    static void SetHitStop(float seconds);

    static void SetHitStopSeconds(float seconds);

    static void SetHitStopFrames(int frames);

    static bool IsHitStopping();

    static float GetHitStopRemainingSeconds();

    static int   GetHitStopRemainingFrames();

    /// <summary>
    /// 固定更新をまとめて実行。
    /// -Updateで蓄積した時間から、固定dtずつ最大maxSteps回まで呼び出す。
    /// -戻り値:行したFixedステップ回数
    /// </summary>
    static int RunFixedUpdateLoop(const std::function<void()>& fixedUpdate, int maxSteps = 4);

    /// <summary>
    /// Fixed補間係数(0~1)
    /// </summary>
    static float FixedInterpolationAlpha();

    static float deltaTime;
    static float timeScale;

    static GameTime& Instance()
    {
        if (instance == nullptr)
        {
            instance = new GameTime();
        }
        return (*instance);
    }

    void Destroy()
    {
        if (instance)
        {
            delete instance;
        }
        instance = nullptr;
    }

    //==================================================
    // ▼変換

    template<typename Ty = float>
    inline float FrameToSec(Ty frame)
    {
        return static_cast<float>(frame) / 60.0f;
    }

    template<typename Ty = float>
    inline float FrameToMin(Ty frame)
    {
        return static_cast<float>(frame) / 60.0f * 60.0f;
    }

    template<typename Ty = float>
    inline float FrameToHour(Ty frame)
    {
        return static_cast<float>(frame) / 3600.0f * 60.0f;
    }

    template<typename Ty = float>
    inline float SecToFrame(Ty sec)
    {
        return static_cast<float>(sec) * 60.0f;
    }

    template<typename Ty = float>
    inline float MinToFrame(Ty min)
    {
        return static_cast<float>(min) * 60.0f * 60.0f;
    }

    template<typename Ty = float>
    inline float HourToFrame(Ty hour)
    {
        return static_cast<float>(hour) * 60.0f * 60.0f * 60.0f;
    }

private:

    GameTime() = default;
    ~GameTime() = default;

    static GameTime* instance;

    static void initialize();
    static bool m_Initialized;

    static std::chrono::high_resolution_clock::time_point m_StartTime;
    static std::chrono::high_resolution_clock::time_point m_LastTime;
    static std::chrono::high_resolution_clock::time_point m_LevelLoadTime;

    static float m_DeltaTime;
    static float m_UnscaledDeltaTime;
    static float m_SmoothDeltaTime;
    static float m_FixedDeltaTime;
    static float m_MaximumDeltaTime;
    static int   m_FrameCount;
    static bool  m_InFixedTimeStep;

    static float m_NextStepDeltaTime;
    static bool  m_UseNextStepDeltaTime;
    static float m_NextStepTimeScale;
    static bool  m_UseNextStepTimeScale;

    static float m_HitStopRemainingSec;
    
    static float m_FixedAccumulator;
    static float m_FixedAlpha;
};

#define GTime GameTime::Instance()
