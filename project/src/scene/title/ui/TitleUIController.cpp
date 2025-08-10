#include "TitleUIController.h"
#include "TitleUICanvas.h"
#include "TitleUIGridCursor.h"
#include "TitleUI.h"

#include <src/util/enum/EnumUtil.h>
#include <vendor/nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <src/util/logger/Logger.h>
#include <src/util/time/GameTime.h>
#include <src/util/file/json/VectorJson.h>
#include <src/util/transform/RectTransform.h>

using json = nlohmann::json;
namespace fs = std::filesystem;

TitleUIController::TitleUIController()
{
	m_pGridCursor		= new TitleUIGridCursor();
	m_pCurrentCanvas	= nullptr;

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

	// Canvasの生成と初期化
	auto* canvas = new TitleUICanvas();
	UI_TITLE_CANVAS_DESC canvasDesc{};
	Vector2 begin	{};
	Vector2 end		{};
	from_json(root["Begin"], begin);
	from_json(root["End"],	end);
	
	canvasDesc.NAME					= root.value("CanvasName", "NoName");
	canvasDesc.BEGIN				= begin;
	canvasDesc.END					= end;
	canvasDesc.IS_FIT_SCREEN		= root.value("IsFitScreen", false);
	canvasDesc.IS_DEFAULT_ACTIVATE	= root.value("IsDefaultActivate", false);

	canvas->Init(canvasDesc, this);

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
		desc.COLLISION = EnumUtil::ToEnum<TUI_COLLISION_MODE>(
			uiData.value("COLLISION", "TUI_COLLISION_MODE_NONE"),
			TUI_COLLISION_MODE_NONE
		);
		desc.ANCHOR = EnumUtil::ToEnum<TUI_CANVAS_ANCHOR_POINT>(
			uiData.value("ANCHOR", "TUI_CANVAS_ANCHOR_POINT_TOP_LEFT"),
			TUI_CANVAS_ANCHOR_POINT_TOP_LEFT
		);

		// イベント読み込み
		if (uiData.contains("EVENTS"))
		{
			for (const auto& e : uiData["EVENTS"])
			{
				TUI_EVENT event;
				event.Event			= e.value("Event", "");
				event.Description	= e.value("Description", "");
				event.Argument		= e.value<nlohmann::json>("Argument", nlohmann::json::object());
				event.Timing		= EnumUtil::ToEnum<TUI_EVENT_TRIGGER_TIMING>(
					e.value("Timing", "TUI_EVENT_TRIGGER_TIMING_ENTER"),
					TUI_EVENT_TRIGGER_TIMING_ENTER
				);

				desc.EVENTS.push_back(event);
			}
		}

		// UI生成・追加
		TitleUI ui;
		ui.Init(desc, canvas);
		canvas->AddUI(ui);
	}

	m_CanvasList[canvasDesc.NAME]	= canvas;
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
	// 現在のキャンバスが有効かチェック
	if (m_pCurrentCanvas && !m_pCurrentCanvas->IsActive())
	{
		m_pCurrentCanvas = nullptr;
	}
	// キャンバスが設定されていない場合、最初のキャンバスを設定
	if (!m_pCurrentCanvas && !m_CanvasList.empty())
	{
		m_pCurrentCanvas = m_CanvasList.begin()->second;
	}
	// キャンバスが有効な場合、更新処理を呼び出す
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

	// キャンバスが有効な場合、描画処理を呼び出す
	if (m_pCurrentCanvas)
	{
		m_pCurrentCanvas->Draw();
	}
}

void TitleUIController::TriggerEvent(TUI_EVENT& event, const nlohmann::json& argument)
{
	if (m_EventHandlers.find(event.Event) != m_EventHandlers.end())
	{
		nlohmann::json argumentCopy;
		argumentCopy["Duration"] = event.Duration;
		argumentCopy["Counter"] = event.Counter;
		argumentCopy["Event"] = argument;

		// イベントハンドラーを呼び出す
		(m_EventHandlers[event.Event])(argumentCopy);

		event.Counter += GTime.deltaTime;
		if (event.Counter >= event.Duration)
			event.Counter = 0.0f;  // イベントのカウンターをリセット
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
}

void TitleUIController::activateCanvas(nlohmann::json argument)
{
	nlohmann::json event = argument["Event"];
	std::string name = event.at("CanvasName").get<std::string>();

	if (m_CanvasList.find(name) != m_CanvasList.end())
	{
		m_pCurrentCanvas->Deactivate();
		m_pCurrentCanvas = m_CanvasList[name];
		m_pCurrentCanvas->Activate();
	}
	else
	{
		Logger::FormatErrorLog("Canvas with name '{}' not found.", name);
		return;
	}
}

void TitleUIController::scaling(nlohmann::json argument)
{
	nlohmann::json event = argument["Event"];
	std::string canvasName = event.at("CanvasName").get<std::string>();
	std::string targetUIName = event.at("TargetUIName").get<std::string>();
	if (m_CanvasList.find(canvasName) != m_CanvasList.end())
	{

		Logger::FormatErrorLog("UI with name '{}' not found in canvas '{}'.", targetUIName, canvasName);
	}
	else
	{
		Logger::FormatErrorLog("Canvas with name '{}' not found.", canvasName);
		return;
	}
}

void TitleUIController::gameStart(nlohmann::json argument)
{
	nlohmann::json event = argument["Event"];
	std::string sceneName = event.at("SceneName").get<std::string>();
	if (sceneName.empty())
	{
		Logger::FormatErrorLog("Scene name is empty. Cannot start game.");
		return;
	}

	SceneManager::ChangeScene(sceneName);
}
