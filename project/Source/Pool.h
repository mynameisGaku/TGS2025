#pragma once

//------------------------------------------------------------
// ▼インクルード
//------------------------------------------------------------
#include <vector>
#include <functional>

/// <summary>
/// オブジェクトプーリング用クラス
/// </summary>
/// <author>フジガク</author>
template<typename T>
class Pool
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="capacity">どのくらいの容量を確保するか</param>
    Pool(uint32_t capacity)
    {
        m_Capacity = capacity;
        m_Items.reserve(capacity);
        for (uint32_t i = 0; i < capacity; ++i)
        {
            Item* item = new Item();
            m_Items.push_back(item);
        }
    }
    
    ~Pool()
    {
        for (auto& item : m_Items)
        {
            delete item;
        }
        m_Items.clear();
    }

    //------------------------------------------------------------
    // パブリック関数
    //------------------------------------------------------------

    /// <summary>
    /// 非アクティブオブジェクトを確保する
    /// </summary>
    /// <returns>確保したポインタ</returns>
    T* GetDeActiveObject()
    {
        int index = GetIndex();
        if (index == -1)
        {
            return nullptr;
        }

        return m_Items[index]->m_Object;
    }

    /// <summary>
    /// オブジェクトを非アクティブ状態にする
    /// </summary>
    /// <param name="object">対象のポインタ</param>
    void DeActive(T* object)
    {
        for (auto& obj : m_Items)
        {
            if (obj->m_Object == object)
            {
                obj->m_Active = false;
                obj->m_Object = nullptr;
                break;
            }
        }
    }

    /// <summary>
    /// 指定個数以上のアクティブなオブジェクトが存在するか確認する
    /// </summary>
    /// <param name="count">確認したい個数</param>
    /// <returns>あればtrue</returns>
    bool CheckActiveObjectByCount(uint32_t count)
    {
        int activeCount = 0;
        for (auto& item : m_Items)
        {
            if (item->m_Active)
            {
                ++activeCount;
            }
        }

        if (activeCount >= count)
        {
            return true;
        }

        return false;
    }

    /// <summary>
    /// 容量を取得
    /// </summary>
    /// <returns>容量を返す</returns>
    uint32_t GetCapacity()
    {
        return m_Capacity;
    }

    /// <summary>
    /// すべてのアイテムを削除し、指定キャパシティ分再確保する。
    /// めちゃくちゃ重いので、あまり使わない方が良い。
    /// </summary>
    /// <param name="capacity">再確保したい分の容量</param>
    void ResetCapacity(uint32_t capacity)
    {
        for (auto& item : m_Items)
        {
            delete item;
        }
        m_Items.clear();
        m_Capacity = capacity;
        m_Items.reserve(capacity);
        for (uint32_t i = 0; i < capacity; ++i)
        {
            Item* item = new Item();
            m_Items.push_back(item);
        }
    }

    /// <summary>
    /// すべての要素をリセットする。
    /// </summary>
    void ReleaseAll()
    {
        for (auto& item : m_Items)
        {
            if (item->m_Object != nullptr)
            {
                delete item->m_Object;
                item->m_Object = nullptr;
                item->m_Active = false;
            }
        }
    }

private:

    //------------------------------------------------------------
    // プライベートクラス
    //------------------------------------------------------------

    /// <summary>
    /// オブジェクト管理用クラス
    /// </summary>
    class Item
    {
    public:
        Item() : m_Active(false) {}
        ~Item() { delete m_Object; }
        bool    m_Active; // アクティブ状態かどうか
        T*      m_Object; // オブジェクトのポインタ
    };

    //------------------------------------------------------------
    // プライベート関数
    //------------------------------------------------------------
    
    /// <summary>
    /// 使われてないインデックスを探して返す
    /// </summary>
    /// <returns>
    /// <para>発見した場合 : 見つかったインデックス</para>
    /// <para>発見できなかった場合 : -1</para>
    /// </returns>
    uint32_t GetIndex()
    {
        if (m_Items.size() >= m_Capacity)
        {
            return -1;
        }

        for (uint32_t i = 0; i < m_Items.size(); ++i)
        {
            if (not m_Items[i]->m_Active)
            {
                return i;
            }
        }

        return -1;
    }

    //------------------------------------------------------------
    // メンバ変数
    //------------------------------------------------------------
    std::vector<Item*>  m_Items;        // オブジェクトのリスト
    uint32_t            m_Capacity;     // このプールの容量
};
