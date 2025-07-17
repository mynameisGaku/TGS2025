#pragma once

class GameObject;

/// <summary>
/// コンポーネント
/// </summary>
class Component {
public:
	GameObject* parent;	// 保有者

	Component() : parent(nullptr), isActive(true), destroyFlag(false) {};

	virtual ~Component() {};
	virtual void Update() {};
	virtual void Draw() {};

	/// <summary>
	/// 削除を行うフラグをオンにする
	/// </summary>
	inline void DestroyMe() { destroyFlag = true; }

	//==========================================================================================
	// ▼セッター

	/// <summary>
	/// 活動しているかを設定する
	/// </summary>
	inline void SetIsActive(bool value) { isActive = value; }

	//==========================================================================================
	// ▼ゲッター

	/// <summary>
	/// 削除するかを取得する
	/// </summary>
	inline const bool IsDestroyFlag() const { return destroyFlag; }

	/// <summary>
	/// 活動しているかを取得する
	/// </summary>
	inline const bool IsActive() const { return isActive; }

	/// <summary>
	/// 保有者のポインターを取得する
	/// </summary>
	/// <typeparam name="C">保有者のクラス</typeparam>
	/// <returns>保有者のポインターを返す。クラスが一致しない場合、nullptrを返す</returns>
	template<class C>
	C* Parent() const { return dynamic_cast<C*>(parent); }

protected:
	bool isActive;		// 活動しているか
	bool destroyFlag;	// 削除を行うか
};