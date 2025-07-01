#pragma once
#include "src/util/singleton/singleton.h"

/// <summary>
/// マッチマネージャーのJsonパラメータを読み込む
/// </summary>
/// <author>佐藤紘斗</author>
class MatchRef
{
public:
	MatchRef() { Load(); }
	void Load();

	float GameOver_Duration;
};

#define MATCH_REF Singleton<MatchRef>::Instance()
