#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "src/util/math/Vector3.h"

#include "src/common/camera/CameraDefine.h"

class CameraPerformanceRef
{
private:
	const std::string   FILEPATH = "data/Json/Camera/CameraPerformance.json";// �t�@�C���̃p�X
	const std::string   FILEKEY = "CameraPerformance";                   // �t�@�C�����ʗp�L�[

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

	void Load(bool ForceLoad = false);

	std::vector<CameraDefine::PerformanceData> GetPerfDatas(const std::string& name) const
	{
		if (m_PerfDataMap.contains(name))
			return m_PerfDataMap.at(name);
		else
			return std::vector<CameraDefine::PerformanceData>(); // ���݂��Ȃ��ꍇ�̓f�t�H���g�l��Ԃ�
	}

	std::unordered_map<std::string, std::vector<CameraDefine::PerformanceData>> m_PerfDataMap; // �p�t�H�[�}���X�f�[�^�̃}�b�v

private:

	bool m_WasLoad; // ���[�h�ς݂��H �d�����p

	CameraPerformanceRef() :
		m_PerfDataMap(),
		m_WasLoad(false)
	{
		Load(true); // �������[�h
	}

	~CameraPerformanceRef()
	{ /*DO NOTHING*/
	}
};

#define CAMERA_PERFORMANCE_REF (*CameraPerformanceRef::Inst())