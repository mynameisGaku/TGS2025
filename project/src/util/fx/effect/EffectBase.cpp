#include "effectBase.h"
#include <EffekseerForDXLib.h>

#include "src/util/time/GameTime.h"
#include "src/util/math/Operations.h"

using namespace EffectDefine;

EffectBase::EffectBase() {

	info = EffectInfo();
	label = "";
}

EffectBase::~EffectBase() {

	Stop();
}

void EffectBase::Update() {

	if (IsActive() == false)
		return;

	if (IsPlaying() == false)
		Stop();		
}

void EffectBase::Play3D(const EffectDefine::EffectInfo& _info, const Transform& trs, const std::string& _label, const bool& loop) {

	// Šù‚ÉÄ¶’†‚È‚çAÄ¶‚µ‚È‚¢
	if (IsPlaying())
		return;

	info = _info;
	label = _label;

	info.dimension = Dimensional::_3D;
	info.playingHandle = PlayEffekseer3DEffect(info.handle);
	info.isLoop = loop;

	SetTransform3D(trs);
	SetPlaySpeed(info.playSpeed);
}

void EffectBase::Play2D(const EffectDefine::EffectInfo& _info, const RectTransform& rectTrs, const std::string& _label, const bool& loop) {

	// Šù‚ÉÄ¶’†‚È‚çAÄ¶‚µ‚È‚¢
	if (IsPlaying())
		return;

	info = _info;

	info.dimension = Dimensional::_2D;
	info.playingHandle = PlayEffekseer2DEffect(info.handle);
	info.isLoop = loop;

	SetTransform2D(rectTrs);
	SetPlaySpeed(info.playSpeed);

	label = _label;
}

void EffectBase::Stop() {

	info.isLoop = false;

	switch (info.dimension) {
	case Dimensional::_2D:	StopEffekseer2DEffect(info.playingHandle);	break;
	case Dimensional::_3D:	StopEffekseer3DEffect(info.playingHandle);	break;
	default:
		break;
	}
}

void EffectBase::SetTransform3D(const Transform& trs) {

	const Transform globalTrs = trs.Global();

	SetPosition3D(globalTrs.position);
	SetRotation3D(globalTrs.rotation);
	SetScale3D(globalTrs.scale);
}

void EffectBase::SetTransform2D(const RectTransform& trs) {

	const RectTransform globalTrs = trs.Global();

	SetPosition2D(globalTrs.position);
	SetRotation2D(globalTrs.rotation);
	SetScale2D(globalTrs.scale);
}

void EffectBase::SetPosition3D(const Vector3& pos) {

	transform->position = pos;
	int err = 0;
	switch (info.dimension) {
	case Dimensional::_2D:	err = SetPosPlayingEffekseer2DEffect(info.playingHandle, transform->position.x, transform->position.y, transform->position.z); break;
	case Dimensional::_3D:	err = SetPosPlayingEffekseer3DEffect(info.playingHandle, transform->position.x, transform->position.y, transform->position.z); break;
	default:
		break;
	}
}

void EffectBase::SetPosition2D(const Vector2& pos) {

	transform->position.x = pos.x;
	transform->position.y = pos.y;

	switch (info.dimension) {
	case Dimensional::_2D:	SetPosPlayingEffekseer2DEffect(info.playingHandle, transform->position.x, transform->position.y, transform->position.z); break;
	case Dimensional::_3D:	SetPosPlayingEffekseer3DEffect(info.playingHandle, transform->position.x, transform->position.y, transform->position.z); break;
	default:
		break;
	}
}

void EffectBase::SetRotation3D(const Vector3& rot) {

	transform->rotation = rot;

	switch (info.dimension) {
	case Dimensional::_2D:	SetRotationPlayingEffekseer2DEffect(info.playingHandle, transform->rotation.x, transform->rotation.y, transform->rotation.z); break;
	case Dimensional::_3D:	SetRotationPlayingEffekseer3DEffect(info.playingHandle, transform->rotation.x, transform->rotation.y, transform->rotation.z); break;
	default:
		break;
	}
}

void EffectBase::SetRotation2D(float rot) {

	transform->rotation.y = rot;

	switch (info.dimension) {
	case Dimensional::_2D:	SetRotationPlayingEffekseer2DEffect(info.playingHandle, transform->rotation.x, transform->rotation.y, transform->rotation.z); break;
	case Dimensional::_3D:	SetRotationPlayingEffekseer3DEffect(info.playingHandle, transform->rotation.x, transform->rotation.y, transform->rotation.z); break;
	default:
		break;
	}
}

void EffectBase::SetScale3D(const Vector3& scale) {

	transform->scale = scale;

	switch (info.dimension) {
	case Dimensional::_2D:	SetScalePlayingEffekseer2DEffect(info.playingHandle, transform->scale.x, transform->scale.y, transform->scale.z); break;
	case Dimensional::_3D:	SetScalePlayingEffekseer3DEffect(info.playingHandle, transform->scale.x, transform->scale.y, transform->scale.z); break;
	default:
		break;
	}
}

void EffectBase::SetScale2D(const Vector2& scale) {

	transform->scale.x = scale.x;
	transform->scale.y = scale.y;
	transform->scale.z = scale.Average();

	switch (info.dimension) {
	case Dimensional::_2D:	SetScalePlayingEffekseer2DEffect(info.playingHandle, transform->scale.x, transform->scale.y, transform->scale.z); break;
	case Dimensional::_3D:	SetScalePlayingEffekseer3DEffect(info.playingHandle, transform->scale.x, transform->scale.y, transform->scale.z); break;
	default:
		break;
	}
}

void EffectBase::SetPlaySpeed(const float& speed) {

	info.playSpeed = speed;

	switch (info.dimension) {
	case Dimensional::_2D:	SetSpeedPlayingEffekseer2DEffect(info.playingHandle, info.playSpeed * GTime.GetTimeScale()); break;
	case Dimensional::_3D:	SetSpeedPlayingEffekseer3DEffect(info.playingHandle, info.playSpeed * GTime.GetTimeScale()); break;
	default:
		break;
	}
}

void EffectBase::SetRGBA(const int& r, const int& g, const int& b, const int& a) {

	switch (info.dimension) {
	case Dimensional::_2D:	SetColorPlayingEffekseer2DEffect(info.playingHandle, r, g, b, a); break;
	case Dimensional::_3D:	SetColorPlayingEffekseer3DEffect(info.playingHandle, r, g, b, a); break;
	default:
		break;
	}
}

bool EffectBase::IsPlaying() const {

	bool result = false;

	switch (info.dimension) {
	case Dimensional::_2D:	result = (IsEffekseer2DEffectPlaying(info.playingHandle) == 0); break;
	case Dimensional::_3D:	result = (IsEffekseer3DEffectPlaying(info.playingHandle) == 0); break;
	default:
		break;
	}

	return (info.isLoop || result);
}

bool EffectBase::CheckConsistency(const std::string& typeName, const std::string& _label) const {

	return (info.typeName == typeName && label == _label);
}
