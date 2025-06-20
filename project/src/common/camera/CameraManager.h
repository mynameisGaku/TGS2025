#pragma once
#include "src/common/camera/Camera.h"

using namespace CameraDefine;

/// <summary>
/// カメラの管理を行う
/// </summary>
namespace CameraManager {

	//================================================================================
	// ▼各種関数

	void Init();
	void Update();
	void Draw();
	void Release();

	/// <summary>
	/// カメラの生成を行う
	/// </summary>
	/// <returns>生成したカメラの実体</returns>
	Camera* CreateCamera();

	/// <summary>
	/// カメラの再初期化を行う
	/// </summary>
	/// <param name="number">カメラの番号</param>
	void ResetCamera(int number);

	/// <summary>
	/// カメラの番号が正しいか確認する。
	/// </summary>
	/// <param name="number">カメラの番号</param>
	/// <param name="number"></param>
	/// <returns>番号が一致したカメラが存在する場合true</returns>
	bool CheckNumber(int number);

	/// <summary>
	/// カメラのステートを変更する
	/// </summary>
	/// <param name="number">カメラの番号</param>
	/// <param name="state">変更するステート</param>
	void ChangeStateCamera(int number, void(Camera::* state)(FSMSignal));

	/// <summary>
	/// カメラ描画を指定された位置とサイズで分割します
	/// </summary>
	/// <param name="x">分割領域の左上隅のX座標</param>
	/// <param name="y">分割領域の左上隅のY座標</param>
	/// <param name="w">分割領域の幅</param>
	/// <param name="h">分割領域の高さ</param>
	void CameraScreenDivision(int x, int y, int w, int h);

	/// <summary>
	/// カメラ描画を指定された位置とサイズで分割してから、描画します
	/// </summary>
	/// <param name="x">分割領域の左上隅のX座標</param>
	/// <param name="y">分割領域の左上隅のY座標</param>
	/// <param name="w">分割領域の幅</param>
	/// <param name="h">分割領域の高さ</param>
	/// <param name="number">描画を行うカメラの番号</param>
	void CameraScreenDivisionDraw(int x, int y, int w, int h, int number);

	/// <summary>
	/// 画面分割を適用する
	/// </summary>
	void ApplyScreenDivision();

	/// <summary>
	/// 既定の画面サイズに戻す
	/// </summary>
	void DefaultScreenSize();

	//================================================================================
	// ▼セッター

	/// <summary>
	/// カメラワークを設定する
	/// </summary>
	/// <param name="number">カメラの番号</param>
	/// <param name="type">カメラワークの種類</param>
	void SetCameraWork(int number, const std::string& type);

	/// <summary>
	/// 画面分割処理を行うかを設定する
	/// </summary>
	void SetIsScreenDivision(bool value);

	//================================================================================
	// ▼ゲッター

	/// <summary>
	/// メインカメラを取得する
	/// </summary>
	/// <returns></returns>
	Camera* MainCamera();

	/// <summary>
	/// カメラの情報を取得する
	/// </summary>
	/// <param name="number">カメラの番号</param>
	/// <returns>カメラのポインター</returns>
	Camera* GetCamera(int number);

	/// <summary>
	/// 全てのカメラのリストを取得する
	/// </summary>
	std::vector<Camera*> AllCameras();

	/// <summary>
	/// 画面が分割されているか取得する
	/// </summary>
	bool IsScreenDivision();

	/// <summary>
	/// 画面分割の開始地点を取得する
	/// </summary>
	Vector2 GetScreenDivisionPos();

	/// <summary>
	/// 画面分割の大きさを取得する
	/// </summary>
	Vector2 GetScreenDivisionSize();

	/// <summary>
	/// 画面分割の開始地点を取得する
	/// </summary>
	Vector2 GetScreenDivisionPos_CameraIndex(int index);

	/// <summary>
	/// 画面分割の中心座標を取得する
	/// </summary>
	Vector2 GetScreenDivisionCenter();

	Vector2 GetDivedByCameraNum();

	//================================================================================
	// ▼デバッグ機能

	/// <summary>
	/// ステートを変更する(string型)
	/// </summary>
	/// <param name="state">変更するステート名</param>
	//void CameraChangeStateTheString(const std::string& state);

#ifdef _DEBUG
#ifndef IMGUI
#define IMGUI
#endif
#endif
#ifdef IMGUI

	/// <summary>
	/// デバッグで使用するImGui関連の機能を初期化する
	/// </summary>
	void InitImGuiNode();

	/// <summary>
	/// デバッグで使用するImGui関連の機能を更新する
	/// </summary>
	void UpdateImGuiNode();

#endif // IMGUI

}