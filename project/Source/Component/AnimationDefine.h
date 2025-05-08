#pragma once
#include <vector>
#include <string>
#include <array>
#include "../Util/Vector3.h"

class Animator;
class AttachedAnimation;
class AnimationEvent;

/// <summary>
/// �A�j���[�V�����ɔC�ӂŐݒ肷�鍀��
/// </summary>
struct AnimOption {
	float defaultAnimSpeed;	// ����Đ����x
	Vector3 offset;		// ���[�g�ɉ�����I�t�Z�b�g
	bool isLoop;		// ���[�v�Đ��̗L��
	std::array<bool, 3> isFixedRoot;	// ���[�g�Œ�̗L���ix,y,z���j

	AnimOption() : AnimOption(1.0f, V3::ZERO, false, { false, false, false })
	{
	}

	AnimOption(float _defAnimSpeed, const Vector3& _offset, bool _isLoop, std::array<bool, 3> isFixedRoot) :
		defaultAnimSpeed(_defAnimSpeed),
		offset(_offset),
		isLoop(_isLoop),
		isFixedRoot(isFixedRoot)
	{
	}

	AnimOption SetDefaultAnimSpeed(float _defaultAnimSpeed) { defaultAnimSpeed = _defaultAnimSpeed; return *this; }
	AnimOption SetOffset(const Vector3& _offset) { offset = _offset; return *this; }
	AnimOption SetIsLoop(bool _isLoop) { isLoop = _isLoop; return *this; }
	AnimOption SetIsFixedRoot(std::array<bool, 3> _isFixedRoot) { isFixedRoot = _isFixedRoot; return *this; }
};

/// <summary>
/// �A�j���[�V�����������
/// </summary>
struct AnimInfo {
	int handle;				// �A�j���[�V�����̃n���h��
	std::string animName;	// �A�j���[�V������

	float startFrame;	// �Đ��J�n�t���[��
	float endFrame;		// �Đ��I���t���[��

	AnimOption option;	// �C�ӂ̃I�v�V����

	std::vector<AnimationEvent*> event;	// �A�j���[�V�����ɍ��킹�čs������

	AnimInfo() :
		handle(-1),
		animName(""),
		startFrame(0.0f),
		endFrame(0.0f)
	{
	}
};