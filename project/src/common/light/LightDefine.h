#pragma once
#include "framework/myDxLib.h"

namespace LightDefine {

	// ���C�g�̎��
	enum class LightType {
		None = -1,
		Directional,	// ���s����
		Point,			// �_����
		Spot,			// �Ɩ�����
		Ambient,		// ������
		Max
	};

	/// <summary>
	/// ���C�g�Ŏg�p������
	/// </summary>
	class LightInfo {
	public:
		LightType type;		// ���C�g�̎��
		int handle;			// ���C�g�̃n���h��
		float range;		// ���C�g�̗L������
		float atten0;		// ���������p�����[�^�[0
		float atten1;		// ���������p�����[�^�[1
		float atten2;		// ���������p�����[�^�[2
		float outAngle;		// �X�|�b�g���C�g�̃R�[���̊O���̊p�x
		float inAngle;		// �X�|�b�g���C�g�̃R�[���̓����̊p�x
		COLOR_F colorDif;	// �g�U�F	
		COLOR_F colorSpc;	// ���ːF
		COLOR_F colorAmb;	// ���F

		/// <summary>
		/// ������
		/// </summary>
		LightInfo() :
			type(LightType::None),
			handle(-1),
			range(0.0f),
			atten0(0.0f),
			atten1(0.0f),
			atten2(0.0f),
			outAngle(0.0f),
			inAngle(0.0f),
			colorDif(COLOR_F(1.0f, 1.0f, 1.0f, 1.0f)),
			colorSpc(COLOR_F(1.0f, 1.0f, 1.0f, 1.0f)),
			colorAmb(COLOR_F(0.0f, 0.0f, 0.0f, 0.0f))
		{
		}
		
		/// <summary>
		/// �����ŏ�����
		/// </summary>
		/// <param name="_type">��ޖ�</param>
		/// <param name="_handle">���C�g�̃n���h��</param>
		/// <param name="_range">���C�g�̗L������</param>
		/// <param name="_atten0">���������p�����[�^�[0</param>
		/// <param name="_atten1">���������p�����[�^�[1</param>
		/// <param name="_atten2">���������p�����[�^�[2</param>
		/// <param name="_outAngle">�X�|�b�g���C�g�̃R�[���̊O���̊p�x</param>
		/// <param name="_inAngle">�X�|�b�g���C�g�̃R�[���̓����̊p�x</param>
		/// <param name="_colorDif">�g�U�F</param>
		/// <param name="_colorSpc">���ːF</param>
		/// <param name="_colorAmb">���F</param>
		LightInfo(const LightType& _type, const int& _handle, const float& _range,
				  const float& _atten0, const float& _atten1, const float& _atten2, const float& _outAngle, const float& _inAngle,
				  const COLOR_F& _colorDif, const COLOR_F& _colorSpc, const COLOR_F& _colorAmb) :
			type(_type),
			handle(_handle),
			range(_range),
			atten0(_atten0),
			atten1(_atten1),
			atten2(_atten2),
			outAngle(_outAngle),
			inAngle(_inAngle),
			colorDif(_colorDif),
			colorSpc(_colorSpc),
			colorAmb(_colorAmb)
		{
		}

		~LightInfo() {}
	};
}