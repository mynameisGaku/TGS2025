#pragma once

// ◇汎用
#include "config.h"
#include "../Source/Util/Transform.h"
#include <list>
#include <vector>

// ◇個別で必要な物
#include "stageDefine.h"
#include "stageObject.h"

/// <summary>
/// ステージオブジェクトを管理するクラス
/// </summary>
namespace StageObjectManager {

	//==========================================================================================
	// ▼各種関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// Updateの前に一度だけ呼び出される
	/// </summary>
	void Start();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	//==========================================================================================
	// ▼当たり判定関連

	/// <summary>
	/// カプセルと、StageObjectとの当たり判定をする
	/// </summary>
	/// <param name="p1">カプセルの点1</param>
	/// <param name="p2">カプセルの点2</param>
	/// <param name="r">半径</param>
	/// <param name="push">押し返すベクトル</param>
	/// <returns>あたったらTrue</returns>
	bool CollCheckCapsule(Vector3 p1, Vector3 p2, float r, Vector3* push);

	/// <summary>
	/// 描画用モデルと当たり判定用モデルを切り替える
	/// </summary>
	void HitModelDrawSwitch();

	//==========================================================================================
	// ▼データ管理関連

	/// <summary>
	/// Csvファイルからステージのデータを読み込む
	/// </summary>
	void LoadToCsv(const std::string& filename);

	/// <summary>
    /// Jsonファイルからステージのデータを読み込む
	/// </summary>
	void LoadFromJson(const std::string& filename);

	/// <summary>
	/// Csvファイルにステージのデータを保存する
	/// </summary>
	void SaveToCsv();

    /// <summary>
    /// Jsonファイルにステージのデータを保存する
    /// </summary>
    void SaveToJson();

	/// <summary>
	/// Csvファイルにステージのデータを書き出す
	/// </summary>
	void OutPutToCsv(const std::string& filename);

	/// <summary>
	/// データの保存と読み込みを一斉に行う
	/// </summary>
	void SaveAndLoad();

	/// <summary>
	/// Csvデータから再度読み込みを行う
	/// </summary>
	void ReloadStageData();

	//==========================================================================================
	// ▼セッター

	void SetModelFilePath(const std::string& filename);

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// 指定したIDのステージオブジェクトの情報を取得する
	/// </summary>
	/// <param name="id">取得したいステージオブジェクトのID</param>
	/// <returns>指定したIDが存在する場合、その情報を返す。存在しない場合、nullptrを返す</returns>
	StageObject* GetStageObject(int id);

	/// <summary>
	/// 配置されているステージオブジェクトのインスタンスを持つリストを取得する
	/// </summary>
	std::vector<StageObject*> StageObjects();

	/// <summary>
	/// 配置されているステージオブジェクトの種類名を持つリストを取得
	/// </summary>
	std::vector<std::string> StageObjectsTheString();

	//==========================================================================================
	// ▼ステージエディット機能

	/// <summary>
	/// ステージオブジェクトを生成する
	/// </summary>
	/// <param name="type">ステージオブジェクトの種類</param>
	/// <param name="transform">生成する座標・回転・拡縮の値</param>
	/// <param name="collider">当たり判定を付けるか</param>
	void Create(StageObjInfo info, const Transform& transform, bool collider = false);
	void CreateTheString(const std::string& objName);

	/// <summary>
	/// 指定したIDのステージオブジェクトを削除する
	/// </summary>
	/// <param name="id">削除するステージオブジェクトのID</param>
	/// <returns>削除が完了したらTrue</returns>
	bool Erase(int id);
	bool EraseTheString(const std::string& id);

	/// <summary>
	/// 全てのステージオブジェクトを消去する
	/// </summary>
	void EraseAll();

	/// <summary>
	/// ステージオブジェクトを削除した際に、生じた隙間を埋める。
	/// </summary>
	/// <param name="id">削除されたステージオブジェクトのID</param>
	void ShiftID(int id);

	/// <summary>
	/// 編集モードの際の描画処理
	/// </summary>
	void DrawEditMode();

#ifdef IMGUI

	/// <summary>
	/// デバッグで使用するImGui関連の機能の初期化を行う(ステージエディット機能)
	/// </summary>
	void InitImGui();

	/// <summary>
	/// デバッグで使用するImGui関連の機能を更新する
	/// </summary>
	void UpdateImGui();

#endif // IMGUI
}
