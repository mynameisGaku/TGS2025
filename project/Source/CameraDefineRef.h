#pragma once

#include <vector>
#include <string>
#include "Util/Vector3.h"

class CameraDefineRef
{
private:
	const std::string   FILEPATH = "data/Json/Camera/CameraDefine.json";// �t�@�C���̃p�X
	const std::string   FILEKEY = "CameraDefine";                   // �t�@�C�����ʗp�L�[
public:
	static CameraDefineRef& Inst()
	{
		static CameraDefineRef instance;
		return instance;
	}

	float m_Near;		// �`�揈�����s���ߕӂ̈ʒu
	float m_Far;		// �`�揈�����s�������̈ʒu
	float m_Perspective;// ����p

	float m_ConeRange;	// �R�[���̒���
	float m_ConeAngle;	// �R�[���̊p�x

	float m_RotX_Max;		// X��]�̍ő�l
	float m_RotX_Min;		// X��]�̍ŏ��l
	float m_RotSpeedLimit;	// �J�����̉�]���x�̐���

	Vector3 m_OffsetDef;	// �J�����̊��葊�΍��W
	Vector3 m_TargetDef;	// �J�����̊��蒍�����W

	Vector3 m_OffsetChase;	// �ǔ��J�����̊��葊�΍��W
	Vector3 m_TargetChase;	// �ǔ��J�����̊��蒍�����W

	void Load(bool ForceLoad = false);

private:

	bool                m_WasLoad = false; // ���[�h�ς݂��H �d�����p

	CameraDefineRef() :
		m_Near(0.0f),
		m_Far(0.0f),
		m_Perspective(0.0f),
		m_ConeRange(0.0f),
		m_ConeAngle(0.0f),
		m_RotX_Max(0.0f),
		m_RotX_Min(0.0f),
		m_RotSpeedLimit(0.0f),
		m_OffsetDef(V3::ZERO),
		m_TargetDef(V3::ZERO),
		m_OffsetChase(V3::ZERO),
		m_TargetChase(V3::ZERO)
	{ /*DO NOTHING*/
	}

	~CameraDefineRef()
	{ /*DO NOTHING*/
	}
};

#define CAMERADEFINE_REF CameraDefineRef::Inst()