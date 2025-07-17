#pragma once

/** ��]���� */
enum class RotationOrder
{
	XYZ,			/**< X �� Y �� Z */
	YZX,			/**< Y �� Z �� X */
	ZXY,			/**< Z �� X �� Y (RotationYawPitchRoll) */
};

/** ����̊e�� */
enum class FrustumPlane
{
	Near = 0,
	Far = 1,
	Left = 2,
	Right = 3,
	Top = 4,
	Bottom = 5,
};
