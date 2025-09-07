#include "src/reference/camera/CameraPerformanceRef.h"
#include "src/util/file/json/settings_json.h"
#include "src/util/math/MathUtil.h"

#include "vendor/magic_enum/magic_enum.hpp"

CameraPerformanceRef* CameraPerformanceRef::instance = nullptr;

using namespace CameraDefine;

void CameraPerformanceRef::Load(bool ForceLoad)
{
	if (not ForceLoad && m_WasLoad)
	{
		return;
	}

	// 諸々の初期化
	auto jsonLoader = Settings_json::Inst();

	// JSON 読み込み
	jsonLoader->LoadSettingJson(FILEPATH, FILEKEY, true);

	// 各種値取得
	{
		std::vector<PerformanceData>& prefDataList = m_PerfDataMap["ResultScene"];
		prefDataList.clear(); // クリアしておく

		std::string JsonDataName = "ResultScene.PerfData_";
		int index = 0;

		std::string jsonKey = JsonDataName + std::to_string(index);

		while (jsonLoader->GetOrDefault(jsonKey + ".CheckData", false, FILEKEY))
		{
			PerformanceData perfData_ResultSccene;
			perfData_ResultSccene.position		= jsonLoader->GetOrDefault(jsonKey + ".Position",		Vector3(0, 0, 0),	FILEKEY);
			perfData_ResultSccene.target		= jsonLoader->GetOrDefault(jsonKey + ".Target",			Vector3(0, 0, 0),	FILEKEY);
			perfData_ResultSccene.afterTaste	= jsonLoader->GetOrDefault(jsonKey + ".AfterTaste",		0.0f,				FILEKEY);
			perfData_ResultSccene.holder		= jsonLoader->GetOrDefault(jsonKey + ".Holder",			std::string(),		FILEKEY);
			
			perfData_ResultSccene.easingInfo.totaltime		= jsonLoader->GetOrDefault(jsonKey + ".EasingInfo.Duration",		0.0f,			FILEKEY);
			std::string easingTypeStr						= jsonLoader->GetOrDefault(jsonKey + ".EasingInfo.EasingType",			std::string(),	FILEKEY);

			perfData_ResultSccene.easingInfo.type			= magic_enum::enum_cast<EasingType>(easingTypeStr).value();
			perfData_ResultSccene.easingInfo.isActive		= true;

			prefDataList.push_back(perfData_ResultSccene);

			jsonKey = JsonDataName + std::to_string(++index);
		}
	}
	// ロードしたよ
	m_WasLoad = true;
}
