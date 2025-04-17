#pragma once

//------------------------------------------------------------
// ���C���N���[�h
//------------------------------------------------------------
#include <vector>
#include <functional>

/// <summary>
/// �I�u�W�F�N�g�v�[�����O�p�N���X
/// </summary>
/// <author>�t�W�K�N</author>
template<typename T>
class Pool
{
public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="capacity">�ǂ̂��炢�̗e�ʂ��m�ۂ��邩</param>
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
    // �p�u���b�N�֐�
    //------------------------------------------------------------

    /// <summary>
    /// ��A�N�e�B�u�I�u�W�F�N�g���m�ۂ���
    /// </summary>
    /// <returns>�m�ۂ����|�C���^</returns>
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
    /// �I�u�W�F�N�g���A�N�e�B�u��Ԃɂ���
    /// </summary>
    /// <param name="object">�Ώۂ̃|�C���^</param>
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
    /// �w����ȏ�̃A�N�e�B�u�ȃI�u�W�F�N�g�����݂��邩�m�F����
    /// </summary>
    /// <param name="count">�m�F��������</param>
    /// <returns>�����true</returns>
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
    /// ���ׂĂ̗v�f�����Z�b�g����B
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
    // �v���C�x�[�g�N���X
    //------------------------------------------------------------

    /// <summary>
    /// �I�u�W�F�N�g�Ǘ��p�N���X
    /// </summary>
    class Item
    {
    public:
        Item() : m_Active(false) {}
        ~Item() { delete m_Object; }
        bool    m_Active; // �A�N�e�B�u��Ԃ��ǂ���
        T*      m_Object; // �I�u�W�F�N�g�̃|�C���^
    };

    //------------------------------------------------------------
    // �v���C�x�[�g�֐�
    //------------------------------------------------------------
    
    /// <summary>
    /// �g���ĂȂ��C���f�b�N�X��T���ĕԂ�
    /// </summary>
    /// <returns>
    /// <para>���������ꍇ : ���������C���f�b�N�X</para>
    /// <para>�����ł��Ȃ������ꍇ : -1</para>
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
    // �����o�ϐ�
    //------------------------------------------------------------
    std::vector<Item*>  m_Items;        // �I�u�W�F�N�g�̃��X�g
    uint32_t            m_Capacity;     // ���̃v�[���̗e��
};
