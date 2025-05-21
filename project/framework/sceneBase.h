#pragma once
#include <list>
#include "framework/sceneManager.h"
class GameObject;

template <class C>
C* Instantiate() {
	C* obj = new C();
	SceneManager::CurrentScene()->Push((GameObject*)obj);
	return obj;
};

template <class C>
C* FindGameObject() {
	auto objects = SceneManager::CurrentScene()->GetAllNodes();
	for (auto& m : objects) {
		C* obj = dynamic_cast<C*>(m.object);
		if (obj != nullptr)
			return obj;
	}
	return nullptr;
}

template <class C>
std::list<C*> FindGameObjects() {
	auto objects = SceneManager::CurrentScene()->GetAllNodes();
	std::list<C*> list;
	for (const auto& m : objects) {
		C* obj = dynamic_cast<C*>(m.object);
		if (obj != nullptr)
			list.emplace_back(obj);
	}
	return list;
}

template<class C>
C* FindGameObjectWithTag(std::string _tag) {
	auto objects = SceneManager::CurrentScene()->GetAllNodes();
	for (const auto& m : objects) {
		C* obj = dynamic_cast<C*>(m.object);
		if (obj != nullptr && obj->GetTag() == _tag)
			return obj;
	}
	return nullptr;
}

template<class C>
C* FindGameObjectsWithTag(std::string _tag) {
	auto objects = SceneManager::CurrentScene()->GetAllNodes();
	std::list<C*> list;
	for (const auto& m : objects) {
		C* obj = dynamic_cast<C*>(m.object);
		if (obj != nullptr && obj->GetTag() == _tag)
			list.push_back(obj);
	}
	return list;
}

class SceneBase {
public:
	SceneBase();
	SceneBase(bool setCurrentScene);
	SceneBase(bool setCurrentScene, std::string name);
	virtual ~SceneBase();

	virtual void Update();
	virtual void Draw();

	/// <summary>
	/// GameObject�𐶐�����
	/// �����Ő�������ƁAUpdate,Draw���Ă΂��悤�ɂȂ�
	/// </summary>
	template <class C>
	C* CreateGameObject() {
		C* obj = new C(this);
		Push(obj);
		return obj;
	};

	/// <summary>
	/// GameObject�𐶐�����
	/// �����Ő�������ƁAUpdate,Draw���Ă΂��悤�ɂȂ�
	/// </summary>
	template <class C>
	C* Instantiate() {
		C* obj = new C();
		Push(obj);
		return obj;
	};

	/// <summary>
	/// �N���X����v����GameObject���擾����
	/// </summary>
	/// <returns>GameObject�̃C���X�^���X</returns>
	template<class C>
	C* FindGameObject() const {
		for (const auto& m : m_objects) {
			C* obj = dynamic_cast<C*>(m.object);
			if (obj != nullptr)
				return obj;
		}
		return nullptr;
	}

	/// <summary>
	/// �N���X����v����GameObject��S�Ď擾����
	/// </summary>
	/// <returns>GameObject�̃C���X�^���X</returns>
	template<class C>
	std::list<C*> FindGameObjects() const {
		std::list<C*> list;
		for (const auto& m : m_objects) {
			C* obj = dynamic_cast<C*>(m.object);
			if (obj != nullptr)
				list.emplace_back(obj);
		}
		return list;
	}

	template<class C>
	C* FindGameObjectWithTag(std::string _tag) const {
		for (const auto& m : m_objects) {
			C* obj = dynamic_cast<C*>(m.object);
			if (obj != nullptr && obj->GetTag() == _tag)
				return obj;
		}
		return nullptr;
	}

	template<class C>
	C* FindGameObjectsWithTag(std::string _tag) const {
		std::list<C*> list;
		for (const auto& m : m_objects) {
			C* obj = dynamic_cast<C*>(m.object);
			if (obj != nullptr && obj->GetTag() == _tag)
				list.push_back(obj);
		}
		return list;
	}

	inline void Push(GameObject* obj)
	{
		NODE node;
		node.object = obj;
		m_objects.emplace_back(node);
		m_needSort = true;
	}

	/// <summary>
	/// �`��̃v���C�I���e�B��ݒ肷��
	/// ���l�����Ȃ����ɕ`�悳���̂ŁA�Q�c�ł͉��ɕ\�������
	/// �Q�c�Ŏ�O�ɕ\�����������A�R�c�Ō�ɕ`�悵�������́A�l����������
	/// �v���C�I���e�B���������̂̏��Ԃ͕ۏ؂���Ȃ�
	/// �v���C�I���e�B�̃f�t�H���g��100�ł�
	/// </summary>
	/// <param name="obj">�v���C�I���e�B��ݒ肷��I�u�W�F�N�g</param>
	/// <param name="order">�`��v���C�I���e�B</param>
	void SetDrawOrder(GameObject* _obj, int _order);

	/// <summary>
	/// GameObject���폜����
	/// </summary>
	/// <param name="obj">GameObject�̃C���X�^���X</param>
	void DeleteGameObject(GameObject* obj);

	/// <summary>
	/// �S�Ă�GameObject���폜����
	/// </summary>
	void DeleteAllGameObject();

	/// <summary>
	/// ���ׂĂ�GameObject���擾����
	/// </summary>
	/// <returns>���ׂĂ�GameObject�̃��X�g</returns>
	std::list<GameObject*> GetAllObjects() const;

	struct NODE {
		int order;
		bool initialized;
		GameObject* object;
		NODE() : order(100), object(nullptr), initialized(false) {}
	};

	/// <summary>
	/// ���ׂĂ�GameObjectNode���擾����
	/// </summary>
	/// <returns>���ׂĂ�GameObject�̃��X�g</returns>
	const std::list<NODE>& GetAllNodes() const { return m_objects; }

	/// <summary>
	/// �V�[���̎�ޖ���ݒ肷��
	/// </summary>
	inline void SetName(std::string name) { sceneName = name; }

	/// <summary>
	/// �V�[���̎�ޖ����擾����
	/// </summary>
	inline std::string Name() const { return sceneName; }

	/// <summary>
	/// �V�[���̃X�e�[�g
	/// </summary>
	enum class SceneState {
		BeforePlay,	// �v���C�O
		InPlay,		// �v���C��
		AfterPlay,	// �v���C��
	};

	/// <summary>
	/// �V�[���̃X�e�[�g��ύX����
	/// </summary>
	/// <param name="state">�ύX�������X�e�[�g</param>
	inline void ChangeState(SceneState state) { sceneState = state; }

	/// <summary>
	/// ���݂̃V�[���̃X�e�[�g�����擾����
	/// </summary>
	inline SceneState State() const { return sceneState; }

	/// <summary>
	/// �v���C�O�̍X�V����
	/// </summary>
	virtual void BeforePlayUpdate() {}
	
	/// <summary>
	/// �v���C���̍X�V����
	/// </summary>
	virtual void InPlayUpdate() {}

	/// <summary>
	/// �v���C��̍X�V����
	/// </summary>
	virtual void AfterPlayUpdate() {}

private:
	std::list<NODE> m_objects;     // �����ŊǗ����Ă���GameObject���X�g
	std::list<NODE> m_drawObjects; // �`��p��GameObject���X�g
	bool m_needSort;

	void DeleteFromDrawObjects(GameObject* obj);
	static bool Comp(const NODE& o1, const NODE& o2);

protected:
	std::string sceneName;	// �V�[���̎�ޖ�
	SceneState sceneState;	// �V�[���̃X�e�[�g
};
