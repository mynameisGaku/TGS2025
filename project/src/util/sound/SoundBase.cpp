#include "SoundBase.h"

//=================================================================================
//
//		コンストラクタ・デストラクタ
//
//=================================================================================

//---------------------------------------------------------------------------------
SoundBase::SoundBase() {

	soundInfo = nullptr;
	playPosition = nullptr;

	fade.SetEasing(0.0f, 255.0f, 0.0f, EasingType::Linear, false);
	label = "";

	isFadeIn = false;
	isFadeOut= false;
	isFadeOutEnd = false;
	isSetPan = false;
}

//---------------------------------------------------------------------------------
SoundBase::~SoundBase() {

}

//=================================================================================
//
//		各種関数
//
//=================================================================================

//---------------------------------------------------------------------------------
void SoundBase::Update() {

	fade.Update();

	// フェード処理が有効な場合は、フェードの進行を行う
	if (IsFade())
		ApplyVolumeFade();

	// パン設定を行う場合の処理
	if (isSetPan && playPosition != nullptr)
		SetPan(*playPosition);

	if (not fade.info.isActive && isFadeOutEnd)
		Stop();
}

//---------------------------------------------------------------------------------
void SoundBase::ApplyVolume() {

	if (soundInfo == nullptr)
		return;

	ChangeVolumeSoundMem(static_cast<int>(soundInfo->curVolume), soundInfo->handle);
}

//---------------------------------------------------------------------------------
void SoundBase::ApplyVolumeFade() {

	if (soundInfo == nullptr || not fade.info.isActive)
		return;

	ChangeVolumeSoundMem(static_cast<int>(fade.current), soundInfo->handle);
}

//=================================================================================
//
//		再生処理
//
//=================================================================================

//---------------------------------------------------------------------------------
void SoundBase::Play(const SoundDefine::SoundInfo* info, const std::string& label) {

	if (info == nullptr)
		return;

	soundInfo = info;
	this->label = label;

	if (IsPlaying())
		return;

	PlaySoundMem(soundInfo->handle, soundInfo->playType);
	ApplyVolume();
}

//=================================================================================
//
//		停止処理
//
//=================================================================================

//---------------------------------------------------------------------------------
void SoundBase::Stop() {

	SetFrequencySoundMem(-1, soundInfo->handle);
	StopSoundMem(soundInfo->handle);
}

//=================================================================================
//
//		フェード処理
//
//=================================================================================

//---------------------------------------------------------------------------------
void SoundBase::FadeIn(const SoundDefine::SoundInfo* info, const std::string& label, int begin, int end, float duration_sec, EasingType easing) {

	if (info == nullptr)
		return;

	Play(info, label);

	float volBegin = (float)begin;
	float volEnd = (float)end;

	if (begin == -1)
		volBegin = fade.begin < 0.0f ? fade.begin : fade.current;

	if (end == -1)
		volEnd = fade.end < 0.0f ? fade.end : (float)info->curVolume;

	fade.SetEasing(volBegin, volEnd, duration_sec, easing, true);
	ApplyVolumeFade();

	isFadeIn = true;
	isFadeOut = false;
	isFadeOutEnd = false;
}

//---------------------------------------------------------------------------------
void SoundBase::FadeOut(int end, float duration_sec, EasingType easing, bool isFadeOutEnd) {

	this->isFadeOutEnd = isFadeOutEnd;

	float volBegin = fade.begin < 0.0f ? fade.begin : fade.current;
	float volEnd = (float)end;

	if (end == -1)
		volEnd = fade.end < 0 ? fade.end : 0.0f;

	fade.SetEasing(volBegin, volEnd, duration_sec, easing, true);
	ApplyVolumeFade();

	isFadeIn = false;
	isFadeOut = true;
}

//=================================================================================
//
//		セッター
//
//=================================================================================

//---------------------------------------------------------------------------------
void SoundBase::SetPan(const VECTOR& position) {

	VECTOR soundVec = VSub(position, GetCameraPosition());// カメラから再生座標へのベクトル

	const float range = 25.0f * fade.current;	// 聞こえなくなる距離

	int vol = static_cast<int>((range - VSize(soundVec)) / range * 255.0f);	// 距離減衰を反映した音量
	vol = min(max(vol, 0), 255);	// ストッパーを掛ける

	VECTOR forward = VTransformSR(VGet(1.0f, 0.0f, 0.0f), MGetRotY(GetCameraAngleTRotate()));
	float pan = VDot(VNorm(soundVec), forward);

	ChangeVolumeSoundMem(vol, soundInfo->handle);
	ChangePanSoundMem(static_cast<int>(pan * 255) / 2, soundInfo->handle);
}

//---------------------------------------------------------------------------------
void SoundBase::SetPanPointer(const VECTOR* position) {

	if (position == nullptr)
		return;

	playPosition = position;

	VECTOR soundVec = VSub(*playPosition, GetCameraPosition());// カメラから再生座標へのベクトル

	const float range = 25.0f * fade.current;	// 聞こえなくなる距離

	int vol = static_cast<int>((range - VSize(soundVec)) / range * 255.0f);	// 距離減衰を反映した音量
	vol = min(max(vol, 0), 255);	// ストッパーを掛ける

	VECTOR forward = VTransformSR(VGet(1.0f, 0.0f, 0.0f), MGetRotY(GetCameraAngleTRotate()));
	float pan = VDot(VNorm(soundVec), forward);

	ChangeVolumeSoundMem(vol, soundInfo->handle);
	ChangePanSoundMem(static_cast<int>(pan * 255) / 2, soundInfo->handle);
}

//---------------------------------------------------------------------------------
void SoundBase::SetFrequency(float frequency) {

	if (frequency != -1) {
		//元データが44.10KHzのため半分の値を停止時の音とする
		float setFrequency = (44100.0f * 0.5f) * (1.0f + frequency);
		SetFrequencySoundMem(static_cast<int>(setFrequency), soundInfo->handle);
	}
	else {
		SetFrequencySoundMem(static_cast<int>(frequency), soundInfo->handle);
	}

	ApplyVolume();
}

//=================================================================================
//
//		ゲッター
//
//=================================================================================

//---------------------------------------------------------------------------------
bool SoundBase::IsPlaying() const {

	if (soundInfo == nullptr)
		return false;

	return (CheckSoundMem(soundInfo->handle) != 0);
}

//=================================================================================
//
//		確認用
//
//=================================================================================

//---------------------------------------------------------------------------------
bool SoundBase::CheckConsistency(const std::string& name, const std::string& label) const {

	if (soundInfo == nullptr)
		return false;

	return (soundInfo->typeName == name && this->label == label);
}