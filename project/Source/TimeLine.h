#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include "nlohmann/json.hpp"
#include "Component/Animator.h"

struct TimeLineEvent
{
	std::string Name;
	float StartFrame;
	float EndFrame;
	nlohmann::json Argument;
};

void to_json(nlohmann::json& j, const TimeLineEvent& ev)
{

}

void from_json(const nlohmann::json& j, TimeLineEvent& ev)
{

}

template <class T>
class TimeLine
{
public:
	TimeLine(T* owner)
	{
		m_Owner = owner;
		m_Animator = nullptr;
		m_LastFrame = 0.0f;
	}

	~TimeLine()
	{

	}

	void Update()
	{
		/*
		���݂̃A�j���[�V�����̃t���[�����擾
		�e�C�x���g�����s�����𖞂�����������A�������Ă���Ȃ���s
		*/

		float currentFrame = m_Animator->CurrentFrame();
		bool looped = currentFrame < m_LastFrame;


	}

	void Play(nlohmann::json timeline)
	{
		/*
		Json���󂯎���āA�C�x���g���A�����A�J�n���ԁA�I�����Ԃ��擾�A�C�x���g���X�g�ɓo�^
		�A�j���[�V�����Đ��A�^�C�����C���J�n
		*/
	}

	void SetFunction(std::string eventName, void(T::* function)(nlohmann::json))
	{
		/*
		�C�x���g�����w�肵�Ċ֐��|�C���^��R�Â���
		*/
	}

private:
	T* m_Owner;
	Animator* m_Animator;
	std::list<TimeLineEvent> m_Events;
	std::unordered_map<std::string, void(T::* function)(nlohmann::json)> m_Functions;
	float m_LastFrame;
};
