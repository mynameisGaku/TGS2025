#pragma once
#include "myDxLib.h"
#include <string>
#include "sceneBase.h"

// ���R���|�[�l���g
#include "../Source/Component/Component.h"

// �������蔻��
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
	/// �����蔻�菈��
	/// </summary>
	/// <param name="colData">�����蔻����</param>
	virtual void CollisionEvent(const CollisionData& colData) {}

	/// <summary>
	/// �������邩��ݒ肷��
	/// </summary>
	/// <param name="value">true�Ȃ�Update��Draw���Ăяo��</param>
	void SetIsActive(bool value) { m_active = value; }

	/// <summary>
	/// �������Ă��邩���擾����
	/// </summary>
	bool IsActive() const { return m_active; }

	/// <summary>
	/// �����̃C���X�^���X���폜���鎞�ɌĂяo��
	/// �����Update�̑O�ɍ폜�����
	/// </summary>
	void DestroyMe() { m_destroy = true; }

	/// <summary>
	/// �C���X�^���X���폜�v������Ă��邩�𒲂ׂ�
	/// ��ɁASceneManager�Ń`�F�b�N���Ă���
	/// </summary>
	/// <returns></returns>
	bool IsDestroy() const { return m_destroy; }

	/// <summary>
	/// �^�O������
	/// �^�O�͂P�����t���邱�Ƃ��ł��܂�
	/// </summary>
	/// <param name="_tag">�^�O</param>
	void SetTag(std::string _tag) { m_tag = _tag; }

	/// <summary>
	/// �w�肳�ꂽ�^�O�Ɠ�������Ԃ�
	/// </summary>
	/// <param name="_tag">�^�O</param>
	/// <returns>�����ł����true</returns>
	bool IsTag(std::string _tag) const { return m_tag == _tag; }

	/// <summary>
	/// �^�O���擾����
	/// </summary>
	/// <returns>�ݒ肳��Ă���^�O</returns>
	const std::string& GetTag() const { return m_tag; }

	/// <summary>
	/// ��������V�[����Ԃ�
	/// </summary>
	/// <returns>��������V�[���̃|�C���^�[</returns>
	SceneBase* GetScene() const { return m_pScene; }

	/// <summary>
	/// �V�[���̃|�C���^�[��ۑ�����
	/// </summary>
	/// <returns>��������V�[���̃|�C���^�[</returns>
	void SetScene(SceneBase* _scene) { m_pScene = _scene; }

	/// <summary>
	/// �e�̃I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <returns>�e�I�u�W�F�N�g�̃|�C���^�[</returns>
	GameObject* GetParent() const { return m_pParent; }

	/// <summary>
	/// �e�̃I�u�W�F�N�g���Z�b�g
	/// </summary>
	void SetParent(GameObject* _parent) { m_pParent = _parent; }

	//==========================================================================================
	// ���R���|�[�l���g

	/// <summary>
	/// �I�u�W�F�N�g�������Ă���R���|�[�l���g���擾
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
	/// �I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�
	/// </summary>
	template<class T>
	T* AddComponent() {
		T* buff = new T();
		buff->parent = this;
		components.push_back(buff);
		return buff;
	}

	/// <summary>
	/// �I�u�W�F�N�g�����R���|�[�l���g���폜
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
	SceneBase* m_pScene;	// ��������V�[��
	GameObject* m_pParent;	// �e�̃I�u�W�F�N�g
	bool m_active;			// �@�\���Ă��邩
	bool m_destroy;			// �������폜���邩�ۂ�
	std::string m_tag;		// �^�O
	std::list<Component*> components;	// �R���|�[�l���g
};
