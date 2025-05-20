 //<summary>
 // CoG�t���[�����[�N
 // WinMain()����n�܂�܂�
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

#define IMGUI
#ifdef IMGUI
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_dx11.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include "vendor/imgui/imgui_ja_gryph_ranges.cpp"

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "src/util/debug/imgui/imGuiManager.h"

#endif // IMGUI

#include <EffekseerForDXLib.h>
#include "framework/App.h"
#include "src/Util/Utils.h"
#include "src/util/font/Font.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/util/file/json/settings_json.h"
#include "src/util/file/ini/settings_ini.h"
#include "src/util/file/FileUtil.h"

#define CoGVersion (2.2)

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    auto& wSetting = WindowSetting::Inst();
	std::string path = "";
    SearchFilePath("window.ini", path);
    wSetting.Load(path);

	SetGraphMode((int)wSetting.width, (int)wSetting.height, 32);
	SetOutApplicationLogValidFlag(FALSE); // ���O���o���Ȃ�

	SetMainWindowText(wSetting.name.c_str());
	SetWindowSizeExtendRate((double)wSetting.extend);
	ChangeWindowMode(wSetting.isFull); // Window���[�h�̏ꍇ

#ifdef IMGUI
	SetHookWinProc(WndProc);	//�v���V�[�W���̐ݒ�
#endif // IMGUI

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	SetZBufferBitDepth(32);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	if (Effekseer_Init(18000) == -1)
	{
		DxLib_End();
		return -1;
	}

#ifdef IMGUI

	// ImGui
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//ImFont* font = io.Fonts->AddFontFromFileTTF("data/font/DotGothic16-Regular.ttf", 18.0f, NULL, glyphRangesJapanese);
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(DxLib::GetMainWindowHandle());
		ImGui_ImplDX11_Init((ID3D11Device*)DxLib::GetUseDirect3D11Device(), (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext());
	}
#endif // IMGUI

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Set2DSetting((int)wSetting.width, (int)wSetting.height);

	SetFontSize(32);
	Font::Load("data/font/", "sazanami_mincho.ttf", "�����Ȃݖ���");

	SetDrawScreen(DX_SCREEN_BACK);
	SetAlwaysRunFlag(TRUE);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	AppInit();

	int mStartTime = GetNowCount();

	while (true) {
		int cur = GetNowCount();
		if (cur < mStartTime + 16) //120fps�΍�
			continue;
		mStartTime = cur;

#ifdef IMGUI

		//���A�b�v�f�[�g�̍ŏ��ɌĂ�
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
		
		//���A�b�v�f�[�g�̍Ō�ɌĂ�
		ImGui::EndFrame();
		ImGui::UpdatePlatformWindows();

#endif // IMGUI

	}
	AppRelease();

#ifdef IMGUI

	// ���
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#endif // IMGUI

	Settings_json::Destroy();

	Effkseer_End();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

#ifdef _DEBUG
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
#endif

	return 0;				// �\�t�g�̏I�� 
}

#ifdef IMGUI

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	return false;
}

#endif // IMGUI