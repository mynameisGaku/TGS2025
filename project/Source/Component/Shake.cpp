#include "shake.h"

// ž”Ä—p
#include "../../Library/time.h"
#include "../Object3D.h"

Shake::Shake() {

	parentObj = nullptr;

	SetParam({ Type::tNone }, 0.0f, V3::ZERO, 0.0f);
}

Shake::~Shake() {

}

void Shake::Init(Object3D* pObj) {

	parentObj = pObj;

}

void Shake::Update() {

	if (duration > 0.0f) {
		duration -= Time::DeltaTime();

		IntervalUpdate();

		float rate = CurrentRatio();

		power = (initPower * rate) * direction;
	}
	else {
		power = V3::ZERO;
	}
}

void Shake::Draw() {

	
}

void Shake::IntervalUpdate() {

	Vector3 vTime = Vector3(Time::DeltaTime());
	interval -= vTime;

	if (interval.x <= 0.0f) {
		interval.x = initInterval.x;
		direction.x *= -1;
	}

	if (interval.y <= 0.0f) {
		interval.y = initInterval.y;
		direction.y *= -1;
	}

	if (interval.z <= 0.0f) {
		interval.z = initInterval.z;
		direction.z *= -1;
	}
}

void Shake::SetParam(std::list<Type> type, float _power, Vector3 _interval, float _duration) {

	if (this->duration > 0.0f && initPower.Size() > _power)
		return;

	types = type;

	power = V3::ZERO;
	initPower = V3::ZERO;

	for (const auto& itr : types) {
		switch (itr) {
		case Shake::Type::tVertical:
			initPower += Vector3(0.0f, _power, 0.0f);
			power += initPower;
			break;

		case Shake::Type::tHorizontal:
			initPower += Vector3(_power, 0.0f, 0.0f);
			power += initPower;
			break;

		case Shake::Type::tDepth:
			initPower += Vector3(0.0f, 0.0f, _power);
			power += initPower;
			break;

		case Shake::Type::tAll:
			initPower = Vector3(_power);
			power += initPower;
			break;

		default:
			initPower = V3::ZERO;
			power = initPower;
			break;
		}
	}

	interval = _interval;
	duration = _duration;

	if (duration == 0)
		initDuration = 1.0f;
	else
		initDuration = duration;
	
	direction = V3::ONE;
}

void Shake::ApplyShakeTrs(std::string frameName, float mag) {

	if (parentObj == nullptr || frameName == "" || duration <= 0.0f)
		return;

	int hModel = parentObj->Model();

	int frame = MV1SearchFrame(hModel, frameName.c_str());
	MATRIX mLocal = MV1GetFrameLocalMatrix(hModel, frame);
	MATRIX mShake = MGetTranslate(power * mag);

	//MV1ResetFrameUserLocalMatrix(hModel, frame);
	MV1SetFrameUserLocalMatrix(hModel, frame, mLocal * mShake);
}

void Shake::ClearShakeTrs(std::string frameName) {

	if (parentObj == nullptr || frameName == "")
		return;

	int hModel = parentObj->Model();

	int frame = MV1SearchFrame(hModel, frameName.c_str());

	MV1ResetFrameUserLocalMatrix(hModel, frame);
}
