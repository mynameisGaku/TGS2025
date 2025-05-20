#pragma once
#include "src/common/stage/stageDefine.h"
#include "src/util/object3D/Object3D.h"

/// <summary>
/// �X�e�[�W�̃N���X���Ǘ�����(�n�`�E��E��)
/// </summary>
namespace Stage {

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	/// <summary>
	/// �n�ʃ��f��(�|���S��)�Ɛ����Ƃ̓����蔻�������
	/// </summary>
	/// <param name="begin">�����̎n�_</param>
	/// <param name="end">�����̏I�_</param>
	/// <param name="hitPos">���������ꍇ�ɍ��W��Ԃ��|�C���^�[</param>
	/// <returns>���������� True</returns>
	bool ColCheckGround(Vector3 begin, Vector3 end, Vector3* hitPos = nullptr);

	/// <summary>
	/// �ǃ��f��(�J�v�Z��)�Ƃ̐����Ƃ̓����蔻��
	/// </summary>
	/// <param name="begin">�J�v�Z���̎n�_</param>
	/// <param name="end">�J�v�Z���̏I�_</param>
	/// <param name="r">�J�v�Z���̔��a</param>
	/// <param name="push">�����o���x�N�g��</param>
	/// <returns>�������Ă�����true</returns>
	bool ColCheckWall(Vector3 begin, Vector3 end, float r, Vector3* push = nullptr);

	/// <summary>
	/// �n�ʂ̃N���X
	/// </summary>
	Object3D* Ground();

	/// <summary>
	/// ��̃N���X
	/// </summary>
	Object3D* Sky();

	/// <summary>
	/// �ǂ̃N���X
	/// </summary>
	Object3D* Wall();
}
