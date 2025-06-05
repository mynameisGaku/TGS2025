#include "src/util/fx/post_effect/bloom/BloomManager.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/util/input/InputManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/camera/CameraManager.h"

void SetDrawScreenWithCamera(int screen)
{
	Vector3 camPos = GetCameraPosition();
	Vector3 camTarget = GetCameraTarget();
	SetDrawScreen(screen);
	SetCameraPositionAndTarget_UpVecY(camPos, camTarget);

	CAMERADEFINE_REF.Load();
	SetupCamera_Perspective(CAMERADEFINE_REF.m_Perspective);
}

BloomManager::BloomManager()
{
	m_EmitterScreen = -1;

	Reset();
}

BloomManager::~BloomManager()
{
	DeleteGraph(m_EmitterScreen);
}

void BloomManager::Reset()
{
	BLOOM_REF.Load();

	DeleteGraph(m_EmitterScreen);

	m_EmitterScreen = MakeScreen((int)WindowSetting::Inst().width, (int)WindowSetting::Inst().height, FALSE);
	SetUseGraphZBuffer(m_EmitterScreen, TRUE);
	SetParameter(BLOOM_REF.Param);
	m_DoBloom = true;
}

void BloomManager::Update()
{
	if (CheckHitKey(KEY_INPUT_R))
	{
		Reset();
	}
	if (InputManager::Push(KeyDefine::KeyCode::F2))
	{
		m_DoBloom = !m_DoBloom;
	}
}

void BloomManager::Draw()
{
	if (not m_DoBloom) return;

	Vector2 pos = CameraManager::GetScreenDivisionPos();
	Vector2 size = CameraManager::GetScreenDivisionSize();

	DrawOnScreenDiv((int)pos.x, (int)pos.y, (int)size.x, (int)size.y);
}

void BloomManager::DrawOnScreenDiv(int x, int y, int w, int h) {

	if (not m_DoBloom) return;

	int highBrightScreen = MakeScreen(w, h, FALSE);
	int downScaleScreen = MakeScreen(w / m_Parameter.DownScale, h / m_Parameter.DownScale, FALSE);

	GetDrawScreenGraph(x, y, x + w, y + h, highBrightScreen);

	// 描画結果から高輝度部分のみを抜き出した画像を得る
	GraphFilterBlt(highBrightScreen, highBrightScreen, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, m_Parameter.MinBrightness, TRUE, GetColor(0, 0, 0), 255);
	// 個別の発光を加算する
	GraphBlendBlt(highBrightScreen, m_EmitterScreen, highBrightScreen, 255, DX_GRAPH_BLEND_ADD);
	GraphBlendBlt(highBrightScreen, m_EmitterScreen, highBrightScreen, 255, DX_GRAPH_BLEND_ADD);

	// 高輝度部分を８分の１に縮小した画像を得る
	GraphFilterBlt(highBrightScreen, downScaleScreen, DX_GRAPH_FILTER_DOWN_SCALE, m_Parameter.DownScale);
	// ８分の１に縮小した画像をガウスフィルタでぼかす
	GraphFilterBlt(downScaleScreen, downScaleScreen, DX_GRAPH_FILTER_GAUSS, 16, m_Parameter.GaussianRate);

	// 描画モードをバイリニアフィルタリングにする( 拡大したときにドットがぼやけるようにする )
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	// 描画ブレンドモードを加算にする
	SetDrawBlendMode(DX_BLENDMODE_ADD, m_Parameter.AddRate);

	// 高輝度部分を縮小してぼかした画像を画面いっぱいに描画する
	SetDrawBright(255, 255, 255);
	DrawExtendGraph(x, y, x + w, y + h, downScaleScreen, FALSE);
	DrawExtendGraph(x, y, x + w, y + h, downScaleScreen, FALSE);
	SetDrawBright(255, 255, 255);

	// 描画ブレンドモードをブレンド無しに戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 描画モードを二アレストに戻す
	SetDrawMode(DX_DRAWMODE_NEAREST);

	// スクリーンを消す
	DeleteGraph(highBrightScreen);
	DeleteGraph(downScaleScreen);
	SetDrawScreenWithCamera(m_EmitterScreen);
	ClearDrawScreen();
	ClearDrawScreenZBuffer();

	// 描画先を戻す
	SetDrawScreenWithCamera(DX_SCREEN_BACK);
}

void BloomManager::SetDrawScreenToEmitter()
{
	SetDrawScreenWithCamera(m_EmitterScreen);
	CopyGraphZBufferImage(m_EmitterScreen, DX_SCREEN_BACK);
}

void BloomManager::SetDrawScreenToBack()
{
	SetDrawScreenWithCamera(DX_SCREEN_BACK);
}

void BloomManager::SetParameter(BloomRef::Parameter parameter)
{
	m_Parameter = parameter;
}
