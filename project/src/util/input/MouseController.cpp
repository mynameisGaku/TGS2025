#include "MouseController.h"

#include "Util/Utils.h"

using namespace KeyDefine;

namespace {

	MouseController::MouseInfo mouse;	// マウスの情報
	std::unordered_map<KeyCode, bool>* mouseInputs;			// マウスの入力情報
	std::unordered_map<KeyCode, bool>* mouseInputsBefore;	// 1フレーム前のマウスの入力情報
}

void MouseController::Init() {

	if (mouseInputs == nullptr) {
		mouseInputs = new std::unordered_map<KeyCode, bool>();

		auto mouseKeyCodes = DeviceTypeToInputData(DeviceType::Mouse);

		for (const auto& itr : mouseKeyCodes)
			(*mouseInputs)[itr.keyCode] = false;

	}

	if (mouseInputsBefore == nullptr) {
		mouseInputsBefore = new std::unordered_map<KeyCode, bool>();
		*mouseInputsBefore = *mouseInputs;
	}

	GetCursorPos(&mouse.point);
	ScreenToClient(mouse.hWnd, &mouse.point);

	SetMouseDispFlag(TRUE);
	SetValidMousePointerWindowOutClientAreaMoveFlag(TRUE);
}

void MouseController::Release() {

	if (mouseInputs != nullptr) {
		delete mouseInputs;
		mouseInputs = nullptr;
	}

	if (mouseInputsBefore != nullptr) {
		delete mouseInputsBefore;
		mouseInputsBefore = nullptr;
	}
}

void MouseController::Update() {

	PushStatusUpdate();

	// マウスの位置を取得
	GetMousePoint(&mouse.newX, &mouse.newY);

	// マウスの移動量を取得
	mouse.moveX = mouse.newX - mouse.oldX;
	mouse.moveY = mouse.newY - mouse.oldY;
	mouse.oldX = mouse.newX;
	mouse.oldY = mouse.newY;

	// マウスホイールの移動量を取得
	mouse.wheelRot = GetMouseWheelRotVol();

	int iScreenWidthHalf = Screen::WIDTH / 2;	// 画面の横幅の半分(int型)
	int iScreenHeightHalf = Screen::HEIGHT / 2;	// 画面の縦幅の半分(int型)

	switch (mouse.movement) {
	case MouseMovement::Free:
	case MouseMovement::OnScreen:
		// 座標の更新
		mouse.position = Vector2(static_cast<float>(mouse.newX), static_cast<float>(mouse.newY));
		SetMouseDispFlag(TRUE);
		break;

	case MouseMovement::Fixed:
		int posX = mouse.newX - iScreenWidthHalf;
		int posY = mouse.newY - iScreenHeightHalf;
		mouse.position = Vector2(static_cast<float>(posX), static_cast<float>(posY));
		mouse.moveX = posX;
		mouse.moveY = posY;
		SetMousePoint(iScreenWidthHalf, iScreenHeightHalf);
		SetMouseDispFlag(FALSE);
		break;
	}
}

void MouseController::PushStatusUpdate() {

	if (mouseInputsBefore == nullptr) {
		mouseInputsBefore = new std::unordered_map<KeyCode, bool>();
	}

	*mouseInputsBefore = *mouseInputs;

	auto mouseKeyCodes = DeviceTypeToInputData(DeviceType::Mouse);

	for (const auto& itr : mouseKeyCodes) {
		(*mouseInputs)[itr.keyCode] = ((GetMouseInput() & (*itr.value)) != 0);
	}
}

bool MouseController::CheckClick(KeyDefine::KeyCode keyCode) {

	if (mouseInputs == nullptr || mouseInputsBefore == nullptr)
		return false;

	return ((*mouseInputs)[keyCode] && (*mouseInputsBefore)[keyCode] == false);
}

bool MouseController::CheckHold(KeyDefine::KeyCode keyCode) {

	if (mouseInputs == nullptr || mouseInputsBefore == nullptr)
		return false;

	return ((*mouseInputs)[keyCode]);
}

bool MouseController::CheckRelease(KeyDefine::KeyCode keyCode) {

	if (mouseInputs == nullptr || mouseInputsBefore == nullptr)
		return false;

	return ((*mouseInputs)[keyCode] == false && (*mouseInputsBefore)[keyCode]);
}

void MouseController::SetMouseMovement(MouseMovement move) {

	int iScreenWidthHalf = static_cast<int>(Screen::WIDTH_HALF);
	int iScreenHeightHalf = static_cast<int>(Screen::HEIGHT_HALF);

	switch (move) {
	case MouseMovement::Free:		SetValidMousePointerWindowOutClientAreaMoveFlag(true);	break;
	case MouseMovement::OnScreen:	SetValidMousePointerWindowOutClientAreaMoveFlag(false); break;
	case MouseMovement::Fixed:
		SetValidMousePointerWindowOutClientAreaMoveFlag(false);
		mouse.oldX = iScreenWidthHalf;
		mouse.oldY = iScreenHeightHalf;
		SetMousePoint(iScreenWidthHalf, iScreenHeightHalf);
		break;
	}

	mouse.movement = move;
}

void MouseController::SetMouseSensitivity(Vector2 value) {

	mouse.sensitivity = value;
}

const MouseController::MouseInfo MouseController::Info() {

	return mouse;
}

bool MouseController::ColCheck_CursorToCircle(Vector2 pos, float r) {

	bool isCollision = false;

	Vector2 p1 = pos;
	Vector2 p2 = mouse.position;

	float r1 = r;
	float r2 = 2.0f;

	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;

	if (dx * dx + dy * dy < (r1 + r2) * (r1 + r2))
		isCollision = true;

	return isCollision;
}

bool MouseController::ColCheck_CursorToBox(Vector2 pos, Vector2 size) {

	bool isCollision = false;

	Vector2 p1 = pos;
	Vector2 p2 = mouse.position;

	Vector2 s1 = size;
	Vector2 s2 = Vector2(2.0f);

	if (p1.x < p2.x + s2.x &&
		p1.x + s1.x > p2.x &&
		p1.y < p2.y + s2.y &&
		p1.y + s1.y > p2.y)
		isCollision = true;

	return isCollision;
}
