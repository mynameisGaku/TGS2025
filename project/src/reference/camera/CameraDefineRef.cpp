#include "src/reference/camera/CameraDefineRef.h"
#include "src/util/file/json/settings_json.h"
#include "src/util/math/MathUtil.h"

CameraDefineRef* CameraDefineRef::instance = nullptr;

void CameraDefineRef::Load(bool ForceLoad)
{
	if (not ForceLoad && m_WasLoad)
	{
		return;
	}

	// 諸々の初期化
	auto jsonLoader = Settings_json::Inst();

	// JSON 読み込み
	jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

	// 各種値取得
	m_Near			= jsonLoader->GetOrDefault<float>("Near", 0.0f, FILEKEY);
	m_Far			= jsonLoader->GetOrDefault<float>("Far", 0.0f, FILEKEY);
	m_Perspective	= MathUtil::ToRadians(jsonLoader->GetOrDefault<float>("Perspective", 0.0f, FILEKEY));
	m_RotX_Max		= MathUtil::ToRadians(jsonLoader->GetOrDefault<float>("RotX_Max", 0.0f, FILEKEY));
	m_RotX_Min		= MathUtil::ToRadians(jsonLoader->GetOrDefault<float>("RotX_Min", 0.0f, FILEKEY));
	m_RotSpeedLimit	= MathUtil::ToRadians(jsonLoader->GetOrDefault<float>("RotSpeedLimit", 0.0f, FILEKEY));

	m_ConeAngle = jsonLoader->GetOrDefault<float>("ConeAngle", 0.0f, FILEKEY);
	m_ConeRange = jsonLoader->GetOrDefault<float>("ConeRange", 0.0f, FILEKEY);

	m_OffsetDef = jsonLoader->GetOrDefault<Vector3>("OffsetDef", Vector3::Zero, FILEKEY);
	m_TargetDef = jsonLoader->GetOrDefault<Vector3>("TargetDef", Vector3::Zero, FILEKEY);

	m_OffsetChase = jsonLoader->GetOrDefault<Vector3>("OffsetChase", Vector3::Zero, FILEKEY);
	m_TargetChase = jsonLoader->GetOrDefault<Vector3>("TargetChase", Vector3::Zero, FILEKEY);

	m_OffsetAim = jsonLoader->GetOrDefault<Vector3>("OffsetAim", Vector3::Zero, FILEKEY);
	m_TargetAim = jsonLoader->GetOrDefault<Vector3>("TargetAim", Vector3::Zero, FILEKEY);

	// ロードしたよ
	m_WasLoad = true;
}
