#pragma once
#include "framework/GameObject.h"
#include <unordered_map>

class BallManager;
class CharaManager;

/// <summary>
/// ���b�N�I������UI�A�탍�b�N�I������UI��`�悷��
/// </summary>
/// <author>�~�b�`</author>
class TargetManager : public GameObject {
public:
	TargetManager();
	~TargetManager();

	void Start() override;
	void Update() override;
	void Draw() override;

	int TargetID(int charaIndex);

private:
	/// <summary>
	/// �_���Ă���{�[���̈ʒu�Ƀ}�[�J�[��`�悵�܂�
	/// </summary>
	/// <param name="ballPos">�{�[���̍��W</param>
	/// <param name="targetCharaID">�}�[�J�[��\������ΏۃL�����N�^�[��ID</param>
	void DrawBallPosMarker(const Vector3& ballPos, int targetCharaID);

	/// <summary>
	/// �O���̐Ԃ��x���\��
	/// </summary>
	void DrawWarning();

	/// <summary>
	/// �_���Ă���{�[���̈ʒu�Ƀh�Q��`�悵�܂�
	/// </summary>
	/// <param name="ballPos">�{�[���̍��W</param>
	/// <param name="targetCharaID">�}�[�J�[��\������ΏۃL�����N�^�[��ID</param>
	void DrawThorn(const Vector3& ballPos, int targetCharaID);

	BallManager* ballManager;	// �{�[���̊Ǘ���
	CharaManager* charaManager;	// �L�����̊Ǘ���

	std::unordered_map<int, int> targetList;	// Key: myIndex, Value: targetIndex
	std::vector<bool> checkCamera;	// �J�����̐��Ԃ�̃`�F�b�N�σ}�[�N
	int hArrow; // �}�[�J�[�p�摜
};