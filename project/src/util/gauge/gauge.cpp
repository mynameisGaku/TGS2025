#include "gauge.h"

// �ÓI�����o�ϐ��̏�����
int Gauge::currentId = 0;

Gauge::Gauge()
{
	id = GenerateId();  // ID�𐶐����Đݒ�
}

int Gauge::GenerateId()
{
	return currentId++;  // �V����ID�𐶐�
}

