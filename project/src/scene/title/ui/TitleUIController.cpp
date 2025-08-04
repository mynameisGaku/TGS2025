#include "TitleUIController.h"
#include "TitleUICanvas.h"
#include "TitleUI.h"
#include "EnumUtil.h"

#include <vendor/nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

TitleUIController::TitleUIController()
{
	m_pGridCursor		= new TitleUIGridCursor();
	m_pCurrentCanvas	= nullptr;
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
	if (!ifs)
	{
		printfDx("Failed to open JSON file: %s\n", file.c_str());
		return;
	}

	json root;
	ifs >> root;

	// Canvasの生成と初期化
	auto* canvas = new TitleUICanvas();
	canvas->Init(this);

	for (const auto& uiData : root["UI_LIST"])
	{
		UI_TITLE_DESC desc{};

		desc.GRAPH_HANDLE	= uiData.value("GRAPH_HANDLE", -1);
		desc.GRAPH_POS_X	= uiData.value("GRAPH_POS_X", 0);
		desc.GRAPH_POS_Y	= uiData.value("GRAPH_POS_Y", 0);
		desc.GRAPH_SRC_X	= uiData.value("GRAPH_SRC_X", 0);
		desc.GRAPH_SRC_Y	= uiData.value("GRAPH_SRC_Y", 0);
		desc.GRAPH_DEST_X	= uiData.value("GRAPH_DEST_X", 0);
		desc.GRAPH_DEST_Y	= uiData.value("GRAPH_DEST_Y", 0);
		desc.INDEX_X		= uiData.value("INDEX_X", INT_MAX);  // 非選択 = INT_MAX
		desc.INDEX_Y		= uiData.value("INDEX_Y", INT_MAX);
		desc.DESCRIPTION	= uiData.value("DESCRIPTION", "");
		desc.COLLISION		= EnumUtil::ToEnum<TUI_COLLISION_MODE>(
			uiData.value("COLLISION", "TUI_COLLISION_MODE_NONE"),
			TUI_COLLISION_MODE_NONE
		);

		// イベント読み込み
		if (uiData.contains("EVENTS"))
		{
			for (const auto& e : uiData["EVENTS"])
			{
				TUI_EVENT event;
				event.EVENT			= e.value("EVENT", "");
				event.DESCRIPTION	= e.value("DESCRIPTION", "");
				event.TIMING		= EnumUtil::ToEnum<TUI_EVENT_TRIGGER_TIMING>(
					e.value("TIMING", "TUI_EVENT_TRIGGER_TIMING_ENTER"),
					TUI_EVENT_TRIGGER_TIMING_ENTER
				);

				event.IS_WAIT_FOR_COMPLETION = e.value("WAIT", false);
				desc.EVENTS.push_back(event);
			}
		}

		// UI生成・追加
		TitleUI ui;
		ui.Init(desc, canvas);
		canvas->AddUI(ui);
	}

	std::string canvasName		= root.value("CANVAS_NAME", file);
	m_CanvasList[canvasName]	= canvas;
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
