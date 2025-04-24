#pragma once
#include "Library/gameObject.h"
#include "BloomRef.h"

/// <summary>
/// �J�����ݒ���ێ����ĕ`����ύX����
/// </summary>
/// <param name="screen">�`���̃X�N���[��</param>
void SetDrawScreenWithCamera(int screen);

/// <summary>
/// Draw���ɉ�ʂɃu���[����������
/// </summary>
/// <author>�����h�l</author>
class BloomManager : public GameObject
{
public:
	BloomManager();
	~BloomManager();
	void Reset();
	void Update() override;
	void Draw() override;

	void SetDrawScreenToEmitter();
	void SetDrawScreenToBack();
	void SetParameter(BloomRef::Parameter parameter);
private:
	int m_EmitterScreen;	// �ʂŔ��������������`�悷�邽�߂̃X�N���[��
	bool m_DoBloom;		// �u���[���������邩�ǂ���
	BloomRef::Parameter m_Parameter;	// �u���[���̃p�����[�^
};