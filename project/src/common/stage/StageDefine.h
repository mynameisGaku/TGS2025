#pragma once
#include <string>

namespace StageDefine {

	static const float STAGE_RANGE = 1750.0f;	// �X�e�[�W�͈̔�

	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g�̏��
	/// </summary>
	struct StageObjInfo {
		int hModel;			// �`�惂�f��
		int hHitModel;		// �����蔻�胂�f��
		std::string type;	// �I�u�W�F�N�g�̎��
		std::string fileName;	// �t�@�C���p�X

		StageObjInfo() :
			hModel(-1),
			hHitModel(-1),
			type(""),
			fileName("")
		{
		}
	};

	/// <summary>
	/// �X�e�[�W�I�u�W�F�N�g�̎��
	/// </summary>
	enum class ObjType {
		None = -1,
		Axe_00,
		Max,
	};
}