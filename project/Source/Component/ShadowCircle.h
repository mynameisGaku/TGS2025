#pragma once

// ���p����
#include "Component.h"

/// <summary>
/// �ۉe�̕`����s��
/// </summary>
class ShadowCircle : public Component {
public:
	//==========================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	ShadowCircle();
	~ShadowCircle();

	//==========================================================================================
	// ���e��֐�

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw() override;

private:
	int handle;

};