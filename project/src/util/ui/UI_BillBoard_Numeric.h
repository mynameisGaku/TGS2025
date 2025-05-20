#pragma once

// ���p����
#include "src/util/ui/UI_Canvas.h"

// ���ėp
#include <vector>

// ���ʂŕK�v�ȕ�
#include "src/util/ui/UI_BillBoard.h"

/// <summary>
/// ���l��`�悷��r���{�[�h�B0�`9�������ꂽ�摜��10�������������g�p����B
/// </summary>
class UI_BillBorad_Numeric : public UI_Canvas {
public:
	UI_BillBorad_Numeric();
	~UI_BillBorad_Numeric();

	void Update() override;
	void Draw() override;

	void Add(const int numberGraph[10], const Transform& trs, const Vector2& center, const float& clipSize, const int& value);

private:
	std::vector<UI_BillBoard*> numbers;	// ���l��`�悷��r���{�[�h
	std::vector<Vector3> numbersPos;	// �r���{�[�h�̊�����W
	float clipSize;		// �摜�̐؂蔲���̑傫��
};