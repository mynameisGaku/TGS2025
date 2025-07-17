#pragma once
#include "mathCommon.h"

/**
	@brief		�^�������𐶐�����̃N���X�ł��B
	@details	�A���S���Y���� Xorshift �@�ł��B
*/
class Randomizer
{
public:

	/**
		@brief		���݂̎��Ԃɉ����Č��肳���V�[�h�l���g�p���ăC���X�^���X�����������܂��B
	*/
	Randomizer();

	/**
		@brief		�w�肵���V�[�h�l���g�p���ăC���X�^���X�����������܂��B
		@param[in]	seed	: �V�[�h�l
	*/
	Randomizer(int seed);

public:

	/**
		@brief		�V�[�h�l��ύX���܂��B�^�����������킪������ԂɃ��Z�b�g����܂��B
		@param[in]	seed	: �V�[�h�l
	*/
	void SetSeed(int seed);

	/**
		@brief		���������_���l��Ԃ��܂��B
		@return		0 �ȏ�̐����l
	*/
	int GetInt();

	/**
		@brief		�w�肵���ő�l��菬�������������_���l��Ԃ��܂��B(0 <= �l < maxValue)
		@param[in]	maxValue	: ����l
		@return		0 �ȏ�� maxValue ��菬���������l
	*/
	int GetInt(int maxValue);

	/**
		@brief		�w�肵���͈͓��̐��������_���l��Ԃ��܂��B(minValue <= �l < maxValue)
		@param[in]	minValue	: �����l
		@param[in]	maxValue	: ����l
		@return		minValue �ȏ�� maxValue ��菬���������l
	*/
	int GetIntRange(int minValue, int maxValue);

	/**
		@brief		�w�肵���U�ꕝ�̐��������_���l��Ԃ��܂��B(median-width <= �l < median+width)
		@param[in]	median	: �����l
		@param[in]	width	: �}��
		@return		median-width �ȏ�� median+width ��菬���������l
	*/
	int GetIntWidth(int median, int width);

	/**
		@brief		���������_���l��Ԃ��܂��B
		@return		0.0 �ȏ� 1.0 �����̎����l
	*/
	float GetFloat();

	/**
		@brief		�w�肵���ő�l��菬�������������_���l��Ԃ��܂��B(0 <= �l < maxValue)
		@param[in]	maxValue	: ����l
		@return		0 �ȏ�� maxValue ��菬���������l
	*/
	float GetFloat(float maxValue);

	/**
		@brief		�w�肵���͈͓��̎��������_���l��Ԃ��܂��B(minValue <= �l < maxValue)
		@param[in]	minValue	: �����l
		@param[in]	maxValue	: ����l
		@return		minValue �ȏ�� maxValue ��菬���������l
	*/
	float GetFloatRange(float minValue, float maxValue);

	/**
		@brief		�w�肵���U�ꕝ�̎��������_���l��Ԃ��܂��B(median-width <= �l < median+width)
		@param[in]	median	: �����l
		@param[in]	width	: �}��
		@return		median-width �ȏ�� median+width ��菬���������l
	*/
	float GetFloatWidth(float median, float width);

private:
	unsigned int x, y, z, w;
};

/**
	@brief	Randomizer �̃O���[�o���C���X�^���X
*/
extern Randomizer Random;
