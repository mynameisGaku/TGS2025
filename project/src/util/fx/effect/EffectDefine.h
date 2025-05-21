#pragma once
#include <string>

namespace EffectDefine {

	/// <summary>
	/// ����
	/// </summary>
	enum class Dimensional {
		_None = -1,
		_2D,
		_3D,
		_Max,
	};

	/// <summary>
	/// �G�t�F�N�g�̏��
	/// </summary>
	/// <param name="handle">�G�t�F�N�g�̃n���h��</param>
	/// <param name="playingHandle">�Đ��p�n���h��</param>
	/// <param name="magnification">�g�嗦</param>
	/// <param name="defMagnification">����g�嗦</param>
	/// <param name="playSpeed">�Đ����x</param>
	/// <param name="defPlaySpeed">����Đ����x</param>
	/// <param name="isLoop">�J��Ԃ��\�����邩?</param>
	/// <param name="fileName">�f�[�^���u����Ă���t�@�C���p�X</param>
	/// <param name="typeName">���g�̖��O</param>
	/// <param name="dimension">����</param>
	class EffectInfo {
	public:
		int handle;				// �G�t�F�N�g�̃n���h��
		int playingHandle;		// �Đ��p�n���h��
		float magnification;	// �g�嗦
		float defMagnification;	// ����g�嗦
		float playSpeed;		// �Đ����x
		float defPlaySpeed;		// ����Đ����x
		bool isLoop;			// �J��Ԃ��\�����邩�H
		std::string fileName;	// �f�[�^���u����Ă���t�H���_�p�X
		std::string typeName;	// ���g�̖��O
		Dimensional dimension;	// ����

		EffectInfo() :
			handle(-1),
			playingHandle(-1),
			magnification(1.0f),
			defMagnification(1.0f),
			playSpeed(1.0f),
			defPlaySpeed(1.0f),
			isLoop(false),
			fileName(""),
			typeName(""),
			dimension(Dimensional::_None)
		{
		}

		~EffectInfo() {}
	};
};