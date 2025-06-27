#pragma once
#include <vector>
#include <string>
#include <utility>
#include <list>

class Team
{
public:

    /// <summary>
    /// �`�[������ݒ肵�ď�����
    /// </summary>
    Team(const std::string& name);

    /// <summary>
    /// �`�[����ID��o�^����
    /// </summary>
    void AddCharaID(int id);

    /// <summary>
    /// �`�[�������o�[�S�̂̍��Z�|�C���g���擾
    /// </summary>
    int GetTotalPoint();

    /// <summary>
    /// �������Ă���L�����B��ID���擾
    /// </summary>
    std::vector<int> GetCharaIDs();

    /// <summary>
    /// �|�C���g�ő��҂�ID�ƃ|�C���g���Z�b�g�Ŏ擾
    /// </summary>
    std::pair<int, int> GetHighestPointHolderIDandPoint();

    /// <summary>
    /// �`�[���������L���O�z����擾
    /// </summary>
    std::vector<std::pair<int, int>> GetTeamRanking();

    /// <summary>
    /// �`�[�������擾
    /// </summary>
    std::string GetTeamName() const;

    /// <summary>
    /// 
    /// </summary>
    bool IsCharaIDInPool(int id);

private:
    friend class TeamManager;

    /// <summary>
    /// ���łɓo�^���ꂽID���m�F����
    /// </summary>
    bool queryIsRegistered(int id);

    class CharaBase* getCharaFromID(int id);

    std::vector<int>                m_CharaIDs;
    std::string                     m_TeamName;
    class CharaManager*             m_pCharaManager;
    std::list<std::pair<int, int>>  m_PointLog;
    std::list<int>                  m_Points;
};