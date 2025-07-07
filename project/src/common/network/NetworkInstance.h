#pragma once
#include <string>
#include <src/reference/network/NetworkRef.h>
#include <DxLib.h>

/// <summary>
/// �ڑ��P�ɂ��A�P�C���X�^���X
/// </summary>
class NetworkInstance
{
private:
	friend class NetworkManager;

	// �󂯎��郁�b�Z�[�W�̍ő�T�C�Y
	static const int MAX_MESSAGE_SIZE = 1024;
public:

	void Update();

	// �l�b�g�n���h�����擾
	int NetHandle();

	// �n���h�����Z�b�g
	void SetHandle(int hNet);

	// �o�b�t�@�Ƀ��b�Z�[�W���i�[����
	void SetMessageToBuffer(const char* message)
	{
		strncpy_s(m_MessageBuf, message, MAX_MESSAGE_SIZE - 1);
		m_MessageBuf[MAX_MESSAGE_SIZE - 1] = '\0'; // Ensure null termination
		m_NeedCheck = true;
	}

	// �擪�A�h���X���Ԃ�܂�
	char* GetMessageBuffer()
	{
		return &m_MessageBuf[0];
	}

	// �`�F�b�N�K�v��Ԃɂ���
	const void NeedCheck() const { m_NeedCheck = true; }

	// ������ԂɃN���A
	void Reset();

	void SetIP(const IPDATA& ip);

    bool CompareCurrentIP(const IPDATA& other) const;
    bool ComparePrevIP(const IPDATA& other) const;

private:
	int m_hNet{};								// �n���h��
	char m_MessageBuf[MAX_MESSAGE_SIZE]{};		// ���b�Z�[�W�̃o�b�t�@
	mutable bool m_NeedCheck{};					// �`�F�b�N�K�v��Ԃ��ǂ���
	NetworkRef::IP m_CurrentIP{};				// ���݂�IP�A�h���X
	NetworkRef::IP m_PrevIP{};					// �O��ڑ����Ă���IP�A�h���X
};