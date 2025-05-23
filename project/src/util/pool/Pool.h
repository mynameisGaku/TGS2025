#pragma once

//------------------------------------------------------------
// ▼インクルード
//------------------------------------------------------------
#include <vector>
#include <functional>
#include <stdexcept>
#include <vendor/ImGui/imgui.h>

/// <summary>
/// オブジェクトプーリング用クラス
/// </summary>
/// <author>フジガク</author>
template<typename T>
class Pool
{
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
        Item() : m_IsActive(false), m_pObject(nullptr), m_Index(-1) {}
        ~Item() { /* DO NOTHING */ }
        bool        m_IsActive; // アクティブ状態かどうか
        uint32_t    m_Index;  // 自身のインデックス
        T*          m_pObject; // オブジェクトのポインタ
    };

public:
    
    using POOL_INIT_FUNC = std::function<T*(uint32_t, T*)>;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="capacity">どのくらいの容量を確保するか</param>
    Pool(uint32_t capacity, POOL_INIT_FUNC func = nullptr)
    {
        reserve(capacity, func);
    }
    
    ~Pool()
    {
        m_Items.clear();
    }

    //------------------------------------------------------------
    // パブリック関数
    //------------------------------------------------------------

    /// <summary>
    /// 非アクティブオブジェクトを確保する
    /// </summary>
    /// <returns>確保したポインタ</returns>
    T* GetDeactiveObject()
    {
        int index = GetIndex();
        if (index == -1)
        {
            throw std::runtime_error("無効なインデックスを参照しています。");
        }

        auto ret = m_Items[index]->m_pObject;
        return ret;
    }

    T* Alloc(POOL_INIT_FUNC func = nullptr)
    {
        T* obj = GetDeactiveObject();
        uint32_t index = GetIndex();

        if (obj == nullptr)
        {
            if (func)
            {
                obj = func(index, obj);
            }
            else
            {
                obj = new T;
            }
        }
        m_Items[index]->m_IsActive = true;
        m_ActiveObjectCount++;

        return obj;
    }

    /// <summary>
    /// オブジェクトを非アクティブ状態にする
    /// </summary>
    /// <param name="object">対象のポインタ</param>
    void DeActive(T* object)
    {
        for (auto& obj : m_Items)
        {
            if (obj->m_pObject == object)
            {
                obj->m_IsActive = false;
                obj->m_pObject = nullptr;
                m_ActiveObjectCount--;
                break;
            }
        }
    }

    /// <summary>
    /// [インデックス版] オブジェクトを非アクティブ状態にする こっちのが軽い
    /// </summary>
    /// <param name="index">対象のインデックス</param>
    void DeActive(uint32_t& index)
    {
        if (index >= m_Capacity)
            return;

        Item* item = m_Items[index];

        if (item == nullptr)
            return;

        if(item->m_IsActive == true)
        {
            m_ActiveObjectCount--;
            item->m_IsActive = false;
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
            if (item->m_IsActive)
            {
                ++activeCount;
            }
        }

        if ((uint32_t)activeCount >= count)
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
    void ResetCapacity(uint32_t capacity, POOL_INIT_FUNC func = nullptr)
    {
        reserve(capacity, func);
    }

    /// <summary>
    /// すべての要素をリセットする。
    /// </summary>
    void ReleaseAll()
    {
        for (auto& item : m_Items)
        {
            if (item->m_pObject != nullptr)
            {
                delete item->m_pObject;
                item->m_pObject = nullptr;
                item->m_IsActive = false;
            }
        }
        m_ActiveObjectCount = 0;
    }

    std::vector<Item*> GetAllItems() const
    {
        return m_Items;
    }

    Item* GetItem(uint32_t index) const
    {
        return m_Items[index];
    }

    void SetObjectPointer(uint32_t index, T* pObj)
    {
        m_Items[index]->m_pObject = pObj;
    }

    void PoolImGuiRendererBegin(const std::string& name = "PoolDebugger")
    {
        ImGui::Begin(name.c_str());
    }

    void PoolImguiRendererEnd()
    {
        ImGui::End();
    }

    /// <summary>
    /// 使われてないインデックスを探して返す
    /// </summary>
    /// <returns>
    /// <para>発見した場合 : 見つかったインデックス</para>
    /// <para>発見できなかった場合 : -1</para>
    /// </returns>
    uint32_t GetIndex()
    {
        if (m_Items.size() - 1 > m_Capacity)
        {
            return -1;
        }

        for (uint32_t i = 0; i < m_Items.size(); ++i)
        {
            if (not m_Items[i]->m_IsActive)
            {
                return i;
            }
        }

        return -1;
    }

    int GetActiveItemNum()
    {
        return m_ActiveObjectCount;
    }

    bool QueryWasRegister(int index)
    {
        if (index > m_Items.size() - 1)
            return false;

        if (m_Items[index]->m_IsActive)
            return true;

        return false;
    }

    T* Get(int index)
    {
        return m_Items[index]->m_pObject;
    }

private:

    //------------------------------------------------------------
    // プライベート関数
    //------------------------------------------------------------
    
    /// <summary>
    /// キャパシティを更新・容量の再確保を行う。
    /// めっちゃ重たい
    /// </summary>
    /// <param name="capacity">キャパ</param>
    void reserve(uint32_t& capacity, POOL_INIT_FUNC func = nullptr)
    {
        for (auto& item : m_Items)
        {
            delete item;
        }
        m_Items.clear();
        m_Capacity = capacity - 1;
        m_ActiveObjectCount = 0;
        m_Items.reserve(capacity);
        for (uint32_t i = 0; i < capacity; ++i)
        {
            Item* item = new Item();
            item->m_Index = i;
            if (func)
            {
                func((uint32_t)i, item->m_pObject);
            }
            m_Items.push_back(item);
        }
    }

    //------------------------------------------------------------
    // メンバ変数
    //------------------------------------------------------------
    std::vector<Item*>      m_Items;                // オブジェクトのリスト
    uint32_t                m_Capacity;             // このプールの容量
    uint32_t                m_ActiveObjectCount;    // アクティブなオブジェクトの数

};
