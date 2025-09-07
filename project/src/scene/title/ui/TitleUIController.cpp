#include "TitleUIController.h"
#include "TitleUICanvas.h"
#include "TitleUIGridCursor.h"
#include "TitleUI.h"

#include <src/util/enum/EnumUtil.h>
#include <vendor/nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <src/util/logger/Logger.h>
#include <src/util/file/json/VectorJson.h>
#include <src/util/transform/RectTransform.h>
#include <src/util/file/resource_loader/resourceLoader.h>
#include <framework/app.h>
#include <src/util/sound/SoundManager.h>

#include <src/common/camera/CameraManager.h>
#include <vendor/mINI/ini.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "framework/App.h"
#include "src/util/font/Font.h"
#include "src/common/setting/window/WindowSetting.h"
#include "src/util/file/json/settings_json.h"
#include "src/util/file/ini/settings_ini.h"
#include "src/util/file/FileUtil.h"

#include "src/reference/bloom/BloomRef.h"
#include "src/reference/camera/CameraDefineRef.h"
#include "src/reference/camera/CameraPerformanceRef.h"
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
#include <src/util/restart/Restart.h>
#ifdef IMGUI
#include "vendor/imgui/imgui_impl_dxlib.hpp"
#endif // IMGUI

using json = nlohmann::json;
namespace fs = std::filesystem;

TitleUIController::TitleUIController()
{
	m_pGridCursor = new TitleUIGridCursor();
	m_pGridCursor->SetImage(ResourceLoader::LoadGraph("data/Img/UI/Title/Cursor.png"));
	m_pGridCursor->SetOffset(Vector2(-120, -30));
	m_pGridCursor->Activate();

	m_pCurrentCanvas = nullptr;

	subscribeFunctions();
}

TitleUIController::~TitleUIController()
{
	for (auto& canvas : m_CanvasList)
	{
		delete canvas.second;
	}
	m_CanvasList.clear();
	if (m_pGridCursor)
	{
		delete m_pGridCursor;
		m_pGridCursor = nullptr;
	}
	m_pCurrentCanvas = nullptr;
}

void TitleUIController::LoadCanvasFromJson(const std::string& file)
{
	std::ifstream ifs(file);
	if (!ifs.is_open())
	{
		Logger::FormatErrorLog("Failed to open JSON file: {}", file);
		return;
	}

	json root;
	ifs >> root;

	// Canvas
	auto* canvas = new TitleUICanvas();
	UI_TITLE_CANVAS_DESC canvasDesc{};
	Vector2 begin{};
	Vector2 end{};
	from_json(root["Begin"], begin);
	from_json(root["End"], end);

	canvasDesc.NAME = root.value("CanvasName", "NoName");
	canvasDesc.BEGIN = begin;
	canvasDesc.END = end;
	canvasDesc.IS_FIT_SCREEN = root.value("IsFitScreen", false);
	canvasDesc.IS_DEFAULT_ACTIVATE = root.value("IsDefaultActivate", false);
	canvasDesc.PREV_NAME = root.value("PrevCanvas", "");

	canvas->Init(canvasDesc, this);
	if (canvasDesc.IS_DEFAULT_ACTIVATE)
		canvas->Activate();

	// UI
	for (const auto& uiData : root["UIList"])
	{
		UI_TITLE_DESC desc{};

		desc.NAME = uiData.value("NAME", "NoName");
		desc.GRAPH_PATH = uiData.value("GRAPH_PATH", "");
		desc.OFFSET_X_FROM_ANCHOR = uiData.value("OFFSET_X_FROM_ANCHOR", 0);
		desc.OFFSET_Y_FROM_ANCHOR = uiData.value("OFFSET_Y_FROM_ANCHOR", 0);
		desc.GRAPH_SRC_X = uiData.value("GRAPH_SRC_X", 0);
		desc.GRAPH_SRC_Y = uiData.value("GRAPH_SRC_Y", 0);
		desc.GRAPH_DEST_X = uiData.value("GRAPH_DEST_X", 0);
		desc.GRAPH_DEST_Y = uiData.value("GRAPH_DEST_Y", 0);
		desc.INDEX_X = uiData.value("INDEX_X", INT_MAX);
		desc.INDEX_Y = uiData.value("INDEX_Y", INT_MAX);
		desc.Description = uiData.value("Description", "");
		desc.IS_SELECTABLE = uiData.value("IS_SELECTABLE", false);

		const std::string anchorStr = uiData.value("ANCHORPOINT",
			uiData.value("ANCHOR", "TUI_CANVAS_ANCHOR_POINT_TOP_LEFT"));
		desc.ANCHOR = EnumUtil::ToEnum<TUI_CANVAS_ANCHOR_POINT>(
			anchorStr, TUI_CANVAS_ANCHOR_POINT_TOP_LEFT);

		desc.COLLISION = EnumUtil::ToEnum<TUI_COLLISION_MODE>(
			uiData.value("COLLISION", "TUI_COLLISION_MODE_NONE"),
			TUI_COLLISION_MODE_NONE
		);

		desc.IsVisible = uiData.value("IsVisible", true);
		desc.RotationDeg = uiData.value("Rotation", 0.0);
		desc.Scale = uiData.value("Scale", 1.0);
		desc.Opacity = uiData.value("Opacity", 1.0);
		desc.ZIndex = uiData.value("ZIndex", 0);
		desc.LockAspectRatio = uiData.value("LockAspectRatio", true);
		desc.PivotX = uiData.value("PivotX", 0.5);
		desc.PivotY = uiData.value("PivotY", 0.5);

		if (uiData.contains("EVENTS"))
		{
			for (const auto& e : uiData["EVENTS"])
			{
				TUI_EVENT ev;
				ev.Event = e.value("Event", "");
				ev.Description = e.value("Description", "");
				ev.Argument = e.value<nlohmann::json>("Argument", nlohmann::json::object());
				ev.Timing = EnumUtil::ToEnum<TUI_EVENT_TRIGGER_TIMING>(
					e.value("Timing", "TUI_EVENT_TRIGGER_TIMING_ENTER"),
					TUI_EVENT_TRIGGER_TIMING_ENTER
				);
				desc.EVENTS.push_back(ev);
			}
		}

		TitleUI ui;
		ui.Init(desc, canvas);
		canvas->AddUI(ui);
	}

	m_CanvasList[canvasDesc.NAME] = canvas;
}

void TitleUIController::LoadCanvasesFromJson(const std::string& path)
{
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".json")
		{
			LoadCanvasFromJson(entry.path().string());
		}
	}
}

void TitleUIController::Update()
{
	if (!m_IsActive)
		return;

	if (m_pCurrentCanvas && !m_pCurrentCanvas->IsActive())
	{
		m_pCurrentCanvas = nullptr;
	}
	if (!m_pCurrentCanvas)
	{
		for(auto& canvas : m_CanvasList)
		{
			if (canvas.second->IsActive())
			{
				m_pCurrentCanvas = canvas.second;
				break;
			}
		}
	}
	if (m_pCurrentCanvas)
	{
		m_pCurrentCanvas->Update();
	}

	m_pGridCursor->Update();
}

void TitleUIController::Draw()
{
	if (!m_IsActive)
		return;

	if (m_pCurrentCanvas)
	{
		m_pCurrentCanvas->Draw();
	}

	m_pGridCursor->Draw();
}

void TitleUIController::TriggerEvent(TUI_EVENT& event, const nlohmann::json& argument)
{
	if (m_EventHandlers.find(event.Event) != m_EventHandlers.end())
	{
		nlohmann::json argumentCopy;
		argumentCopy["Duration"] = event.Duration;
		argumentCopy["Counter"] = event.Counter;
		argumentCopy["Event"] = argument;

		(m_EventHandlers[event.Event])(argumentCopy);

		event.Counter += GTime.deltaTime;
		if (event.Counter >= event.Duration)
		{
			event.Counter = 0.0f;
		}
	}
	else
	{
		Logger::FormatErrorLog("Event '{}' not found in TitleUIController.", event.Event);
		return;
	}
}

void TitleUIController::subscribeFunctions()
{
	SUBSCRIBE_FUNCTION("ActivateCanvas", activateCanvas);
	SUBSCRIBE_FUNCTION("Scaling", scaling);
	SUBSCRIBE_FUNCTION("GameStart", gameStart);
	SUBSCRIBE_FUNCTION("PlaySound", playSound);
	SUBSCRIBE_FUNCTION("Exit", exit);
	SUBSCRIBE_FUNCTION("SetWindowMode", setWindowMode);
}

void TitleUIController::activateCanvas(const std::string& canvasName)
{
	if (canvasName.empty() || m_CanvasList.find(canvasName) == m_CanvasList.end())
	{
		Logger::FormatErrorLog("Canvas with name '{}' not found.", canvasName);
		return;
	}

	if (m_pCurrentCanvas) m_pCurrentCanvas->Deactivate();
	m_pCurrentCanvas = m_CanvasList[canvasName];
	m_pCurrentCanvas->Activate();

	m_pGridCursor->MoveTo(0, 0);
	m_pGridCursor->Deactivate();
} 

void TitleUIController::activateCanvas(nlohmann::json argument)
{
	nlohmann::json event = argument["Event"];
	std::string name = event.value("CanvasName", "");

	if (name.empty() || m_CanvasList.find(name) == m_CanvasList.end())
	{
		Logger::FormatErrorLog("Canvas with name '{}' not found.", name);
		return;
	}

	activateCanvas(name);
}

void TitleUIController::scaling(nlohmann::json argument)
{
	nlohmann::json event = argument["Event"];
	std::string canvasName = event.value("CanvasName", "");
	std::string targetUIName = event.value("TargetUIName", "");
	if (m_CanvasList.find(canvasName) == m_CanvasList.end())
	{
		Logger::FormatErrorLog("Canvas with name '{}' not found.", canvasName);
		return;
	}



	Logger::FormatErrorLog("UI with name '{}' not found in canvas '{}'.", targetUIName, canvasName);
}

void TitleUIController::gameStart(nlohmann::json argument)
{
	nlohmann::json event = argument["Event"];
	std::string sceneName = event.value("SceneName", "");
	if (sceneName.empty())
	{
		Logger::FormatErrorLog("Scene name is empty. Cannot start game.");
		return;
	}

	SceneManager::ChangeScene(sceneName);
}

void TitleUIController::exit(nlohmann::json argument)
{
	Exit();
}

void TitleUIController::playSound(nlohmann::json argument)
{
	nlohmann::json event = argument["Event"];
	std::string soundName = event.value("SoundName", "");
	if (soundName.empty())
	{
		Logger::FormatErrorLog("Sound name is empty. Cannot play sound.");
		return;
	}

	SoundManager::Play(soundName, soundName);
}


void TitleUIController::setWindowMode(nlohmann::json argument)
{
	nlohmann::json event = argument["Event"];

	std::string modeStr = event.value("Mode", "WindowMode");

	int w{}, h{};

	if (modeStr == "FullScreen")
	{
		w = 1920;
		h = 1080;
	}

	if (modeStr == "WindowMode")
	{
		w = 1600;
		h = 900;
	}

	if (modeStr == "DualScreen")
	{
		w = 3840;
		h = 1080;
	}

	// first, create a file instance
	mINI::INIFile file("window.ini");

	// next, create a structure that will hold data
	mINI::INIStructure ini;

	// now we can read the file
	file.read(ini);

	// read a value
	//std::string& amountOfApples = ini["fruits"]["apples"];

	// update a value
	ini["Main_Window"]["Width"] = std::to_string(w);
	ini["Main_Window"]["Height"] = std::to_string(h);

	// add a new entry
	//ini["Main_Window"]["bananas"] = "100";

	// write updates to file
	file.write(ini);

	auto cleanup = []() {

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
		WindowSetting::Inst().Destroy();
		GTime.Destroy();
		InputRef::Inst().Destroy();
		UI_ButtonHintRef::Inst().Destroy();
		NetworkRef::Inst().Destroy();
		PtrUtil::SafeDelete(nameText);
		CameraPerformanceRef::Inst()->Destroy();

#ifdef IMGUI

		// 解放
		ImGui_ImplDXlib_Shutdown();
		ImGui::DestroyContext();

#endif // IMGUI

		Effkseer_End();
		DxLib_End();				// ＤＸライブラリ使用の終了処理
		};

	Restart::RestartAndExit(__argc, __argv, cleanup);
}
