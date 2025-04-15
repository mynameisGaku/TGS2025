#pragma once

// ◇継承元
#include "imGuiNode_Base.h"

// ▼ImGuiの機能を使用するか
#ifdef IMGUI

// ◇汎用
#include <map>
#include <vector>

// ◇個別で必要な物
#include "imGuiNode_BeginChildren.h"
#include "imGuiNode_EndChildren.h"

/// <summary>
/// ImGuiNodeを繋げる際の根元の機能を持つクラス
/// </summary>
/// <author> ミッチ </author>
class ImGuiRoot : public ImGuiNode_Base {
public:
	//==================================================
	// ▼コンストラクタ・デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="label">自身の名前</param>
	ImGuiRoot(std::string label);

	~ImGuiRoot();

	//==================================================
	// ▼各種関数

	void Update() override;

	/// <summary>
	/// ImGuiNodeを追加する
	/// </summary>
	/// <param name="node">ImGuiNodeの実体</param>
	/// <returns>追加したInGuiNodeの実体</returns>
	ImGuiNode_Base* Add(ImGuiNode_Base* node);

	/// <summary>
	/// ImGuiRootを追加する
	/// </summary>
	/// <param name="root">ImGuiRootの実体</param>
	/// <returns>追加したInGuiRootの実体</returns>
	ImGuiRoot* AddChild(ImGuiRoot* root);

	/// <summary>
	/// 引数の名前と一致しているNode、Childを削除する
	/// </summary>
	/// <param name="nodeLabel">削除するNode、Childの名前</param>
	void Delete(std::string nodeLabel);

	/// <summary>
	/// 引数と一致しているNode、Childを削除する
	/// </summary>
	/// <param name="nodeLabel">削除するNode、Childのポインター</param>
	void Delete(ImGuiNode_Base* node);
	
	/// NodeにChild(まとめる)機能を追加する
	/// EndChildが呼ばれるまでに、処理されたNodeをまとめる
	/// そのため、Childを生成した場合は必ずEndChildも生成しなければいけない
	/// パラメーター:
	///  windowSizeX まとめた際のWindowの大きさ(X軸)
	///  windowSizeY まとめた際のWindowの大きさ(Y軸)
	void NodeBeginChild(float windowSizeX, float windowSizeY);

	/// <summary>
	/// Child(まとめる)機能を停止する機能を追加する
	/// </summary>
	void NodeEndChild();

	/// <summary>
	/// 所持しているNode(Childも含める)全てにChild(まとめる)機能を追加する
	/// </summary>
	/// <param name="windowSizeX">まとめた際のWindowの大きさ(X軸)</param>
	/// <param name="windowSizeY">まとめた際のWindowの大きさ(Y軸)</param>
	void SetChild(float windowSizeX, float windowSizeY);

	/// <summary>
	/// 引数の名前と一致しているNodeを取得する
	/// </summary>
	/// <param name="nodeLabel">名前</param>
	/// <returns>引数の名前と一致しているNode。一致した物がない場合nullptr</returns>
	template<class C>
	C* Node(std::string nodeLabel) {
		for (auto itr : node) {
			if (itr->Label() == nodeLabel)
				return dynamic_cast<C*>(itr);
		}
		return nullptr;
	}

	/// <summary>
	/// 引数の名前と一致しているRootを取得する
	/// </summary>
	/// <param name="nodeLabel">名前</param>
	/// <returns>引数の名前と一致しているRoot。一致した物がない場合nullptr</returns>
	ImGuiRoot* Children(std::string nodeLabel);

	/// <summary>
	/// 引数の名前と一致しているRootを親子階層を検索して取得する
	/// </summary>
	/// <param name="nodeLabel">名前</param>
	/// <returns>引数の名前と一致しているRoot。一致した物がない場合nullpt</returns>
	ImGuiRoot* SearchChildren(std::string nodeLabel);

	//==================================================
	// ▼ゲッター

	// 開いている状態かを取得する
	inline bool IsOpen() const { return isOpen; }

private:
	std::vector<ImGuiNode_Base*> node;	// 繋がっているノード
	std::vector<ImGuiRoot*> children;	// 親子階層

	ImGuiNode_BeginChildren* beginChildren;	// まとめる機能
	ImGuiNode_EndChildren* endChildren;		// まとめる機能を停止する機能

	bool isOpen;	// 開いている状態か
};

#endif // IMGUI