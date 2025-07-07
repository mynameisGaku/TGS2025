#include "NetworkInstance.h"
#include <src/util/file/json/RawJson.h>

void NetworkInstance::Update()
{
	if (m_NeedCheck == false)
		return;

	// 受信データがjsonかどうか確認する
	if (RawJson::CheckIsJson(m_MessageBuf) == false)
		return;

	// Jsonデータをパースして処理する
	auto jsonData = RawJson::Create({
		std::string(m_MessageBuf)
		});

	// コマンドに応じた処理を行う
	if (jsonData["Command"] == "")
	{

	}
}

int NetworkInstance::NetHandle()
{
	return m_hNet;
}

void NetworkInstance::SetHandle(int hNet)
{
	m_hNet = hNet;
	m_NeedCheck = true;
}

void NetworkInstance::Reset()
{
	m_hNet = -1;
	m_MessageBuf[0] = '\0';
	m_NeedCheck = false;
}

void NetworkInstance::SetIP(const IPDATA& ip)
{
	m_PrevIP = m_CurrentIP; // 前回のIPを保存
	m_CurrentIP.d1 = ip.d1;
	m_CurrentIP.d2 = ip.d2;
	m_CurrentIP.d3 = ip.d3;
	m_CurrentIP.d4 = ip.d4;
}

bool NetworkInstance::CompareCurrentIP(const IPDATA& other) const
{
	bool result = (
		m_CurrentIP.d1 == other.d1 &&
		m_CurrentIP.d2 == other.d2 &&
		m_CurrentIP.d3 == other.d3 &&
		m_CurrentIP.d4 == other.d4);

	return result;
}

bool NetworkInstance::ComparePrevIP(const IPDATA& other) const 
{
	bool result = (
		m_PrevIP.d1 == other.d1 &&
		m_PrevIP.d2 == other.d2 &&
		m_PrevIP.d3 == other.d3 &&
		m_PrevIP.d4 == other.d4);

	return result;
}
