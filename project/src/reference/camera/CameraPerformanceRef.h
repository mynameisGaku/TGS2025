#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "src/util/math/Vector3.h"

#include "src/common/camera/CameraDefine.h"

class CameraPerformanceRef
{
private:
	const std::string   FILEPATH = "data/Json/Camera/CameraPerformance.json";// ファイルのパス
	const std::string   FILEKEY = "CameraPerformance";                   // ファイル識別用キー

	static CameraPerformanceRef* instance;
public:
	static CameraPerformanceRef* Inst()
	{
		if (instance == nullptr)
		{
			instance = new CameraPerformanceRef;
		}
		return instance;
	}

	// " リファレンス解放 "
	// ↑を文字列検索したら着くところで解放してます。(おそらくメインループのすぐあと)
	void Destroy()
	{
		if (instance)
		{
			delete instance;
		}
		instance = nullptr;
	}

	void Load(bool ForceLoad = false);

	std::vector<CameraDefine::PerformanceData> GetPerfDatas(const std::string& name) const
	{
		if (m_PerfDataMap.contains(name))
			return m_PerfDataMap.at(name);
		else
			return std::vector<CameraDefine::PerformanceData>(); // 存在しない場合はデフォルト値を返す
	}

	std::unordered_map<std::string, std::vector<CameraDefine::PerformanceData>> m_PerfDataMap; // パフォーマンスデータのマップ

private:

	bool m_WasLoad; // ロード済みか？ 重複回避用

	CameraPerformanceRef() :
		m_PerfDataMap(),
		m_WasLoad(false)
	{
		Load(true); // 初期ロード
	}

	~CameraPerformanceRef()
	{ /*DO NOTHING*/
	}
};

#define CAMERA_PERFORMANCE_REF (*CameraPerformanceRef::Inst())