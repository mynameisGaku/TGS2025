#pragma once

#include <framework/gameObject.h>
#include <vendor/nlohmann/json.hpp>
#include <vector>
#include <mutex>
#include <string>
#include <src/common/network/user/User.h>

// �p�P�b�g�̎�ނ�\�����ʎq
enum PacketType
{
	PACKET_MESSAGE = 1,	// �e�L�X�g���b�Z�[�W
	PACKET_JSON = 2,	// JSON�`���̃f�[�^
};

#pragma pack(push, 1)
// �p�P�b�g�̃w�b�_�[�\���́i�S�f�[�^�̐擪�ɂ��Ď��ʁj
struct PacketHeader
{
	int type; // �f�[�^�̎�ށi��� enum�j
	int size; // ��ɑ����f�[�^�̃o�C�g��
};
#pragma pack(pop)

// �N���C�A���g���i�T�[�o�[���Ŏg�p�j
struct ClientInfo
{
	SOCKET sock;        // �ʐM�p�\�P�b�g
	std::string name;   // �N���C�A���g�̕\�����i������ "Unknown"�j
};

// �T�[�o�[����S�N���C�A���g�փf�[�^�𑗐M����֐�
// exclude �ɂ͑��M���̃\�P�b�g���w�肷�邱�ƂŁA�����ɂ͑���Ȃ��悤�ɂł���
void Broadcast(PacketHeader header, const void* data, SOCKET exclude = INVALID_SOCKET);

// �N���C�A���g�Ƃ̒ʐM�����i�T�[�o�[���j���s���X���b�h�֐�
unsigned __stdcall ClientThread(void* param);

// �N���C�A���g���T�[�o�[����̃f�[�^����M����X���b�h�֐�
unsigned __stdcall RecvThread(void* param);

// �z�X�g���ŃN���C�A���g�̐ڑ���҂��󂯂�X���b�h�֐�
unsigned __stdcall HostAcceptThread(void* param);

class NetworkManager : public GameObject
{
public:
	NetworkManager();
	~NetworkManager();

	void Update() override;
	void Draw() override;

	static void HostCommandProcess(nlohmann::json& json, SOCKET sock);
	static void ClientCommandProcess(nlohmann::json& json, SOCKET sock);

	/// <summary>
	/// Json�𑗐M
	/// </summary>
	/// <param name="json">�_���v��̕�����</param>
	void SendJson(const std::string& json);

	/// <summary>
	/// ���[�U�[����ǉ�����
	/// </summary>
	/// <param name="name">�ǉ����郆�[�U�[�̖��O</param>
	void SendAddUser(const std::string& name);

	void SendTransform(const class Transform& trs, UINT uuid);

	void SendTransitToPlay();


	static SOCKET					g_ListenSock;		// �T�[�o�[���ڑ���҂��󂯂邽�߂̃\�P�b�g
	static std::vector<ClientInfo*> g_Clients;			// �T�[�o�[�ɐڑ����̃N���C�A���g�ꗗ
	static std::mutex				g_Mutex;			// �~���[�e�b�N�X
	static SOCKET					g_Sock;				// �N���C�A���g���ڑ��Ɏg�p����\�P�b�g
	static bool						g_Running;			// ���s����
	static UINT						g_UUIDGenerator;	// UUID�̃W�F�l���[�^�[ 1����n�܂�
	static std::vector<User>		g_Users;			// �z�X�g�ŊǗ�����郆�[�U�[���
	static UINT						g_MyUUID;			// ������UUID �N���C�A���g�����g��UUID��m�邽�߂ɂ����

	void subscribe(const std::string& name);
};
