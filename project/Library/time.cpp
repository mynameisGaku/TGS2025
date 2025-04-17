#include "time.h"
#include <Windows.h>

#include "Source/ImGuiTree/imGuiManager.h"

namespace {

	LARGE_INTEGER freq;
	LARGE_INTEGER current;
	float deltaTime;
	float lapseRate;
	int hitStop;
}

void Time::Init() {

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&current);
	Refresh();
	lapseRate = 1.0f;
	hitStop = 0;

#ifdef IMGUI
	ImGuiManager::AddNode(new ImGuiNode_SliderFloat("Time", &lapseRate, 0.0f, 2.0f, "%.2f"));
#endif // IMGUI
}

void Time::Refresh() {

	LARGE_INTEGER last = current;
	QueryPerformanceCounter(&current);
	deltaTime = static_cast<float>(current.QuadPart - last.QuadPart) / freq.QuadPart;

	if (deltaTime >= FrameToSec(1.0f))
		deltaTime = FrameToSec(1.0f);

	hitStop = max(hitStop - 1, 0);
}

float Time::DeltaTime() {

	return deltaTime;
}

void Time::SetLapseRate(float rate) {

	lapseRate = rate;
}

float Time::LapseRate() {

	return lapseRate;
}

float Time::DeltaTimeLapseRate() {

	return deltaTime * lapseRate;
}

void Time::SetHitStop(int time) {

	if (hitStop < time)
		hitStop = time;
}

int Time::HitStop() {

	return hitStop;
}

bool Time::IsHitStop() {

	return (hitStop > 0);
}
