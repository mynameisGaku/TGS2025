#pragma once

// ���p����
#include "src/util/ui/UI_Canvas.h"

#include "src/util/transform/Transform.h"

/// <summary>
/// �r���{�[�h�̏������s���N���X
/// </summary>
class UI_BillBoard : public UI_Canvas {
public:

	Transform* transform;

	//================================================================================
	// ���R���X�g���N�^�E�f�X�g���N�^

	/// <summary>
	/// �r���{�[�h�𐶐�����
	/// </summary>
	/// <param name="graph">�g�p����摜�n���h��</param>
	/// <param name="trs">�`����W</param>
	/// <param name="center"></param>
	/// <param name="size"></param>
	UI_BillBoard(const int& image, const Transform& trs, const Vector2& center, const float& clipSize);

	~UI_BillBoard();

	//==========================================================================================
	// ���e��֐�

	void Update() override;
	void Draw() override;

};
