#include "AnimationEvent.h"
#include "Animator.h"

#include "../Object3D.h"
#include "../SoundManager.h"
#include "../EffectManager.h"

AnimationEvent::AnimationEvent(Animator* _animator) {

	animator = _animator;
	parent = _animator->Parent<Object3D>();

	SetCommand("", "", -1.0f);
}

AnimationEvent::~AnimationEvent()
{
}

void AnimationEvent::Update() {

	CommandProcess();
}

void AnimationEvent::Draw()
{
}

void AnimationEvent::CommandProcess() {

	if (animator == nullptr || parent == nullptr)
		return;

	// コマンドの実行が可能かつ、アニメーションフレームが実行開始フレームより大きかったら
	if (canUseCommande == true && executeAnimFrame <= animator->CurrentAttach().nowFrame) {
		canUseCommande = false;

		// ▽音再生
		if (commandType == "PlaySound")
			SoundManager::PlaySetPan(commandContents, "AnimEvent", &parent->transform->position);
		// ▽エフェクト再生
		else if (commandType == "PlayEffect")
			EffectManager::Play3D(commandContents, *parent->transform, "AnimEvent");
	}
}

void AnimationEvent::SetCommand(std::string _commandType, std::string _commandContents, float _executeAnimFrame) {

	SetCommandType(_commandType);
	SetCommandContents(_commandContents);
	SetExecuteAnimFrame(_executeAnimFrame);
	SetUseCommand(true);
}
