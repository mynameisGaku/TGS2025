#pragma once
#include <string>
#include "EasingUtils.h"

namespace SoundDefine {

	/// <summary>
	/// �����̕���
	/// </summary>
	enum class SoundCategory {
		cNone = -1,
		cMaster,
		cBGM,
		cSE,
		cVoice,
		cENV,
		cMax
	};

	/// <summary>
	/// ���Ɋւ�����
	/// </summary>
	/// <param name="handle">�T�E���h�̃n���h��</param>
	/// <param name="curVolume">���݂̉���</param>
	/// <param name="defVolume">����̉���</param>
	/// <param name="playType">�Đ��`��(DX_PLAYTYPE_�`�`)</param>
	/// <param name="category">���̕���</param>
	/// <param name="fileName">�t�@�C���p�X</param>
	/// <param name="typeName">���g�̖��O</param>
	class SoundInfo {
	public:
		int handle;		// �T�E���h�̃n���h��
		int curVolume;	// ���݂̉���
		int defVolume;	// ����̉���
		int playType;	// �Đ��`��(DX_PLAYTYPE_�`�`)
		SoundCategory category;	// ���̕���
		std::string fileName;	// �t�@�C���p�X
		std::string typeName;	// ���g�̖��O

		SoundInfo() :
			handle(-1),
			curVolume(0),
			defVolume(0),
			playType(DX_PLAYTYPE_BACK),
			category(SoundCategory::cNone),
			fileName(""),
			typeName("")
		{
		}

		~SoundInfo() {
			fileName.clear();
			typeName.clear();
		}
	};

	typedef EasingFloat SoundFade;
}