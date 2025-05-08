#pragma once
#include <vector>
#include <string>
#include <array>
#include "../Util/Vector3.h"

class Animator;
class AttachedAnimation;
class AnimationEvent;

/// <summary>
/// アニメーションに任意で設定する項目
/// </summary>
struct AnimOption {
	float defaultAnimSpeed;	// 既定再生速度
	Vector3 offset;		// ルートに加えるオフセット
	bool isLoop;		// ループ再生の有無
	std::array<bool, 3> isFixedRoot;	// ルート固定の有無（x,y,z軸）

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
/// アニメーションが持つ情報
/// </summary>
struct AnimInfo {
	int handle;				// アニメーションのハンドル
	std::string animName;	// アニメーション名

	float startFrame;	// 再生開始フレーム
	float endFrame;		// 再生終了フレーム

	AnimOption option;	// 任意のオプション

	std::vector<AnimationEvent*> event;	// アニメーションに合わせて行う処理

	AnimInfo() :
		handle(-1),
		animName(""),
		startFrame(0.0f),
		endFrame(0.0f)
	{
	}
};