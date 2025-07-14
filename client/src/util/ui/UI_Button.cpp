#include "src/util/ui/UI_Button.h"

// ◇演出・機能
#include "src/util/input/InputManager.h"
#include "src/util/input/MouseController.h"

using namespace KeyDefine;

UI_Button::UI_Button(const int& image, const RectTransform& trs, const std::string& _inputName, const std::vector<KeyDefine::TouchPhase>& _timing, const std::function<void()>& func, const int& _padNumber) :
	UI_Button(image, trs, _inputName, {}, _timing, func, _padNumber) {}

UI_Button::UI_Button(const int& image, const RectTransform& trs, const std::vector<KeyDefine::KeyCode>& _keyCode, const std::vector<KeyDefine::TouchPhase>& _timing, const std::function<void()>& func, const int& _padNumber) :
	UI_Button(image, trs, "", _keyCode, _timing, func, _padNumber) {}

UI_Button::UI_Button(const int& image, const RectTransform& trs) : 
	UI_Button(image, trs, "", {}, {}, nullptr, DX_INPUT_PAD1) {}

UI_Button::UI_Button(const int& image, const RectTransform& trs, const std::string& _inputName, const std::vector<KeyDefine::KeyCode>& _keyCode, const std::vector<KeyDefine::TouchPhase>& _timing, const std::function<void()>& func, const int& _padNumber) {

	Object2D::SetTransform(trs);
	Object2D::SetImage(image);

	inputName = _inputName;
	allKeyCode = _keyCode;
	timing = _timing;
	action = func;
	padNumber = _padNumber;

	isPushed = false;
	isPushedDarkenes = false;
	isMouseClick = false;

	SeparateInputTypes();
}

UI_Button::~UI_Button() {

}

void UI_Button::Update() {

	if (IsActive() == false || isUpdate == false)
		return;

	UI_Canvas::Update();

	isPushed = false;

	for (const auto& itr : timing) {
		isPushed = PushProcess(itr);
		if (isPushed)
			break;

		if (isMouseClick == false)
			continue;

		isPushed = PushProcessMouse(itr);
		if (isPushed)
			break;
	}
}

void UI_Button::Draw() {

	if (IsActive() == false || isDraw == false)
		return;

	UI_Canvas::Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	if (isPushedDarkenes && isPushed)
		DrawDarken();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG

	//RectTransform globalTrs = rectTransform->Global();	// グローバルな座標・回転・拡縮情報

	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);

	//DrawCircle(globalTrs.position.x, globalTrs.position.y, 16, GetColor(255, 255, 255), true);

	//Vector2 diff = DisplacementByAnchorPoint();

	//DrawBoxAA(globalTrs.position.x - diff.x * 0.75f, globalTrs.position.y - diff.y * 0.75f,
	//		  globalTrs.position.x + diff.x * 0.75f, globalTrs.position.y + diff.y * 0.75f,
	//		  GetColor(255, 0, 0), true);
	//
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//if (ColCheck_Mouse())
	//	DrawFormatString(100, 100, GetColor(255, 0, 255), "HitMouse");

#endif // _DEBUG
}

bool UI_Button::PushProcess(const KeyDefine::TouchPhase& phase) {

	bool result = false;

	switch (phase) {
	case TouchPhase::Begin:
		result = (inputName != "" && InputManager::Push(inputName, padNumber));
		if (result)
			break;

		for (const auto& itr : keyCodes) {
			result = InputManager::Push(itr, padNumber);
			if (result)
				break;
		}
		break;

	case TouchPhase::Stationary:
	case TouchPhase::Moved:
		result = (inputName != "" && InputManager::Hold(inputName, padNumber));
		if (result)
			break;

		for (const auto& itr : keyCodes) {
			result = InputManager::Hold(itr, padNumber);
			if (result)
				break;
		}
		break;

	case TouchPhase::Ended:
		result = (inputName != "" && InputManager::Release(inputName, padNumber));
		if (result)
			break;

		for (const auto& itr : keyCodes) {
			result = InputManager::Release(itr, padNumber);
			if (result)
				break;
		}
		break;

	default:
		break;
	}

	if (result && action != nullptr)
		action();

	return result;
}

bool UI_Button::PushProcessMouse(const KeyDefine::TouchPhase& phase) {

	bool result = false;

	if (action == nullptr || isMouseClick == false || ColCheck_Mouse() == false)
		return result;

	for (const auto& itr : mouseKeyCodes) {

		switch (phase) {
		case TouchPhase::Begin:
			result = InputManager::Push(itr, padNumber);
			break;

		case TouchPhase::Stationary:
		case TouchPhase::Moved:
			result = InputManager::Hold(itr, padNumber);
			break;

		case TouchPhase::Ended:
			result = InputManager::Release(itr, padNumber);
			break;

		default:
			break;
		}

		if (result) {
			action();
			return result;
		}
	}

	return result;
}

void UI_Button::DrawDarken() {

	RectTransform globalTrs = rectTransform->Global();	// グローバルな座標・回転・拡縮情報

	// 不透明度を設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, max(alpha - 50, 1));

	// 色を黒く塗りつぶす
	SetDrawBright(50, 50, 50);

	Object2D::Draw();

	// 元に戻す
	SetDrawBright(255, 255, 255);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UI_Button::SeparateInputTypes() {

	mouseKeyCodes.clear();
	keyCodes.clear();

	for (const auto& itr : InputManager::KeyList(inputName)) {
		// KeyCodeがマウスの情報かつ、mouseKeyCodesに含まれていない場合
		if (KeyCodeToDeviceType(itr) == DeviceType::Mouse && std::find(mouseKeyCodes.begin(), mouseKeyCodes.end(), itr) == mouseKeyCodes.end()) {
			mouseKeyCodes.push_back(itr);
		}
		// KeyCodeがマウスの情報ではないかつ、keyCodesに含まれていない場合
		if (KeyCodeToDeviceType(itr) != DeviceType::Mouse && std::find(keyCodes.begin(), keyCodes.end(), itr) == keyCodes.end()) {
			keyCodes.push_back(itr);
		}
	}

	for (const auto& itr : allKeyCode) {
		// KeyCodeがマウスの情報かつ、mouseKeyCodesに含まれていない場合
		if (KeyCodeToDeviceType(itr) == DeviceType::Mouse && std::find(mouseKeyCodes.begin(), mouseKeyCodes.end(), itr) == mouseKeyCodes.end()) {
			mouseKeyCodes.push_back(itr);
		}
		// KeyCodeがマウスの情報ではないかつ、keyCodesに含まれていない場合
		if (KeyCodeToDeviceType(itr) != DeviceType::Mouse && std::find(keyCodes.begin(), keyCodes.end(), itr) == keyCodes.end()) {
			keyCodes.push_back(itr);
		}
	}
}

void UI_Button::SetIsMouseClick(const bool& value) {

	isMouseClick = value;

	if (isMouseClick == false)
		return;

	mouseKeyCodes.clear();

	for (const auto& itr : InputManager::KeyList(inputName)) {
		if (KeyDefine::KeyCodeToDeviceType(itr) == DeviceType::Mouse)
			mouseKeyCodes.push_back(itr);
	}

	for (const auto& itr : keyCodes) {
		if (KeyDefine::KeyCodeToDeviceType(itr) == DeviceType::Mouse)
			mouseKeyCodes.push_back(itr);
	}
}

bool UI_Button::ColCheck_Mouse() {

	RectTransform globalTrs = rectTransform->Global();	// グローバルな座標・回転・拡縮情報
	Vector2 offset = DisplacementByAnchorPoint();

	return MouseController::ColCheck_CursorToCircle(globalTrs.position + offset, imageSize.GetLength());
}
