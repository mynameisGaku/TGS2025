#pragma once
#include <unordered_map>
#include <string>

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
		m_name(""),
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
	void ChangeState(void(T::*state)(FSMSignal)) 
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
			}
		}
	}

	// ���݂̃X�e�[�g
	void(T::*GetCurrentState())(FSMSignal) { return m_state; }
	// ��O�̃X�e�[�g
	void(T::*GetPrevState())(FSMSignal) { return m_statePrev; }
	// �\�񂳂ꂽ�X�e�[�g
	void(T::*GetNextState())(FSMSignal) { return m_stateNext; }
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

	// ���v�����悤�ɂ���
	void EnableStatistics() { m_isStatistics = true; }
	// ���v�����Ȃ��悤�ɂ���
	void DisableStatistics() { m_isStatistics = false; }
	// ���v������Ă��邩�ǂ���
	bool IsStatistics() const { return m_isStatistics; }

private:
	// ���݁A�ǂ̒i�K�̏������s���Ă��邩
	FSMSignal m_signal;
	// �O��̃X�e�[�g
	void(T::*m_statePrev)(FSMSignal);
	// ���݂̃X�e�[�g
	void(T::*m_state)(FSMSignal);
	// ���̃X�e�[�g
	void(T::*m_stateNext)(FSMSignal);
	// �X�e�[�g�ύX���K�v��
	bool m_needChange;
	// ���L��
	T* m_owner;
	// ���̃X�e�[�g�}�V���̖��O
	std::string m_name;
	// �e�X�e�[�g�̑J�ډ�
	std::unordered_map<void(T::*)(FSMSignal), int>* m_transitCounters;
	// ���v����邩
	bool m_isStatistics;
};