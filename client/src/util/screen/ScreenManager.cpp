#include "ScreenManager.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/common/camera/CameraManager.h"

int ScreenManager::GetScreenDivCount()
{
	if (not CameraManager::IsScreenDivision())
		return 1;

	return (int)CameraManager::AllCameras().size();
}

int ScreenManager::GetScreenDivWidth()
{
	int width = WindowSetting::Inst().width;
	int divCount = GetScreenDivCount();

	return width / divCount;
}

int ScreenManager::GetScreenDivHeight()
{
	int height = WindowSetting::Inst().height;
	int divCount = GetScreenDivCount();

	return height / divCount;
}

Vector2 ScreenManager::GetScreenBeginPos(int divCount) {

	if (divCount < 0)
		return Vector2::Zero;

	return Vector2((float)GetScreenDivWidth() * divCount, 0.0f);
}

Vector2 ScreenManager::GetScreenEndPos(int divCount) {

	if (divCount < 0)
		return Vector2::Zero;

	return Vector2((float)GetScreenDivWidth() * (divCount + 1), WindowSetting::Inst().height);
}
