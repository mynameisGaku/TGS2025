#include "src/util/fx/post_effect/bloom/BloomManager.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/util/input/InputManager.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/common/camera/CameraManager.h"
#include "src/util/math/MathUtil.h"
#include "BloomTarget.h"
#include "src/common/light/LightManager.h"

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
	m_ColorScreen = -1;
	m_EmitterScreen = -1;
	m_LastDrawScreen = -1;

	Reset();
}

BloomManager::~BloomManager()
{
	DeleteGraph(m_ColorScreen);
	DeleteGraph(m_EmitterScreen);
}

void BloomManager::Reset()
{
	BLOOM_REF.Load();

	DeleteGraph(m_EmitterScreen);

	m_ColorScreen = MakeScreen((int)WindowSetting::Inst().width, (int)WindowSetting::Inst().height, TRUE);
	m_EmitterScreen = MakeScreen((int)WindowSetting::Inst().width, (int)WindowSetting::Inst().height, TRUE);
	SetUseGraphZBuffer(m_EmitterScreen, TRUE);
	m_LastDrawScreen = -1;
	SetParameter(BLOOM_REF.Param);
	m_DoBloom = true;
	m_IsUsingColorScreen = false;
	m_IsUsingEmitterScreen = false;
	m_State = NONE;
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

	// 登録したオブジェクトはエミッションをさえぎらないかも
	drawEmitterTargets();
	drawCustomBloomTargets();

	Camera* camera = CameraManager::GetCameraDrawing();
	if (camera == nullptr)
		return;

	Vector2 pos = Vector2::Zero;
	Vector2 size = Vector2::Zero;
	Vector2 end = Vector2::Zero;
	camera->GetDefaultDrawArea(&pos, &size);

	end = pos + size;

	DrawOnScreenDiv((int)pos.x, (int)pos.y, (int)end.x, (int)end.y);
}

void BloomManager::DrawOnScreenDiv(int x, int y, int w, int h) {

	if (not m_DoBloom) return;

	int highBrightScreen = MakeScreen(w, h, FALSE);
	int downScaleScreen = MakeScreen(w / m_Parameter.DownScale, h / m_Parameter.DownScale, FALSE);

	int drawScreen = GetDrawScreen();
	//GetDrawScreenGraph(x, y, x + w, y + h, highBrightScreen);

	// 描画結果から高輝度部分のみを抜き出した画像を得る
	GraphFilterBlt(m_ColorScreen, highBrightScreen, DX_GRAPH_FILTER_BRIGHT_CLIP, DX_CMP_LESS, m_Parameter.MinBrightness, TRUE, GetColor(0, 0, 0), 255);
	// 個別の発光を加算する
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
	SetDrawScreenWithCamera(m_ColorScreen);
	ClearDrawScreen();
	ClearDrawScreenZBuffer();

	m_WasInitEmitterScreen = false;

	// 描画先を戻す
	SetDrawScreenWithCamera(drawScreen);
}

void BloomManager::SetDrawScreenToColor()
{
	m_LastDrawScreen = GetDrawScreen();
	SetDrawScreenWithCamera(m_ColorScreen);
	m_IsUsingColorScreen = true;
	m_IsUsingEmitterScreen = false;
}

void BloomManager::CopyDrawScreenToColor()
{
	if (not m_DoBloom) return;

	Camera* camera = CameraManager::GetCameraDrawing();
	if (camera == nullptr)
		return;

	Vector2 pos = Vector2::Zero;
	Vector2 size = Vector2::Zero;
	Vector2 end = Vector2::Zero;
	camera->GetDefaultDrawArea(&pos, &size);

	end = pos + size;

	GetDrawScreenGraph(pos.x, pos.y, pos.x + end.x, pos.y + end.y, m_ColorScreen);
}

void BloomManager::SetDrawScreenToEmitter()
{
	m_LastDrawScreen = GetDrawScreen();
	SetDrawScreenWithCamera(m_EmitterScreen);
	m_IsUsingColorScreen = false;
	m_IsUsingEmitterScreen = true;
}

void BloomManager::CopyDrawScreenToEmitter()
{
	if (not m_DoBloom) return;

	Camera* camera = CameraManager::GetCameraDrawing();
	if (camera == nullptr)
		return;

	Vector2 pos = Vector2::Zero;
	Vector2 size = Vector2::Zero;
	Vector2 end = Vector2::Zero;
	camera->GetDefaultDrawArea(&pos, &size);

	end = pos + size;

	int drawScreen = GetDrawScreen();
	GetDrawScreenGraph(pos.x, pos.y, pos.x + end.x, pos.y + end.y, m_EmitterScreen);
	CopyGraphZBufferImage(m_EmitterScreen, drawScreen);
}

void BloomManager::SetDrawScreenToLastScreen()
{
	SetDrawScreenWithCamera(m_LastDrawScreen);
	m_LastDrawScreen = -1;
	m_IsUsingColorScreen = false;
	m_IsUsingEmitterScreen = false;
}

void BloomManager::SetDrawScreenToBack()
{
	SetDrawScreenWithCamera(DX_SCREEN_BACK);
	m_IsUsingColorScreen = false;
	m_IsUsingEmitterScreen = false;
}

void BloomManager::SetDrawScreenToAll()
{
	m_LastDrawScreen = GetDrawScreen();
	SetUsePixelLighting(TRUE);
	SetRenderTargetToShader(0, m_ColorScreen);
	SetRenderTargetToShader(1, m_LastDrawScreen);
	SetRenderTargetToShader(2, m_EmitterScreen);
}

void BloomManager::ResetDrawScreenToAll()
{
	SetUsePixelLighting(FALSE);
	SetRenderTargetToShader(0, m_LastDrawScreen);
	SetRenderTargetToShader(1, -1);
	SetRenderTargetToShader(2, -1);
}

void BloomManager::AddEmitterTarget(GameObject* pObject, float effectRate)
{
	m_EmitterTargets.push_back(new BloomObject(pObject, effectRate));
}

void BloomManager::AddCustomBloomTarget(GameObject* pObject, float effectRate)
{
	m_CustomBloomTargets.push_back(new BloomObject(pObject, effectRate));
}

void BloomManager::AddEmitterTarget(void(*pDrawFunc)(), float effectRate)
{
	m_EmitterTargets.push_back(new BloomDrawFunc(pDrawFunc, effectRate));
}

void BloomManager::AddCustomBloomTarget(void(*pDrawFunc)(), float effectRate)
{
	m_CustomBloomTargets.push_back(new BloomDrawFunc(pDrawFunc, effectRate));
}

void BloomManager::SetParameter(BloomRef::Parameter parameter)
{
	m_Parameter = parameter;
}

void BloomManager::drawEmitterTargets()
{
	m_State = EMITTER_OBJECT;
	{
		for (BloomTarget* object : m_EmitterTargets)
		{
			object->Draw();

			SetDrawScreenToEmitter();
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, MathUtil::RateToByte(object->EffectRate));
			SetUseLighting(FALSE);
			{
				object->Draw();
			}
			SetDrawScreenToLastScreen();
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			SetUseLighting(TRUE);
		}

		// メモリ解放
		for (auto* object : m_EmitterTargets) {
			delete object;
		}
		m_EmitterTargets.clear();
	}
	m_State = NONE;
}

void BloomManager::drawCustomBloomTargets()
{
	m_State = CUSTOM_BLOOM_OBJECT;
	{
		for (BloomTarget* object : m_CustomBloomTargets)
		{
			object->Draw();

			/*
			SetDrawScreenToColor();
			{
#if FALSE
				Light* light = LightManager::GetLight(LightDefine::LightType::Directional);
				const LightInfo info = light->Info();
				LightInfo newInfo = info;

				newInfo.colorAmb.a *= object->EffectRate;
				newInfo.colorAmb.r *= object->EffectRate;
				newInfo.colorAmb.g *= object->EffectRate;
				newInfo.colorAmb.b *= object->EffectRate;
				newInfo.colorDif.a *= object->EffectRate;
				newInfo.colorDif.r *= object->EffectRate;
				newInfo.colorDif.g *= object->EffectRate;
				newInfo.colorDif.b *= object->EffectRate;
				newInfo.colorSpc.a *= object->EffectRate;
				newInfo.colorSpc.r *= object->EffectRate;
				newInfo.colorSpc.g *= object->EffectRate;
				newInfo.colorSpc.b *= object->EffectRate;

				light->SetInfo(newInfo);
				light->Update();
				{
					object->Draw();
				}
				light->SetInfo(info);
				light->Update();
#else
				int r, g, b;
				GetFogColor(&r, &g, &b);
				float density = GetFogDensity();
				float start, end;
				GetFogStartEnd(&start, &end);
				int enable = GetFogEnable();

				SetFogColor(0, 0, 0);
				SetFogDensity(object->EffectRate);
				SetFogStartEnd(0, 0);
				SetFogEnable(TRUE);
				{
					object->Draw();
				}
				SetFogColor(r, g, b);
				SetFogDensity(density);
				SetFogStartEnd(start, end);
				SetFogEnable(enable);
#endif
			}
			SetDrawScreenToLastScreen();
			*/
		}

		// メモリ解放
		for (auto* object : m_CustomBloomTargets) {
			delete object;
		}
		m_CustomBloomTargets.clear();
	}
	m_State = NONE;
}
