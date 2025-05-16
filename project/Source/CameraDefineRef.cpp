#include "CameraDefineRef.h"
#include "settings_json.h"

#include "Util/Utils.h"

void CameraDefineRef::Load(bool ForceLoad)
{
	if (not ForceLoad && m_WasLoad)
	{
		return;
	}

	// ���X�̏�����
	auto jsonLoader = Settings_json::Inst();

	// JSON �ǂݍ���
	jsonLoader->LoadSettingJson(FILEPATH, FILEKEY);

	// �e��l�擾
	m_Near			= jsonLoader->GetOrDefault<float>("Near", 0.0f, FILEKEY);
	m_Far			= jsonLoader->GetOrDefault<float>("Far", 0.0f, FILEKEY);
	m_Perspective	= Math::DegToRad(jsonLoader->GetOrDefault<float>("Perspective", 0.0f, FILEKEY));
	m_RotX_Max		= Math::DegToRad(jsonLoader->GetOrDefault<float>("RotX_Max", 0.0f, FILEKEY));
	m_RotX_Min		= Math::DegToRad(jsonLoader->GetOrDefault<float>("RotX_Min", 0.0f, FILEKEY));
	m_RotSpeedLimit	= Math::DegToRad(jsonLoader->GetOrDefault<float>("RotSpeedLimit", 0.0f, FILEKEY));

	m_OffsetDef = jsonLoader->GetOrDefault<Vector3>("OffsetDef", V3::ZERO, FILEKEY);
	m_OffsetChase = jsonLoader->GetOrDefault<Vector3>("OffsetChase", V3::ZERO, FILEKEY);
	m_OffsetAim = jsonLoader->GetOrDefault<Vector3>("OffsetAim", V3::ZERO, FILEKEY);

	// ���[�h������
	m_WasLoad = true;
}
