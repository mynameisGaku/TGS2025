#pragma once

// ◇汎用
#include <vector>

// ◇個別で必要な物
#include "src/util/ui/UI_Base.h"
#include "src/util/ui/UI_Canvas.h"
#include "src/util/ui/UI_Text.h"
#include "src/util/ui/UI_Image.h"
#include "src/util/ui/UI_Button.h"
#include "src/util/ui/UI_Gauge.h"
#include "src/util/ui/UI_Slider.h"
#include "src/util/ui/UI_BillBoard.h"

/// <summary>
/// UIの情報を管理する
/// </summary>
/// <author> ミッチ </author>
namespace UI_Manager {

	//================================================================================
	// ▼各種関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 管理しているUIを更新する
	/// </summary>
	void Update();

	/// <summary>
	/// 管理しているUIを描画する(背面)
	/// </summary>
	void DrawBack();

	/// <summary>
	/// 管理しているUIを描画する(正面)
	/// </summary>
	void DrawFront();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 管理するUIを追加する
	/// </summary>
	/// <param name="uiData">UI情報</param>
	/// <returns>追加したUI情報のインスタンス</returns>
	UI_Canvas* Add(UI_Canvas* uiData, const bool& front = false);

	/// <summary>
	/// 管理しているUIを切り離す
	/// </summary>
	/// <param name="uiData">UI情報</param>
	/// <returns>切り離せた場合:True</returns>
	bool Detach(UI_Canvas* uiData);

	/// <summary>
	/// 管理しているUIを検索して取得する
	/// </summary>
	/// <param name="tag">検索するタグ</param>
	/// <returns>存在すればその実態を返す。無ければnullptrを返す。</returns>
	UI_Canvas* Find(const std::string& tag);

	template<class C>
	C* Find(const std::string& tag)
	{
		UI_Canvas* ui = Find(tag);
		if (ui == nullptr)
			return nullptr;

		return dynamic_cast<C*>(ui);
	}

	//================================================================================
	// ▼セッター

	/// <summary>
	/// 機能させるかを設定する
	/// </summary>
	void SetIsActive(const bool& value);

	/// <summary>
	/// 更新するかを設定する
	/// </summary>
	void SetIsUpdate(const bool& value);

	/// <summary>
	/// 描画するかを設定する
	/// </summary>
	void SetIsDraw(const bool& value);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// 機能させるかを取得する
	/// </summary>
	bool IsActive();

	/// <summary>
	/// 更新するかを取得する
	/// </summary>
	bool IsUpdate();

	/// <summary>
	/// 描画するかを取得する
	/// </summary>
	bool IsDraw();

	/// <summary>
	/// 描画優先度に応じた並び替えを行う
	/// </summary>
	void Sort();

	/// <summary>
	/// 描画優先度に応じた並び替えを行う処理
	/// </summary>
	void UI_Canvas_CombSort(std::vector<UI_Canvas*>& ui);

	/// <summary>
	/// 画面分割によるアンカー位置の設定を行う
	/// </summary>
	/// <param name="ui">アンカー位置の設定を行うUIのポインター</param>
	/// <param name="cameraIndex">カメラの番号</param>
	void SetAnchorPositionByScreenSplit(UI_Canvas* ui, int cameraIndex);
};
