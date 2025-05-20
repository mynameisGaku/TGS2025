#pragma once
#include "myDxLib.h"
#include <string>
#include "sceneBase.h"

// ◇コンポーネント
#include "../Source/Component/Component.h"

// ◇当たり判定
#include "../Source/Component/CollisionData.h"

class GameObject {
public:
	GameObject() : m_pScene(nullptr), m_pParent(nullptr), m_destroy(false), m_tag(""), m_active(true) {}
	GameObject(SceneBase* scene) : m_pScene(scene), m_pParent(nullptr), m_destroy(false), m_tag(""), m_active(true) {}
	GameObject(GameObject* object) : m_pScene(nullptr), m_pParent(object), m_destroy(false), m_tag(""), m_active(true) {}
	GameObject(SceneBase* scene, GameObject* object) : m_pScene(scene), m_pParent(object), m_destroy(false), m_tag(""), m_active(true) {}

	virtual ~GameObject() {
		for (auto c = components.begin(); c != components.end();) {
			delete* c;
			c = components.erase(c);
			if (c == components.end()) break;
		}
	}
	
	virtual void Start() {}

	virtual void Update() {
		if (m_active == false)
			return;

		auto& buff = components;
		for (auto com : buff)
			com->Update();
	}

	virtual void Draw() {
		if (m_active == false)
			return;

		auto& buff = components;
		for (auto com : buff)
			com->Draw();
	}

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="colData">当たり判定情報</param>
	virtual void CollisionEvent(const CollisionData& colData) {}

	/// <summary>
	/// 活動するかを設定する
	/// </summary>
	/// <param name="value">trueならUpdateやDrawを呼び出す</param>
	void SetIsActive(bool value) { m_active = value; }

	/// <summary>
	/// 活動しているかを取得する
	/// </summary>
	bool IsActive() const { return m_active; }

	/// <summary>
	/// 自分のインスタンスを削除する時に呼び出す
	/// 次回のUpdateの前に削除される
	/// </summary>
	void DestroyMe() { m_destroy = true; }

	/// <summary>
	/// インスタンスが削除要求されているかを調べる
	/// 主に、SceneManagerでチェックしている
	/// </summary>
	/// <returns></returns>
	bool IsDestroy() const { return m_destroy; }

	/// <summary>
	/// タグをつける
	/// タグは１つだけ付けることができます
	/// </summary>
	/// <param name="_tag">タグ</param>
	void SetTag(std::string _tag) { m_tag = _tag; }

	/// <summary>
	/// 指定されたタグと同じかを返す
	/// </summary>
	/// <param name="_tag">タグ</param>
	/// <returns>同じであればtrue</returns>
	bool IsTag(std::string _tag) const { return m_tag == _tag; }

	/// <summary>
	/// タグを取得する
	/// </summary>
	/// <returns>設定されているタグ</returns>
	const std::string& GetTag() const { return m_tag; }

	/// <summary>
	/// 所属するシーンを返す
	/// </summary>
	/// <returns>所属するシーンのポインター</returns>
	SceneBase* GetScene() const { return m_pScene; }

	/// <summary>
	/// シーンのポインターを保存する
	/// </summary>
	/// <returns>所属するシーンのポインター</returns>
	void SetScene(SceneBase* _scene) { m_pScene = _scene; }

	/// <summary>
	/// 親のオブジェクトを返す
	/// </summary>
	/// <returns>親オブジェクトのポインター</returns>
	GameObject* GetParent() const { return m_pParent; }

	/// <summary>
	/// 親のオブジェクトをセット
	/// </summary>
	void SetParent(GameObject* _parent) { m_pParent = _parent; }

	//==========================================================================================
	// ▼コンポーネント

	/// <summary>
	/// オブジェクトが持っているコンポーネントを取得
	/// </summary>
	template<class T>
	T* GetComponent() {
		for (auto com : components) {
			T* buff = dynamic_cast<T*>(com);
			if (buff != nullptr)
				return buff;
		}
		return nullptr;
	}


	/// <summary>
	/// オブジェクトにコンポーネントを追加
	/// </summary>
	template<class T>
	T* AddComponent() {
		T* buff = new T();
		buff->parent = this;
		components.push_back(buff);
		return buff;
	}

	/// <summary>
	/// オブジェクトが持つコンポーネントを削除
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>
	template<class T>
	bool RemoveComponent() {
		for (auto com = components.begin(); com != components.end();) {
			T* buff = dynamic_cast<T*>(*com);
			if (buff != nullptr) {
				delete* com;
				com = components.erase(com);
				return true;
			}
			else
				com++;
		}
		return false;
	}

private:
	SceneBase* m_pScene;	// 所属するシーン
	GameObject* m_pParent;	// 親のオブジェクト
	bool m_active;			// 機能しているか
	bool m_destroy;			// 自分を削除するか否か
	std::string m_tag;		// タグ
	std::list<Component*> components;	// コンポーネント
};
