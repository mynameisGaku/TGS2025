#pragma once
#include <unordered_map>

class GameObject;
class StateBase;

/// <summary>
/// ステートを管理して、呼び出すクラスを変更する
/// </summary>
class StateManager {
public:
	StateManager(GameObject* parent);
	~StateManager();

	void Update();
	void Draw();
	
	/// <summary>
	/// ステートを追加する
	/// </summary>
	/// <param name="id">ステートの判別番号</param>
	/// <param name="inst">ステートのインスタンス</param>
	void AddInstance(int id, StateBase* inst);

	/// <summary>
	/// ステートを変更する
	/// </summary>
	/// <param name="newState">ステートの判別番号</param>
	void ChangeState(int newState);
	
	//==================================================
	// ▼ゲッター

	/// <summary>
	/// 保有者のポインターを取得する
	/// </summary>
	/// <returns>GameObjectのポインター</returns>
	inline GameObject* Object() const { return parent; }

	/// <summary>
	/// ステートのポインターを取得する
	/// </summary>
	/// <param name="id">ステートの判別番号</param>
	/// <returns>ステートのポインター。idが一致していない場合、nullptr</returns>
	inline StateBase* State(int id) { return stateInstances[id]; }

	/// <summary>
	/// 現在のステートのポインターを取得する
	/// </summary>
	/// <returns>現在使用しているステートのポインター</returns>
	inline StateBase* CurrentState() { return stateInstances[currentStateID]; }

	/// <summary>
	/// 現在のステートの番号を取得する
	/// </summary>
	/// <returns>現在使用しているステートの番号</returns>
	inline int CurrentStateID() const { return currentStateID; }

	/// <summary>
	/// 一つ前のステートの番号を取得する
	/// </summary>
	/// <returns>一つ前のステートの番号</returns>
	inline int PrevStateID() const { return prevStateID; }

	/// <summary>
	/// 変更が予約されているステートの番号を取得する
	/// </summary>
	/// <returns>変更予定のステートの番号</returns>
	inline int NextStateID() const { return nextStateID; }

	inline const std::unordered_map<int, StateBase*> AllStateInstances() const { return stateInstances; }

private:
	GameObject* parent;
	int currentStateID;
	int prevStateID;
	int nextStateID;

	std::unordered_map<int, StateBase*> stateInstances;
};