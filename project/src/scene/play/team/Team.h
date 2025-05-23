#pragma once
#include <vector>
#include <utility>

class Team
{
public:

    /// <summary>
    /// �`�[����ID��o�^����
    /// </summary>
    void AddCharaID(int id);

    /// <summary>
    /// �`�[�������o�[�S�̂̍��Z�|�C���g���擾
    /// </summary>
    int GetTotalPoint();

    /// <summary>
    /// �|�C���g�ő��҂�ID�ƃ|�C���g���Z�b�g�Ŏ擾
    /// </summary>
    std::pair<int, int> GetHighestPointHolderIDandPoint();

    /// <summary>
    /// �`�[���������L���O�z����擾
    /// </summary>
    std::vector<std::pair<int, int>> GetTeamRanking();

private:

    /// <summary>
    /// ���łɓo�^���ꂽID���m�F����
    /// </summary>
    bool queryIsRegistered(int id);

    /// <summary>
    /// 
    /// </summary>
    bool isCharaIDInPool(int id);

    class CharaBase* getCharaFromID(int id);

    std::vector<int> m_CharaIDs;

    class CharaManager* m_pCharaManager;
};