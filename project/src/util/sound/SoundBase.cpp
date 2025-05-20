#include "SoundBase.h"

// ���ėp
#include "../Library/myDxLib.h"
#include "../Library/time.h"
#include "../Source/Util/Utils.h"

// �����o�E�@�\
#include "cameraManager.h"

using namespace SoundDefine;

SoundBase::SoundBase() {

	info = nullptr;
	fade = SoundFade();

	label = "";

	isFadeOutEnd = false;
	isSetPan = false;

	playPos = nullptr;
}

SoundBase::~SoundBase() {

}

void SoundBase::Update() {

	// Fade�̋@�\���ғ����Ă���ꍇ�̏���
	if (fade.info.isActive) {
		applyFadeVolume();
	}
	else {

		ChangeVolumeSoundMem(info->curVolume, info->handle);

		// �p���ݒ���s���ꍇ�̏���
		if (isSetPan)
			SetPan(playPos);
	}

	if (IsPlaying() == false)
		Stop();
}

void SoundBase::Play(SoundDefine::SoundInfo* _info, const std::string& _label) {

	if (_info == nullptr)
		return;

	info = _info;
	label = _label;
	fade.current = static_cast<float>(info->curVolume);

	applyFadeVolume();
	PlaySoundMem(info->handle, info->playType);
}

void SoundBase::PlaySetPan(SoundDefine::SoundInfo* _info, const std::string& _label, Vector3* pos) {

	if (_info == nullptr || pos == nullptr)
		return;

	Play(_info, _label);
	SetPan(pos);

	isSetPan = true;
}

void SoundBase::PlaySetFrequency(SoundDefine::SoundInfo* _info, const std::string& _label, const float& frequency) {

	if (_info == nullptr)
		return;

	Play(_info, _label);
	SetFrequency(frequency);
}

void SoundBase::SetFrequency(const float& frequency) {

	if (frequency != -1) {
		//���f�[�^��44.10KHz�̂��ߔ����̒l���~���̉��Ƃ���
		float setFrequency = (44100.0f * 0.5f) * (1.0f + frequency);
		SetFrequencySoundMem(static_cast<int>(setFrequency), info->handle);
	}
	else
		SetFrequencySoundMem(static_cast<int>(frequency), info->handle);

	applyFadeVolume();
	PlaySoundMem(info->handle, info->playType);
}

void SoundBase::FadeIn(SoundDefine::SoundInfo* _info, const std::string& _label, const SoundDefine::SoundFade& _fade) {

	if (_info == nullptr)
		return;

	info = _info;
	label = _label;

	fade = _fade;

	fade.begin	= fade.begin < 0 ? 0 : fade.begin;
	fade.end	= fade.end < 0 ? info->curVolume : fade.end;

	fade.info.time = 0.0f;
	fade.info.isActive = true;

	applyFadeVolume();
	PlaySoundMem(info->handle, info->playType);
}

void SoundBase::FadeOut(const SoundDefine::SoundFade& _fade, const bool& _isFadeOutEnd) {

	fade = _fade;

	fade.begin = fade.begin < 0 ? info->curVolume : fade.begin;
	fade.end = fade.end < 0 ? 0 : fade.end;

	fade.info.time = 0.0f;
	fade.info.isActive = true;

	isFadeOutEnd = _isFadeOutEnd;

	applyFadeVolume();
}

void SoundBase::Stop() {

	SetFrequencySoundMem(-1, info->handle);
	StopSoundMem(info->handle);
	DestroyMe();
}

void SoundBase::SetPan(Vector3* pos) {

	if (pos == nullptr)
		return;

	playPos = pos;

	Camera* camera = CameraManager::MainCamera();	// ���C���J����
	Vector3 soundVec = *playPos - camera->WorldPos();// �J��������Đ����W�ւ̃x�N�g��

	const float range = 25.0f * fade.current;	// �������Ȃ��Ȃ鋗��

	int vol = static_cast<int>((range - soundVec.Size()) / range * 255.0f);	// ���������𔽉f��������
	vol = min(max(vol, 0), 255);	// �X�g�b�p�[���|����

	Vector3 forward = V3::RIGHT * MGetRotY(camera->transform->rotation.y);
	float pan = VDot(VNorm(soundVec), forward);

	ChangeVolumeSoundMem(vol, info->handle);
	ChangePanSoundMem(static_cast<int>(pan * 255) / 2, info->handle);
}

bool SoundBase::IsPlaying() const {

	if (info == nullptr)
		return false;

	return (CheckSoundMem(info->handle) == 1 || (fade.info.isActive == false && isFadeOutEnd));
}

bool SoundBase::CheckConsistency(const std::string& typeName, const std::string& _label) const {

	if (info == nullptr)
		return false;

	return (info->typeName == typeName && label == _label);
}

void SoundBase::applyFadeVolume() {

	if (info == nullptr)
		return;

	fade.Update();
	ChangeVolumeSoundMem(static_cast<int>(fade.current), info->handle);
}
