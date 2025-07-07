#pragma once

// ���ėp
#include "src/util/math/vector3.h"
#include "src/util/math/vector2.h"

namespace ShadowMap {

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �������
	/// </summary>
	void Release();

	/// <summary>
	/// �J�����̕`��͈͂ŉe�v�Z���s��
	/// </summary>
	void DrawBegin(int cameraIndex = 0);

	/// <summary>
	/// �e�̕`����I������
	/// </summary>
	void DrawEnd();

	/// <summary>
	/// �`��Ɏg�p����e�̐ݒ����������
	/// </summary>
	void CleanUp();

	/// <summary>
	/// �������邩��ݒ肷��
	/// </summary>
	/// <param name="value">true�Ȃ�Update��Draw���Ăяo��</param>
	void SetIsActive(bool value);

	/// <summary>
	/// �������Ă��邩���擾����
	/// </summary>
	bool IsActive();
}