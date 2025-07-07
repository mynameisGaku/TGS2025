#include "NetworkManager.h"
#include <src/reference/network/NetworkRef.h>
#include <cassert>
#include <src/util/file/json/RawJson.h>

NetworkManager::NetworkManager()
{
	auto& net = NetworkRef::Inst();
	net.Load(true);

	m_NetworkInstances.reserve((size_t)net.ConnectionMax);
	for (USHORT i = 0; i < net.ConnectionMax; i++)
	{
		m_NetworkInstances.push_back(new NetworkInstance());
	}

	if (net.IsHost)
	{
		// �T�[�o�[���̏���
		if (PreparationListenNetWork(net.Port) == -1)
			assert(!"�ڑ��ҋ@��Ԃɂł��܂���ł����B");
	}
	else
	{
		// �N���C�A���g���̏���
		IPDATA ip{
			net.Host_IPAddress.d1,
			net.Host_IPAddress.d2,
			net.Host_IPAddress.d3,
			net.Host_IPAddress.d4};

		if (ConnectNetWork(ip, net.Port) == -1)
			assert(!"�T�[�o�[�ɐڑ��ł��܂���ł����B");
	}
}

NetworkManager::~NetworkManager()
{
	StopListenNetWork();

	for (auto& inst : m_NetworkInstances)
	{
		if (inst != nullptr)
		{
			delete inst;
			inst = nullptr;
		}
	}
	m_NetworkInstances.clear();
}

void NetworkManager::Update()
{
	// �V���Ȑڑ����������m�F����
	int NetHandle = GetNewAcceptNetWork();
	if (NetHandle != -1)
	{
		for (auto& netinst : m_NetworkInstances)
		{
			IPDATA ip{};
			GetNetWorkIP(NetHandle, &ip);

			if (netinst->NetHandle() == -1)
			{
				netinst->SetHandle(NetHandle);
				netinst->SetIP(ip);
				break;
			}
		}
	}

	// �ڑ�����Ă���l�b�g���[�N�̍X�V
	for (auto& netinst : m_NetworkInstances)
	{
		if (netinst->NetHandle() != -1)
		{
			// ��M���Ă��邩�ǂ����m�F
			INT dataLength = GetNetWorkDataLength(netinst->NetHandle());
			if (dataLength == 0)
				continue;

			char buf[NetworkInstance::MAX_MESSAGE_SIZE]{};

			// �f�[�^��M
			NetWorkRecv(netinst->NetHandle(), buf, dataLength);

			// ��M�����f�[�^������
			netinst->SetMessageToBuffer(buf);

			// �u�m�F��낵���v��Ԃɂ���
			netinst->NeedCheck();
		}
		netinst->Update();
	}

	int lost = GetLostNetWork();
	if (lost != -1)
	{
		for (auto& netinst : m_NetworkInstances)
		{
			if (netinst->NetHandle() == lost)
			{
				CloseNetWork(netinst->NetHandle());
				netinst->SetHandle(-1); // �ڑ����폜
				break;
			}
		}
	}
}

void NetworkManager::Draw()
{
	// �f�o�b�O
	int c = 0;
	for (auto& netinst : m_NetworkInstances)
	{
		DrawFormatString(700, 100 + c * 40, 0xffffff, "NetHandle: %d", netinst->NetHandle());
		DrawFormatString(400, 100 + c * 40, 0xffffff, "IP: %d.%d.%d.%d",
		netinst->m_CurrentIP.d1,
		netinst->m_CurrentIP.d2,
		netinst->m_CurrentIP.d3,
		netinst->m_CurrentIP.d4);
		c++;
	}
}
