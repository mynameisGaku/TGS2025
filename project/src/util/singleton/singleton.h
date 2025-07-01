#pragma once
#include <memory>
#include <list>

// �N���X�uAAA�v���V���O���g���ɂ��� -> Singleton<AAA>::Instance();
//    (����C���X�^���X���ASingletonDeleter�Ɏ����œo�^�����)
// 
// �S�V���O���g����������� -> SingletonDeleter::Delete();

class SingletonDeleter
{
public:
	// �ǉ����ꂽ�폜�֐���S���Ă�
	static void Delete();
	using DestroyFunc = void(*)();
private:
	/// <summary>
	/// �폜�Ώۂ�ǉ�
	/// </summary>
	/// <param name="func">�폜���邽�߂ɌĂԊ֐�</param>
	static void addDeleteTarget(DestroyFunc func);

	// addDeleteTarget��Singleton���炵���ĂԕK�v�Ȃ��̂Ō�����J
	template <class C>
	friend class Singleton;
};

template <class C>
class Singleton
{
public:
	static C& Instance()
	{
		if (not instance)
		{
			create();
		}
		return *instance;
	}

private:
	static void create()
	{
		instance = std::make_unique<C>();
		SingletonDeleter::addDeleteTarget(&Singleton<C>::destroy);
	}

	static void destroy()
	{
		instance.reset();
	}

	static std::unique_ptr<C> instance;
};

template <class C>
std::unique_ptr<C> Singleton<C>::instance = nullptr;
