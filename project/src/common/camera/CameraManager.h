#pragma once
#include "src/common/camera/Camera.h"

#include "src/common/network/user/User.h"
#include "src/config/imgui/ImGuiConfig.h"

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
	Camera* CreateCamera(int charaindex, const User& user);
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

	void SetCurrentDrawingCameraID(int index);

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
	/// カメラの情報を取得する
	/// </summary>
	/// <param name="user">ユーザーのインスタンス</param>
	/// <returns>カメラのポインター</returns>
	Camera* GetCamera(const User& user);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Camera* GetCameraDrawing();

	/// <summary>
	/// 全てのカメラのリストを取得する
	/// </summary>
	std::vector<Camera*> AllCameras();

	/// <summary>
	/// 画面が分割されているか取得する
	/// </summary>
	bool IsScreenDivision();

	/// <summary>
	/// 描画領域の開始地点を取得する
	/// </summary>
	Vector2 GetDrawingAreaPos_CameraIndex(int index);

	/// <summary>
	/// 描画領域の大きさを取得する
	/// </summary>
	Vector2 GetDrawingAreaSize_CameraIndex(int index);

	/// <summary>
	/// カメラ数によって分割された2Dベクトルを取得します。
	/// </summary>
	/// <returns>カメラの数で分割された結果の2次元ベクトル。</returns>
	Vector2 GetDivedByCameraNum();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int GetCurrentDrawingCameraID();

	bool IsPlayingPerformance();

	//================================================================================
	// ▼デバッグ機能

	/// <summary>
	/// ステートを変更する(string型)
	/// </summary>
	/// <param name="state">変更するステート名</param>
	//void CameraChangeStateTheString(const std::string& state);

#ifdef _DEBUG
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
#endif

}