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
#include "src/util/font/Font.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/util/file/json/settings_json.h"
#include "src/util/file/ini/settings_ini.h"
#include "src/util/file/FileUtil.h"

#include "src/reference/ball/BallRef.h"
#include "src/reference/bloom/BloomRef.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/reference/chara/CharaDefineRef.h"
#include "src/reference/chara/CharaHPRef.h"
#include "src/reference/chara/CharaStaminaRef.h"
#include "src/reference/crystal/CrystalFragmentRef.h"
#include "src/reference/crystal/CrystalFragmentSpawnerRef.h"
#include <src/reference/game/GameRef.h>
#include <src/util/time/GameTime.h>

#define CoGVersion (2.2)

LARGE_INTEGER freq;
LARGE_INTEGER start, end;
int timeRecordCounter = 0;
double tick = 0.0;
bool isRefreshTick = false;

void BeginRecordPerformance();
void EndRecordPerformance();

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
	ChangeWindowMode(wSetting.isWindow); // Window���[�h�̏ꍇ

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

		BeginRecordPerformance();

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

		ImGui::Begin("TimeRecord");
		ImGui::Text("time %lf[ms]", tick);
		ImGui::End();

		AppUpdate();
		ClearDrawScreen();
		AppDraw();
		if (ProcessMessage() == -1 || IsExit())
			break;
		ScreenFlip();

		EndRecordPerformance();

		if (++timeRecordCounter % 60 == 0)
			tick = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;

#ifdef IMGUI
		
		//���A�b�v�f�[�g�̍Ō�ɌĂ�
		ImGui::EndFrame();
		ImGui::UpdatePlatformWindows();

#endif // IMGUI

	}

	AppRelease();

	/*
	���t�@�����X���

	�����I�ɉ������K�v������܂��B
	*/
	Settings_json::Inst()->Destroy();
	Settings_ini::Inst().Destroy();
	BALL_REF.Destroy();
	BLOOM_REF.Destroy();
	CAMERADEFINE_REF.Destroy();
	CHARADEFINE_REF.Destroy();
	CHARAHP_REF.Destroy();
	CHARASTAMINA_REF.Destroy();
	CRYSTALFRAGMENT_REF.Destroy();
	CRYSTALFRAGMENTSPAWNER_REF.Destroy();
	GAME_REF.Destroy();
	wSetting.Destroy();
	GTime.Destroy();

#ifdef IMGUI

	// ���
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

#endif // IMGUI

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