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
		現在のアニメーションのフレームを取得
		各イベントが実行条件を満たしたか判定、満たしているなら実行
		*/

		float currentFrame = m_Animator->CurrentFrame();
		bool looped = currentFrame < m_LastFrame;


	}

	void Play(nlohmann::json timeline)
	{
		/*
		Jsonを受け取って、イベント名、引数、開始時間、終了時間を取得、イベントリストに登録
		アニメーション再生、タイムライン開始
		*/
	}

	void SetFunction(std::string eventName, void(T::* function)(nlohmann::json))
	{
		/*
		イベント名を指定して関数ポインタを紐づける
		*/
	}

private:
	T* m_Owner;
	Animator* m_Animator;
	std::list<TimeLineEvent> m_Events;
	std::unordered_map<std::string, void(T::* function)(nlohmann::json)> m_Functions;
	float m_LastFrame;
};
