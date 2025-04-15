#pragma once
#include "imGuiNode_Base.h"

// ▼ImGuiの機能を使用するか
#ifdef IMGUI

// ◇汎用
#include <map>
#include <unordered_map>

// ◇個別で必要な物
#include "imGuiFunc.h"
#include "imGuiRoot.h"
#include "imGuiNode_Text.h"
#include "imGuiNode_SliderInt.h"
#include "imGuiNode_SliderFloat.h"
#include "imGuiNode_Button.h"
#include "imGuiNode_CheckBox.h"
#include "imGuiNode_DropDown.h"
#include "imGuiNode_Space.h"
#include "imGuiNode_ColorEdit4.h"
#include "imGuiNode_BeginChildren.h"
#include "imGuiNode_EndChildren.h"

/// <summary>
/// ImGuiの処理をまとめて管理する
/// デバッグウィンドウとインフォメーションウィンドウを持つ
/// </summary>
/// <author> ミッチ </author>
namespace ImGuiManager {

	//=============================================================================================
	// ▼各種関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
	
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui更新処理
	/// </summary>
	void ImGuiUpdate();

	/// <summary>
	/// ImGuiInfo更新処理
	/// </summary>
	void ImGuiInfoUpdate();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	
	/// <summary>
	/// 解放処理
	/// </summary>
	void Release();

	/// <summary>
	/// 実体が存在するか確認する。存在しなければ生成する。
	/// </summary>
	void CheckInstance();

	//=============================================================================================
	// ▼セッター

	/// <summary>
	/// 活動を行うかを設定する
	/// </summary>
	void SetIsActive(bool value);

	//=============================================================================================
	// ▼ゲッター

	/// <summary>
	/// 活動を行うかを取得する
	/// </summary>
	bool IsActive();

	/// <summary>
	/// ImGuiInfoの根っこを取得する
	/// </summary>
	const std::map<std::string, ImGuiRoot*>* const InfoRoots();

	/// <summary>
	/// ImGuiInfoのノードを取得する
	/// </summary>
	const std::map<std::string, ImGuiNode_Base*>* const InfoNodes();

	/// <summary>
	/// ImGuiの根っこを取得する
	/// </summary>
	const std::unordered_map<std::string, ImGuiRoot*>* const Roots();

	/// <summary>
	/// ImGuiのノードを取得する
	/// </summary>
	const std::unordered_map<std::string, ImGuiNode_Base*>* const Nodes();

	//=============================================================================================
	// ▼Node関連

	/// <summary>
	/// Nodeを追加する
	/// </summary>
	/// <param name="node">追加するNodeの実体</param>
	/// <returns>追加したNode</returns>
	ImGuiNode_Base* AddNode(ImGuiNode_Base* node);

	/// <summary>
	/// Nodeを検索する
	/// </summary>
	/// <param name="label">検索する名前</param>
	/// <returns>引数の名前と一致しているNode、無ければnullptr</returns>
	template<class C>
	C* FindNode(std::string label) {

		auto nodes = Nodes();

		if (nodes == nullptr)
			return nullptr;

		if ((*nodes).contains(label) == false)
			return nullptr;

		for (const auto& itr : *nodes) {
			if (itr.first == label)
				return dynamic_cast<C*>(itr.second);
		}

		return nullptr;
	}

	//=============================================================================================
	// ▼Root関連

	/// <summary>
	/// Rootを追加する
	/// </summary>
	/// <param name="root">追加するRootの実体</param>
	/// <returns>追加するRoot</returns>
	ImGuiRoot* AddRoot(ImGuiRoot* root);
	
	/// <summary>
	/// Rootを検索する
	/// </summary>
	/// <param name="label">検索する名前</param>
	/// <returns>引数の名前と一致しているRoot、無ければnullptr</returns>
	ImGuiRoot* FindRoot(std::string label);

	//=============================================================================================
	// ▼ImGuiInfo関連

	/// <summary>
	/// ImGuiInfoにNodeを追加する
	/// </summary>
	/// <param name="node">追加するNodeの実体</param>
	/// <returns>追加したNode</returns>
	ImGuiNode_Base* AddNodeInfo(ImGuiNode_Base* node);

	/// <summary>
	/// ImGuiInfoのNodeを検索する
	/// </summary>
	/// <param name="label">検索する名前</param>
	/// <returns>引数の名前と一致しているNode、無ければnullptr</returns>
	template<class C>
	C* FindNodeInfo(std::string label) {

		auto infoNodes = InfoNodes();

		if (infoNodes == nullptr)
			return nullptr;

		if ((*infoNodes).contains(label) == false)
			return nullptr;

		for (const auto& itr : *infoNodes) {
			if (itr.first == label)
				return dynamic_cast<C*>(itr.second);
		}

		return nullptr;
	}

	/// <summary>
	/// ImGuiInfoにRootを追加する
	/// </summary>
	/// <param name="root">追加するRootの実体</param>
	/// <returns>追加するRoot</returns>
	ImGuiRoot* AddRootInfo(ImGuiRoot* root);

	/// <summary>
	/// ImGuiInfoのRootを検索する
	/// </summary>
	/// <param name="label">検索する名前</param>
	/// <returns>引数の名前と一致しているRoot、無ければnullptr</returns>
	ImGuiRoot* FindRootInfo(std::string label);

	//=============================================================================================
	// ▼Delete関連

	/// <summary>
	/// Node、Rootから引数の名前と一致している物を削除する
	/// </summary>
	/// <param name="label">削除したいNode、Rootの名前</param>
	void Delete(std::string label);

	/// <summary>
	/// InfoNode、InfoRootから引数の名前と一致している物を削除する
	/// </summary>
	/// <param name="label">削除したいNode、Rootの名前</param>
	void DeleteInfo(std::string label);

}

#endif // IMGUI