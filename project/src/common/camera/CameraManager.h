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

	void CreateCamera(bool view);

	/// <summary>
	/// カメラの再初期化を行う
	/// </summary>
	/// <param name="number">カメラの番号</param>
	void ResetCamera(const int& number);

	/// <summary>
	/// カメラの番号が正しいか確認する。
	/// </summary>
	/// <param name="number">カメラの番号</param>
	/// <param name="number"></param>
	/// <returns>番号が一致したカメラが存在する場合true</returns>
	bool CheckNumber(const int& number);

	/// <summary>
	/// カメラのステートを変更する
	/// </summary>
	/// <param name="number">カメラの番号</param>
	/// <param name="state">変更するステート</param>
	void ChangeStateCamera(const int& number, void(Camera::* state)(FSMSignal));

	//================================================================================
	// ▼セッター

	/// <summary>
	/// カメラワークを設定する
	/// </summary>
	/// <param name="number">カメラの番号</param>
	/// <param name="type">カメラワークの種類</param>
	void SetCameraWork(const int& number, const std::string& type);

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
	Camera* GetCamera(const int& number);

	/// <summary>
	/// 全てのカメラのリストを取得する
	/// </summary>
	std::vector<Camera*> AllCameras();

	//================================================================================
	// ▼デバッグ機能

	/// <summary>
	/// ステートを変更する(string型)
	/// </summary>
	/// <param name="state">変更するステート名</param>
	void CameraChangeStateTheString(const std::string& state);


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