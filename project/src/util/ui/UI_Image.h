#pragma once
// ���p����
#include "src/util/ui/UI_Canvas.h"

/// <summary>
/// �摜�\�����s���N���X
/// </summary>
/// <author> �~�b�` </author>
class UI_Image : public UI_Canvas {
public:
	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// 
	/// </summary>
	/// <param name="image">�`��摜</param>
	/// <param name="trs">���W�E��]�E�g�k�̏��</param>
	UI_Image(const int& image, const RectTransform& trs);
	~UI_Image();

	//==========================================================================================
	// ���e��֐�

	void Draw() override;

	/// <summary>
	/// �c���`�揈��
	/// </summary>
	void DrawAfterImage() override;

};