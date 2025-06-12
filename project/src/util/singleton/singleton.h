#pragma once
#include <memory>
#include <list>

// クラス「AAA」をシングルトンにする -> Singleton<AAA>::Instance();
//    (初回インスタンス時、SingletonDeleterに自動で登録される)
// 
// 全シングルトンを解放する -> SingletonDeleter::Delete();

class SingletonDeleter
{
public:
	// 追加された削除関数を全部呼ぶ
	static void Delete();
	using DestroyFunc = void(*)();
private:
	/// <summary>
	/// 削除対象を追加
	/// </summary>
	/// <param name="func">削除するために呼ぶ関数</param>
	static void addDeleteTarget(DestroyFunc func);

	// addDeleteTargetはSingletonからしか呼ぶ必要ないので限定公開
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
		instance.release();
	}

	static std::unique_ptr<C> instance;
};

template <class C>
std::unique_ptr<C> Singleton<C>::instance = nullptr;
