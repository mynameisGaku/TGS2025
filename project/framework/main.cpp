 //<summary>
 // CoGフレームワーク
 // WinMain()から始まります
 //</summary>
 //<author>N.Hanai</author>
 
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__);
#else
#define DBG_NEW new
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> 

#pragma comment(lib, "ws2_32.lib")

#include <DxLib.h>

#define IMGUI
#ifdef IMGUI
#include "vendor/imgui/imgui_impl_dxlib.hpp"

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "src/util/debug/imgui/imGuiManager.h"

#endif // IMGUI

#include <EffekseerForDXLib.h>
#include "framework/App.h"
#include "src/util/font/Font.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/util/file/json/settings_json.h"
#include "src/util/file/ini/settings_ini.h"
#include "src/util/file/FileUtil.h"

#include "src/reference/bloom/BloomRef.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/reference/chara/CharaDefineRef.h"
#include "src/reference/chara/CharaHPRef.h"
#include "src/reference/chara/CharaStaminaRef.h"
#include "src/reference/crystal/CrystalFragmentRef.h"
#include "src/reference/crystal/CrystalFragmentSpawnerRef.h"
#include <src/reference/game/GameRef.h>
#include "src/reference/input/InputRef.h"
#include <src/util/time/GameTime.h>
#include <src/reference/ui/UI_ButtonHintRef.h>
#include <src/reference/network/NetworkRef.h>
#include <src/util/editbox/editbox.hpp>
#include <src/util/ptr/PtrUtil.h>
#include <src/reference/camera/CameraPerformanceRef.h>

#define CoGVersion (2.2)

LARGE_INTEGER freq;
LARGE_INTEGER start, end;
int timeRecordCounter = 0;
double tick = 0.0;
bool isRefreshTick = false;

void BeginRecordPerformance();
void EndRecordPerformance();

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    auto& wSetting = WindowSetting::Inst();
	std::string path = "";
    SearchFilePath("window.ini", path);
    wSetting.Load(path);

	SetGraphMode((int)wSetting.width, (int)wSetting.height, 32);
	SetOutApplicationLogValidFlag(FALSE); // ログを出さない

    SYSTEMTIME t;
	GetSystemTime(&t);
	std::string wndname = wSetting.name + std::to_string(t.wMilliseconds);

	SetMainWindowText(wndname.c_str());
	SetWindowSizeExtendRate((double)wSetting.extend);
	ChangeWindowMode(wSetting.isWindow); // Windowモードの場合

#ifdef IMGUI
	SetHookWinProc(WndProc);	//プロシージャの設定
#endif // IMGUI

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetZBufferBitDepth(32);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	if (Effekseer_Init(18000) == -1)
	{
		DxLib_End();
		return -1;
	}

	SetHookWinProc([](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT /*CALLBACK*/
		{
			// DxLibとImGuiのウィンドウプロシージャを両立させる
			SetUseHookWinProcReturnValue(FALSE);
			return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		});
#ifdef IMGUI
	// ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());	ImGui_ImplDXlib_Init();
#endif // IMGUI

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Set2DSetting((int)wSetting.width, (int)wSetting.height);

	SetFontSize(32);
	Font::Load("data/font/", "sazanami_mincho.ttf", "さざなみ明朝");

	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	AppInit();

	int mStartTime = GetNowCount();

	while (true) {

		BeginRecordPerformance();

		int cur = GetNowCount();
		if (cur < mStartTime + 16) //120fps対策
			continue;
		mStartTime = cur;

#ifdef IMGUI
		ImGui_ImplDXlib_NewFrame();
		ImGui::NewFrame();
#endif // IMGUI

		ImGui::Begin("TimeRecord");
		ImGui::Text("time %lf[ms]", tick);
		ImGui::End();

		AppUpdate();
		ClearDrawScreen();
		AppDraw();
		DrawEffekseer2D();
		if (ProcessMessage() == -1 || IsExit())
			break;
		ScreenFlip();

		EndRecordPerformance();

		if (++timeRecordCounter % 60 == 0)
			tick = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;

#ifdef IMGUI

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDXlib_RenderDrawData();

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
#endif // IMGUI

	}

	AppRelease();

	/*
	リファレンス解放

	明示的に解放する必要があります。
	*/
	Settings_json::Inst()->Destroy();
	Settings_ini::Inst().Destroy();
	BLOOM_REF.Destroy();
	CAMERADEFINE_REF.Destroy();
	CAMERA_PERFORMANCE_REF.Destroy();
	CHARADEFINE_REF.Destroy();
	CHARAHP_REF.Destroy();
	CHARASTAMINA_REF.Destroy();
	CRYSTALFRAGMENT_REF.Destroy();
	CRYSTALFRAGMENTSPAWNER_REF.Destroy();
	GAME_REF.Destroy();
	wSetting.Destroy();
    GTime.Destroy();
    InputRef::Inst().Destroy();
	UI_ButtonHintRef::Inst().Destroy();
	NetworkRef::Inst().Destroy();

	PtrUtil::SafeDelete(nameText);

#ifdef IMGUI

	// 解放
	ImGui_ImplDXlib_Shutdown();
	ImGui::DestroyContext();

#endif // IMGUI

	Effkseer_End();
	DxLib_End();				// ＤＸライブラリ使用の終了処理

#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
#endif

	return 0;				// ソフトの終了 
}

#ifdef IMGUI

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	return false;
}

#endif // IMGUI

void BeginRecordPerformance()
{
	isRefreshTick = false;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
}

void EndRecordPerformance()
{
	QueryPerformanceCounter(&end);
	isRefreshTick = true;
}