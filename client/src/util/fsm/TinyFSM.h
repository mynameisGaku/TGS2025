#pragma once
#include <unordered_map>
#include <string>
#include <vendor/imgui/imgui.h>
#include <list>
#include <basetsd.h>
#include <cstdio>
#include <memory>
#include <type_traits>
#include <vadefs.h>

/// <summary>
/// <para> 実行されているステートへ送る信号。 </para>
/// <para> FSMの現在の処理状況を実行中のステートへ伝達できる。</para>
/// </summary>
enum FSMSignal
{
    SIG_Enter,          // ステート変更後の初回フレーム処理
    SIG_Update,         // 更新処理
    SIG_AfterUpdate,    // 更新処理後の更新処理
    SIG_Exit,           // ステート変更時に入る処理
};

namespace std
{
    template<typename T>
    struct hash<void(T::*)(FSMSignal)>
    {
        size_t operator()(void(T::* ptr)(FSMSignal)) const noexcept
        {
            return std::hash<std::uintptr_t>()(reinterpret_cast<std::uintptr_t>(*(void**)(&ptr)));
        }
    };
}

/// <summary>
/// ステートを表すステートマシンのインターフェース
/// FSMはfinite state machineの略
/// </summary>
/// <typeparam name="T">使用するクラス</typeparam>
template<typename T>
class TinyFSM
{
public:
    TinyFSM(T* owner) :
        m_state(nullptr),
        m_statePrev(nullptr),
        m_stateNext(nullptr),
        m_signal(SIG_Enter),
        m_needChange(false),
        m_owner(owner),
        m_name("No Setting"),
        m_transitCounters(nullptr)
    {
#if _DEBUG
        m_isStatistics = true;
#else
        m_isStatistics = false;
#endif
    }
    ~TinyFSM()
    {
        if (m_transitCounters)
        {
            delete m_transitCounters;
            m_transitCounters = nullptr;
        }
        m_needChange = false;
        m_state = nullptr;
        m_statePrev = nullptr;
        m_stateNext = nullptr;
        m_signal = SIG_Exit;
    }
    /// <summary>
    /// 更新を行う。
    /// ステートの変更は更新で行われる
    /// </summary>
    void Update()
    {
        if (m_isStatistics)
        {
            if (not m_transitCounters)
            {
                m_transitCounters = new std::unordered_map<void(T::*)(FSMSignal), int>();
            }
        }
        else
        {
            if (m_transitCounters)
            {
                delete m_transitCounters;
                m_transitCounters = nullptr;
            }
        }

        // 変更予約が入っている場合
        if (m_needChange)
        {
            // 終了処理
            if (m_state)
            {
                m_signal = SIG_Exit;
                (m_owner->*m_state)(m_signal);
            }

            // ステートの入れ替え
            m_statePrev = m_state;
            m_state = m_stateNext;
            m_stateNext = nullptr;

            // 初回処理
            if (m_state)
            {
                m_signal = SIG_Enter;
                (m_owner->*m_state)(m_signal);
            }

            // 予約完了としてフラグをオフる
            m_needChange = false;
        }

        // 変更予約なし(通常の更新)
        if (m_state)
        {
            m_signal = SIG_Update;
            (m_owner->*m_state)(m_signal);
        }

        // 通常更新後の更新
        if (m_state)
        {
            m_signal = SIG_AfterUpdate;
            (m_owner->*m_state)(m_signal);
        }
    }
    /// <summary>
    /// ImGuiを使用したデバッグを描画
    /// </summary>
    void ImGuiDebugRender()
    {
#if _DEBUG
        std::string name = "TinyFSM [" + m_name + "] Debug";
        if (ImGui::TreeNode(name.c_str()))
        {
            auto GetStateName = [this](void(T::* state)(FSMSignal)) -> const char*
                {
                    if (!state) return "(nullptr)";
                    auto it = m_stateNameMap.find(state);
                    if (it != m_stateNameMap.end()) return it->second.c_str();
                    static char buf[64];
                    snprintf(buf, sizeof(buf), "Unregistered_%p", reinterpret_cast<void*>(*reinterpret_cast<uintptr_t*>(&state)));
                    return buf;
                };

            ImGui::Text("Current State: %s", GetStateName(m_state));
            ImGui::Text("Previous State: %s", GetStateName(m_statePrev));
            ImGui::Text("Next State: %s", GetStateName(m_stateNext));

            const char* signalStr = "Unknown";
            switch (m_signal)
            {
            case SIG_Enter:        signalStr = "SIG_Enter"; break;
            case SIG_Update:       signalStr = "SIG_Update"; break;
            case SIG_AfterUpdate:  signalStr = "SIG_AfterUpdate"; break;
            case SIG_Exit:         signalStr = "SIG_Exit"; break;
            }
            ImGui::Text("Current Signal: %s", signalStr);

            if (m_isStatistics && m_transitCounters)
            {
                if (ImGui::TreeNode("Transition Counts"))
                {
                    for (const auto& pair : *m_transitCounters)
                    {
                        const char* name = GetStateName(pair.first);
                        ImGui::Text("%s: %d", name, pair.second);
                    }
                    ImGui::TreePop();
                }
            }

            // 遷移ログの表示
            if (ImGui::TreeNode("Transition Log"))
            {
                for (const auto& log : m_transitLog)
                {
                    const char* stateName = GetStateName(log.state);
                    ImGui::Text("State: %s, Signal: %s", stateName, signalStr);
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
#endif
    }
    /// <summary>
    /// 現在このステートマシンがどの処理段階か
    /// </summary>
    /// <returns></returns>
    FSMSignal GetSignal() const
    {
        return m_signal;
    }
    /// <summary>
    /// ステートの変更を予約する
    /// </summary>
    /// <param name="state">変更したいステートの関数ポインタ</param>
    void ChangeState(void(T::* state)(FSMSignal))
    {
        // すでにステート変更予約が入っている場合は無視
        if (not m_needChange)
        {
            m_stateNext = state;
            m_needChange = true;

            // 統計が有効な場合は、各ステートの遷移回数を数える
            if (m_isStatistics)
            {
                if (not m_transitCounters)
                {
                    m_transitCounters = new std::unordered_map<void(T::*)(FSMSignal), int>();
                }
                (*m_transitCounters)[state]++;

                // 名前未登録ならアドレス文字列で仮登録
                if (m_stateNameMap.find(state) == m_stateNameMap.end())
                {
                    char buf[64];
                    snprintf(buf, sizeof(buf), "State_%p", reinterpret_cast<void*>(*reinterpret_cast<uintptr_t*>(&state)));
                    m_stateNameMap[state] = buf;
                    m_stateNameMapKeyStr[buf] = state;
                }
            }

            // ログに記録する
            Log log{};
            log.signal = m_signal;
            log.state = state;
            if (m_transitLog.size() >= LOGMAX)
                m_transitLog.pop_front();
            m_transitLog.push_back(log);
        }
    }

    /// <summary>
    /// ステート変更 string版
    /// RegisterStateName関数によって登録されたステートに遷移する。
    /// 登録されていない場合は遷移せず、エラーログを出力します。
    /// </summary>
    /// <param name="name">登録したステート名</param>
    void ChangeStateByName(const std::string& name)
    {
        if (m_stateNameMapKeyStr.find(name) == m_stateNameMapKeyStr.end())
            return;

        ChangeState(m_stateNameMapKeyStr[name]);
    }

    // 現在のステート
    void(T::* GetCurrentState())(FSMSignal) { return m_state; }
    // 一つ前のステート
    void(T::* GetPrevState())(FSMSignal) { return m_statePrev; }
    // 予約されたステート
    void(T::* GetNextState())(FSMSignal) { return m_stateNext; }
    // 特定のステートに対する遷移回数を取得 (統計が無効になっている場合は0が返る)
    int GetTransitCount(void(T::* state)(FSMSignal))
    {
        if (not m_isStatistics)
            return 0;

        if (m_transitCounters)
            return 0;

        auto it = m_transitCounters->find(state);
        return (it != m_transitCounters->end()) ? it->second : 0;
    }

    // 統計を取るようにする。TinyFSM::RegisterStateName()でステート名を登録するようにしてください。
    void EnableStatistics() { m_isStatistics = true; }
    // 統計を取らないようにする
    void DisableStatistics() { m_isStatistics = false; }
    // 統計を取っているかどうか
    bool IsStatistics() const { return m_isStatistics; }
    // 指定のステートに名前を設定する (デバッグ用)
    void RegisterStateName(void(T::* state)(FSMSignal), const std::string& name)
    {
        m_stateNameMap[state] = name;
        m_stateNameMapKeyStr[name] = state;
    }
    // ステートマシンに名前を付ける
    void SetName(const std::string& name)
    {
        m_name = name;
    }
    // 過去10回の遷移ログを確認する allowNumberで許容回数設定可能
    bool HasTransitionWithInThePast10Times(void(T::* state)(FSMSignal), int allowNumber)
    {
        int count = 0;
        for (const auto& log : m_transitLog)
        {
            if (log.state == state && count > allowNumber)
            {
                return true;
            }
            count++;
        }
        return false;
    }
    bool HasTransitionWithInThePast(void(T::* state)(FSMSignal), int times) const
    {
        int count = 0;
        for (const auto& log : m_transitLog)
        {
            if (count >= times)
                return false;

            if (log.state == state)
            {
                return true;
            }
            count++;
        }
        return false;
    }

    std::string GetStateNameFromMap(void(T::* state)(FSMSignal))
    {
        return m_stateNameMap[state];
    }

private:
    // 現在、どの段階の処理を行っているか
    FSMSignal m_signal;
    // 前回のステート
    void(T::* m_statePrev)(FSMSignal);
    // 現在のステート
    void(T::* m_state)(FSMSignal);
    // 次のステート
    void(T::* m_stateNext)(FSMSignal);
    // ステート変更が必要か
    bool m_needChange;
    // 所有者
    T* m_owner;
    // このステートマシンの名前
    std::string m_name;
    // 各ステートの遷移回数
    std::unordered_map<void(T::*)(FSMSignal), int>* m_transitCounters;
    // 各ステートの名前 (デバッグ用)
    std::unordered_map<void(T::*)(FSMSignal), std::string> m_stateNameMap;
    std::unordered_map<std::string, void(T::*)(FSMSignal)> m_stateNameMapKeyStr;
    // 統計を取るか
    bool m_isStatistics;
    // 遷移ログ
    const UINT32 LOGMAX = 10;
    struct Log
    {
        void(T::* state)(FSMSignal);
        FSMSignal signal;
    };
    std::list<Log> m_transitLog;
};