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
		現在のアニメーションのフレームを取得
		各イベントが実行条件を満たしたか判定、満たしているなら実行
		*/

		float currentFrame = m_Animator->CurrentFrame();
		bool looped = currentFrame < m_LastFrame;

		for (const TimelineEvent& event : m_Events)
		{
			if (not m_Functions.contains(event.Name)) continue;

			if (looped)
			{
				// 終了時まで
				if (m_LastFrame <= event.StartFrame && event.StartFrame <= m_Animator->GetAnimInfo().endFrame)
				{
					(m_Owner->*m_Functions.at(event.Name))(event.Argument);
				}
				// 開始時から
				if (0 <= event.StartFrame && event.StartFrame <= currentFrame)
				{
					(m_Owner->*m_Functions.at(event.Name))(event.Argument);
				}
			}
			else
			{
				// 跨いだら
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
		Jsonを読み込んでJsonのまま保持
		*/

		m_Timelines.clear();

		// フォルダ内の全ファイルのファイル名を取得
		std::list<std::string> fileNames = Function::FindFileNames(folder, true);

		// 読み込み
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
		名前からJsonを参照、イベント名、引数、開始時間、終了時間を取得、イベントリストに登録
		アニメーション再生、タイムライン開始
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
		イベント名を指定して関数ポインタを紐づける
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
