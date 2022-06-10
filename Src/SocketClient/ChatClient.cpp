#include "pch.h"
#include "ChatClient.h"

DWORD CChatClient::ConnectedThread()
{
	while (true)
	{

	}
	return 0;
}

DWORD WINAPI ConnectedThreadCaller(LPVOID pContext)
{
	CChatClient* client = (CChatClient*)pContext;
	return client->ConnectedThread();
}

bool CChatClient::Connect(ST_SERVER_INFO stServerInfo, std::wstring strUserName)
{
	__super::Connect(stServerInfo);

	return true;
}

int CChatClient::Send(PACKET_HEADER* packet)
{
	return ::send(m_hClientSocket, (const char*)&packet, sizeof(packet), 0);
}

int CChatClient::Recv(PACKET_HEADER* packet, int nLength)
{
	return ::recv(m_hClientSocket, (char*)packet, nLength, 0);
}

int CChatClient::Peek(PACKET_HEADER* packet, int nLength)
{
	return ::recv(m_hClientSocket, (char*)&packet, nLength, MSG_PEEK);
}

std::vector<std::wstring> CChatClient::RecvChatData()
{
	std::vector<std::wstring> vecChatData;
	size_t nVecSize;
	::recv(m_hClientSocket, (char*)&nVecSize, sizeof(size_t), 0);
	vecChatData.resize(nVecSize);

	for (size_t i = 0; i < nVecSize; i++)
	{
		int nMsgLength;
		::recv(m_hClientSocket, (char*)&nMsgLength, sizeof(nMsgLength), 0);
		vecChatData[i].resize(nMsgLength / sizeof(wchar_t));
		::recv(m_hClientSocket, (char*)vecChatData[i].c_str(), nMsgLength, 0);
	}

	return vecChatData;
}

void CChatClient::setUserName(std::wstring strName)
{
	m_strUserName = strName;
}

std::wstring CChatClient::getUserName()
{
	if (m_strUserName.empty())
		return L"NoName";
	else
		return m_strUserName;
}
