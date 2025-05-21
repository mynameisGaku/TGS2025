#pragma once

#include <vector>
#include <string>
#include "src/util/math/Vector3.h"

class CameraDefineRef
{
private:
	const std::string   FILEPATH = "data/Json/Camera/CameraDefine.json";// �t�@�C���̃p�X
	const std::string   FILEKEY = "CameraDefine";                   // �t�@�C�����ʗp�L�[

	static CameraDefineRef* instance;
public:
	static CameraDefineRef* Inst()
	{
		if (instance == nullptr)
		{
			instance = new CameraDefineRef;
		}
		return instance;
	}

	// " ���t�@�����X��� "
	// ���𕶎��񌟍������璅���Ƃ���ŉ�����Ă܂��B(�����炭���C�����[�v�̂�������)
	void Destroy()
	{
		if (instance)
		{
			delete instance;
		}
		instance = nullptr;
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

	Vector3 m_OffsetAim;	// �G�C���J�����̊��葊�΍��W
	Vector3 m_TargetAim;	// �G�C���J�����̊��蒍�����W

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
		m_OffsetDef(Vector3::Zero),
		m_TargetDef(Vector3::Zero),
		m_OffsetChase(Vector3::Zero),
		m_TargetChase(Vector3::Zero)
	{ /*DO NOTHING*/
	}

	~CameraDefineRef()
	{ /*DO NOTHING*/
	}
};

#define CAMERADEFINE_REF (*CameraDefineRef::Inst())