#pragma once

//------------------------------------------------------------
// ���C���N���[�h
//------------------------------------------------------------
#include <vector>
#include <functional>
#include <stdexcept>
#include <vendor/ImGui/imgui.h>

/// <summary>
/// �I�u�W�F�N�g�v�[�����O�p�N���X
/// </summary>
/// <author>�t�W�K�N</author>
template<typename T>
class Pool
{
private:
    //------------------------------------------------------------
    // �v���C�x�[�g�N���X
    //------------------------------------------------------------

    /// <summary>
    /// �I�u�W�F�N�g�Ǘ��p�N���X
    /// </summary>
    class Item
    {
    public:
        Item() : m_IsActive(false), m_pObject(nullptr), m_Index(-1) {}
        ~Item() { /* DO NOTHING */ }
        bool        m_IsActive; // �A�N�e�B�u��Ԃ��ǂ���
        uint32_t    m_Index;  // ���g�̃C���f�b�N�X
        T*          m_pObject; // �I�u�W�F�N�g�̃|�C���^
    };

public:
    
    using POOL_INIT_FUNC = std::function<T*(uint32_t, T*)>;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="capacity">�ǂ̂��炢�̗e�ʂ��m�ۂ��邩</param>
    Pool(uint32_t capacity, POOL_INIT_FUNC func = nullptr)
    {
        reserve(capacity, func);
    }
    
    ~Pool()
    {
        m_Items.clear();
    }

    //------------------------------------------------------------
    // �p�u���b�N�֐�
    //------------------------------------------------------------

    /// <summary>
    /// ��A�N�e�B�u�I�u�W�F�N�g���m�ۂ���
    /// </summary>
    /// <returns>�m�ۂ����|�C���^</returns>
    T* GetDeactiveObject()
    {
        int index = GetIndex();
        if (index == -1)
        {
            throw std::runtime_error("�����ȃC���f�b�N�X���Q�Ƃ��Ă��܂��B");
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
    /// �I�u�W�F�N�g���A�N�e�B�u��Ԃɂ���
    /// </summary>
    /// <param name="object">�Ώۂ̃|�C���^</param>
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
    /// [�C���f�b�N�X��] �I�u�W�F�N�g���A�N�e�B�u��Ԃɂ��� �������̂��y��
    /// </summary>
    /// <param name="index">�Ώۂ̃C���f�b�N�X</param>
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
    /// �w����ȏ�̃A�N�e�B�u�ȃI�u�W�F�N�g�����݂��邩�m�F����
    /// </summary>
    /// <param name="count">�m�F��������</param>
    /// <returns>�����true</returns>
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
    /// �e�ʂ��擾
    /// </summary>
    /// <returns>�e�ʂ�Ԃ�</returns>
    uint32_t GetCapacity()
    {
        return m_Capacity;
    }

    /// <summary>
    /// ���ׂẴA�C�e�����폜���A�w��L���p�V�e�B���Ċm�ۂ���B
    /// �߂��Ⴍ����d���̂ŁA���܂�g��Ȃ������ǂ��B
    /// </summary>
    /// <param name="capacity">�Ċm�ۂ��������̗e��</param>
    void ResetCapacity(uint32_t capacity, POOL_INIT_FUNC func = nullptr)
    {
        reserve(capacity, func);
    }

    /// <summary>
    /// ���ׂĂ̗v�f�����Z�b�g����B
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
    /// �g���ĂȂ��C���f�b�N�X��T���ĕԂ�
    /// </summary>
    /// <returns>
    /// <para>���������ꍇ : ���������C���f�b�N�X</para>
    /// <para>�����ł��Ȃ������ꍇ : -1</para>
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
    // �v���C�x�[�g�֐�
    //------------------------------------------------------------
    
    /// <summary>
    /// �L���p�V�e�B���X�V�E�e�ʂ̍Ċm�ۂ��s���B
    /// �߂�����d����
    /// </summary>
    /// <param name="capacity">�L���p</param>
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
    // �����o�ϐ�
    //------------------------------------------------------------
    std::vector<Item*>      m_Items;                // �I�u�W�F�N�g�̃��X�g
    uint32_t                m_Capacity;             // ���̃v�[���̗e��
    uint32_t                m_ActiveObjectCount;    // �A�N�e�B�u�ȃI�u�W�F�N�g�̐�

};
