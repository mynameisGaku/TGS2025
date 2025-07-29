#pragma once
#include "src/util/transform/Transform.h"
#include "src/scene/play/ball/Ball.h"
#include <unordered_map>
#include <string>

class RockOnData
{
public:
	RockOnData() : RockOnData(-1) {}

	RockOnData(int ballIndex) : 
		BallIndex(ballIndex) 
	{}

	int BallIndex;	// �{�[���̃C���f�b�N�X
};

/// <summary>
/// �{�[���𓊂���Ƃ����b�N�I���ł���^�[�Q�b�g
/// </summary>
/// <author>�����h�l</author>
class BallTarget
{
public:
	BallTarget();

	void Reset();

	void Update();
	void Draw();

	void SetPosition(const Vector3& position) { m_Position = position; }
	void SetPositionWithParent(const Vector3& offset, Transform* parent);
	const Vector3& Position() const { return m_Position; }

	void SetActive(bool isActive) { m_IsActive = isActive; }
	bool IsActive() const { return m_IsActive; }

	void SetIndex(int index) { m_Index = index; }
	int Index() const { return m_Index; }

	void SetCanRockOn(bool canRockOn) { m_CanRockOn = canRockOn; }
	bool CanRockOn() const { return m_CanRockOn; }

	void SetDoDeactivateOnNoRockOn(bool doDeactivate) { m_DoDeactivateOnNoRockOn = doDeactivate; }
	bool DoDeactivateOnNoRockOn() const { return m_DoDeactivateOnNoRockOn; }

	void SetRockOnData(const RockOnData& rockOnData) { m_RockOnData[rockOnData.BallIndex] = rockOnData; }
	void EraseRockOnData(int ballIndex) { if (m_RockOnData.contains(ballIndex)) m_RockOnData.erase(ballIndex); }

	void SetCharaTag(std::string charaTag) { m_CharaTag = charaTag; }
	std::string GetCharaTag() const { return m_CharaTag; }
private:
	int m_Index;		// �v�[���̃C���f�b�N�X

	Vector3 m_Position;	// �^�[�Q�b�g�̈ʒu
	std::string m_CharaTag;	// �����`�[���̃^�O

	bool m_IsActive;	// �^�[�Q�b�g���A�N�e�B�u���ǂ���
	bool m_CanRockOn;	// ���b�N�I���\���ǂ���
	bool m_DoDeactivateOnNoRockOn;	// ���b�N�I�����Ȃ��Ƃ��ɔj�����邩�ǂ���

	std::unordered_map<int, RockOnData> m_RockOnData;	// ���g�����b�N�I�����Ă�����̂����̃f�[�^
};
