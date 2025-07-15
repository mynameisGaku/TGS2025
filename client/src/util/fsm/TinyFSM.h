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
/// <para> ���s����Ă���X�e�[�g�֑���M���B </para>
/// <para> FSM�̌��݂̏����󋵂����s���̃X�e�[�g�֓`�B�ł���B</para>
/// </summary>
enum FSMSignal
{
    SIG_Enter,          // �X�e�[�g�ύX��̏���t���[������
    SIG_Update,         // �X�V����
    SIG_AfterUpdate,    // �X�V������̍X�V����
    SIG_Exit,           // �X�e�[�g�ύX���ɓ��鏈��
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
/// �X�e�[�g��\���X�e�[�g�}�V���̃C���^�[�t�F�[�X
/// FSM��finite state machine�̗�
/// </summary>
/// <typeparam name="T">�g�p����N���X</typeparam>
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
    /// �X�V���s���B
    /// �X�e�[�g�̕ύX�͍X�V�ōs����
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

        // �ύX�\�񂪓����Ă���ꍇ
        if (m_needChange)
        {
            // �I������
            if (m_state)
            {
                m_signal = SIG_Exit;
                (m_owner->*m_state)(m_signal);
            }

            // �X�e�[�g�̓���ւ�
            m_statePrev = m_state;
            m_state = m_stateNext;
            m_stateNext = nullptr;

            // ���񏈗�
            if (m_state)
            {
                m_signal = SIG_Enter;
                (m_owner->*m_state)(m_signal);
            }

            // �\�񊮗��Ƃ��ăt���O���I�t��
            m_needChange = false;
        }

        // �ύX�\��Ȃ�(�ʏ�̍X�V)
        if (m_state)
        {
            m_signal = SIG_Update;
            (m_owner->*m_state)(m_signal);
        }

        // �ʏ�X�V��̍X�V
        if (m_state)
        {
            m_signal = SIG_AfterUpdate;
            (m_owner->*m_state)(m_signal);
        }
    }
    /// <summary>
    /// ImGui���g�p�����f�o�b�O��`��
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

            // �J�ڃ��O�̕\��
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
    /// ���݂��̃X�e�[�g�}�V�����ǂ̏����i�K��
    /// </summary>
    /// <returns></returns>
    FSMSignal GetSignal() const
    {
        return m_signal;
    }
    /// <summary>
    /// �X�e�[�g�̕ύX��\�񂷂�
    /// </summary>
    /// <param name="state">�ύX�������X�e�[�g�̊֐��|�C���^</param>
    void ChangeState(void(T::* state)(FSMSignal))
    {
        // ���łɃX�e�[�g�ύX�\�񂪓����Ă���ꍇ�͖���
        if (not m_needChange)
        {
            m_stateNext = state;
            m_needChange = true;

            // ���v���L���ȏꍇ�́A�e�X�e�[�g�̑J�ډ񐔂𐔂���
            if (m_isStatistics)
            {
                if (not m_transitCounters)
                {
                    m_transitCounters = new std::unordered_map<void(T::*)(FSMSignal), int>();
                }
                (*m_transitCounters)[state]++;

                // ���O���o�^�Ȃ�A�h���X������ŉ��o�^
                if (m_stateNameMap.find(state) == m_stateNameMap.end())
                {
                    char buf[64];
                    snprintf(buf, sizeof(buf), "State_%p", reinterpret_cast<void*>(*reinterpret_cast<uintptr_t*>(&state)));
                    m_stateNameMap[state] = buf;
                    m_stateNameMapKeyStr[buf] = state;
                }
            }

            // ���O�ɋL�^����
            Log log{};
            log.signal = m_signal;
            log.state = state;
            if (m_transitLog.size() >= LOGMAX)
                m_transitLog.pop_front();
            m_transitLog.push_back(log);
        }
    }

    /// <summary>
    /// �X�e�[�g�ύX string��
    /// RegisterStateName�֐��ɂ���ēo�^���ꂽ�X�e�[�g�ɑJ�ڂ���B
    /// �o�^����Ă��Ȃ��ꍇ�͑J�ڂ����A�G���[���O���o�͂��܂��B
    /// </summary>
    /// <param name="name">�o�^�����X�e�[�g��</param>
    void ChangeStateByName(const std::string& name)
    {
        if (m_stateNameMapKeyStr.find(name) == m_stateNameMapKeyStr.end())
            return;

        ChangeState(m_stateNameMapKeyStr[name]);
    }

    // ���݂̃X�e�[�g
    void(T::* GetCurrentState())(FSMSignal) { return m_state; }
    // ��O�̃X�e�[�g
    void(T::* GetPrevState())(FSMSignal) { return m_statePrev; }
    // �\�񂳂ꂽ�X�e�[�g
    void(T::* GetNextState())(FSMSignal) { return m_stateNext; }
    // ����̃X�e�[�g�ɑ΂���J�ډ񐔂��擾 (���v�������ɂȂ��Ă���ꍇ��0���Ԃ�)
    int GetTransitCount(void(T::* state)(FSMSignal))
    {
        if (not m_isStatistics)
            return 0;

        if (m_transitCounters)
            return 0;

        auto it = m_transitCounters->find(state);
        return (it != m_transitCounters->end()) ? it->second : 0;
    }

    // ���v�����悤�ɂ���BTinyFSM::RegisterStateName()�ŃX�e�[�g����o�^����悤�ɂ��Ă��������B
    void EnableStatistics() { m_isStatistics = true; }
    // ���v�����Ȃ��悤�ɂ���
    void DisableStatistics() { m_isStatistics = false; }
    // ���v������Ă��邩�ǂ���
    bool IsStatistics() const { return m_isStatistics; }
    // �w��̃X�e�[�g�ɖ��O��ݒ肷�� (�f�o�b�O�p)
    void RegisterStateName(void(T::* state)(FSMSignal), const std::string& name)
    {
        m_stateNameMap[state] = name;
        m_stateNameMapKeyStr[name] = state;
    }
    // �X�e�[�g�}�V���ɖ��O��t����
    void SetName(const std::string& name)
    {
        m_name = name;
    }
    // �ߋ�10��̑J�ڃ��O���m�F���� allowNumber�ŋ��e�񐔐ݒ�\
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
    // ���݁A�ǂ̒i�K�̏������s���Ă��邩
    FSMSignal m_signal;
    // �O��̃X�e�[�g
    void(T::* m_statePrev)(FSMSignal);
    // ���݂̃X�e�[�g
    void(T::* m_state)(FSMSignal);
    // ���̃X�e�[�g
    void(T::* m_stateNext)(FSMSignal);
    // �X�e�[�g�ύX���K�v��
    bool m_needChange;
    // ���L��
    T* m_owner;
    // ���̃X�e�[�g�}�V���̖��O
    std::string m_name;
    // �e�X�e�[�g�̑J�ډ�
    std::unordered_map<void(T::*)(FSMSignal), int>* m_transitCounters;
    // �e�X�e�[�g�̖��O (�f�o�b�O�p)
    std::unordered_map<void(T::*)(FSMSignal), std::string> m_stateNameMap;
    std::unordered_map<std::string, void(T::*)(FSMSignal)> m_stateNameMapKeyStr;
    // ���v����邩
    bool m_isStatistics;
    // �J�ڃ��O
    const UINT32 LOGMAX = 10;
    struct Log
    {
        void(T::* state)(FSMSignal);
        FSMSignal signal;
    };
    std::list<Log> m_transitLog;
};