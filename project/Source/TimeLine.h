#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include <fstream>
#include "nlohmann/json.hpp"
#include "Component/Animator.h"
#include "Util/Utils.h"

struct TimelineEvent
{
	std::string Name;
	float StartFrame = 0.0f;
	float EndFrame = 0.0f;
	nlohmann::json Argument;
};

void to_json(nlohmann::json& j, const TimelineEvent& ev)
{

}

void from_json(const nlohmann::json& j, TimelineEvent& ev)
{
	j.at("Name").get_to(ev.Name);
	j.at("Start").get_to(ev.StartFrame);
	if (j.contains("End"))
		j.at("End").get_to(ev.EndFrame);
	else
		ev.EndFrame = ev.StartFrame;
	j.at("Argument").get_to(ev.Argument);
}

template <class T>
class Timeline
{
public:
	Timeline(T* owner, Animator* animator)
	{
		m_Owner = owner;
		m_Animator = animator;
		m_LastFrame = 0.0f;
	}

	~Timeline()
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

		for (const TimelineEvent& event : m_Events)
		{
			if (not m_Functions.contains(event.Name)) continue;

			if (looped)
			{
				// �I�����܂�
				if (m_LastFrame <= event.StartFrame && event.StartFrame <= m_Animator->GetAnimInfo().endFrame)
				{
					(m_Owner->*m_Functions.at(event.Name))(event.Argument);
				}
				// �J�n������
				if (0 <= event.StartFrame && event.StartFrame <= currentFrame)
				{
					(m_Owner->*m_Functions.at(event.Name))(event.Argument);
				}
			}
			else
			{
				// �ׂ�����
				if (m_LastFrame <= event.StartFrame && event.StartFrame <= currentFrame)
				{
					(m_Owner->*m_Functions.at(event.Name))(event.Argument);
				}
			}
		}

		m_LastFrame = currentFrame;
	}

	void LoadJsons(std::string folder)
	{
		/*
		Json��ǂݍ����Json�̂܂ܕێ�
		*/

		m_Timelines.clear();

		// �t�H���_���̑S�t�@�C���̃t�@�C�������擾
		std::list<std::string> fileNames = Function::FindFileNames(folder, true);

		// �ǂݍ���
		for (std::string fileName : fileNames)
		{
			nlohmann::json json;

			std::ifstream ifJson(folder + "/" + fileName + ".json");
			ifJson >> json;
			ifJson.close();

			m_Timelines.emplace(fileName, json);
		}
	}

	void Play(std::string name)
	{
		/*
		���O����Json���Q�ƁA�C�x���g���A�����A�J�n���ԁA�I�����Ԃ��擾�A�C�x���g���X�g�ɓo�^
		�A�j���[�V�����Đ��A�^�C�����C���J�n
		*/

		nlohmann::json timeline = m_Timelines.at(name);

		std::string animName = timeline.at("Animation").get<std::string>();
		m_Animator->Play(animName);

		m_Events = timeline.at("Events");
		m_LastFrame = 0.0f;
	}

	void SetFunction(std::string eventName, void(T::* function)(const nlohmann::json&))
	{
		/*
		�C�x���g�����w�肵�Ċ֐��|�C���^��R�Â���
		*/

		m_Functions.emplace(eventName, function);
	}

private:
	T* m_Owner;
	Animator* m_Animator;
	std::unordered_map<std::string, nlohmann::json> m_Timelines;
	std::unordered_map<std::string, void(T::*)(const nlohmann::json&)> m_Functions;

	std::list<TimelineEvent> m_Events;

	float m_LastFrame;
};
