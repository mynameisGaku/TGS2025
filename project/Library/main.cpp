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

#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "App.h"
#include "../Source/config.h"
#include "../Source/Util/Utils.h"
#include "../Source/Font.h"

#ifdef IMGUI

#include "../vendor/ImGui/imgui.h"
#include "../vendor/ImGui/imgui_impl_dx11.h"
#include "../vendor/ImGui/imgui_impl_win32.h"

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // IMGUI

#define CoGVersion (2.2)

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);
	SetOutApplicationLogValidFlag(FALSE); // ログを出さない

	SetMainWindowText(WINDOW_NAME);
	SetWindowSizeExtendRate(WINDOW_EXTEND);
	ChangeWindowMode(WINDOW_MODE); // Windowモードの場合

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetZBufferBitDepth(32);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

#ifdef IMGUI

	SetHookWinProc(WndProc);	//プロシージャの設定
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;       // Enable Gamepad Controls
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui_ImplWin32_Init(DxLib::GetMainWindowHandle());
	ImGui_ImplDX11_Init((ID3D11Device*)DxLib::GetUseDirect3D11Device(), (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext());
	
#endif // IMGUI

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Set2DSetting(Screen::WIDTH, Screen::HEIGHT);

	SetFontSize(32);
	Font::Load("data/font/", "sazanami_mincho.ttf", "さざなみ明朝");

	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	AppInit();

	int mStartTime = GetNowCount();

	while (true) {
		int cur = GetNowCount();
		if (cur < mStartTime + 16) //120fps対策
			continue;
		mStartTime = cur;

#ifdef IMGUI

		//※アップデートの最初に呼ぶ
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
#endif // IMGUI

		AppUpdate();
		ClearDrawScreen();
		AppDraw();
		if (ProcessMessage() == -1 || IsExit())
			break;
		ScreenFlip();

#ifdef IMGUI
		
		//※アップデートの最後に呼ぶ
		ImGui::EndFrame();

#endif // IMGUI

	}
	AppRelease();

#ifdef IMGUI

	// 解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
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