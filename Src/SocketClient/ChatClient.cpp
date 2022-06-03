#include "pch.h"
#include "ChatClient.h"

DWORD CChatClient::ConnectedThread()
{
	while (true)
	{
		PACKET_HEADER packet;
		Peek(&packet, (int)sizeof(PACKET_HEADER));
		E_PACKET ePacketType = packet.GetPacketType();

		switch (ePacketType)
		{
		case E_PACKET::RES_ACCEPT:
		{
			RES_ACCEPT pckAccept;
			Recv(&pckAccept, (int)sizeof(RES_ACCEPT));
			pckAccept.onRecv(pckAccept);
		}
		case E_PACKET::RES_CHATDATA:
		{
			RES_CHATDATA pckChatData;
			Recv(&pckChatData, (int)sizeof(RES_CHATDATA));
			pckChatData.onRecv(pckChatData);
		}
		case E_PACKET::BROADCAST_MESSAGE:
		{
			BROADCAST_MESSAGE pckMessage;
			Recv(&pckMessage, (int)sizeof(BROADCAST_MESSAGE));
			pckMessage.onRecv(pckMessage);
		}
		case E_PACKET::BROADCAST_DISCONNECT:
		{
			BROADCAST_DISCONNECT pckDisconnect;
			Recv(&pckDisconnect, (int)sizeof(pckDisconnect));
			pckDisconnect.onRecv(pckDisconnect);
		}
		}
		std::wstring strMessage = client.Recv();

		if (!wcscmp(strMessage.c_str(), CONNECTION_CLOSE_BY_SERVER) || strMessage.empty())
		{
			client.Close();
			break;
		}

		vecChatData.push_back(strMessage);
		PrintChatData();
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
	REQ_CONNECT pckConnect;
	pckConnect.m_strUserName = m_strUserName;
	this->Send(&pckConnect);

	PACKET_HEADER packet;
	::recv(m_hClientSocket, (char*)&packet, (int)sizeof(packet), MSG_PEEK);
	if (!packet.MagicOK() || packet.GetPacketType() != E_PACKET::RES_ACCEPT)
		return false;

	RES_ACCEPT pckAccept;
	this->Recv(&pckAccept, sizeof(RES_ACCEPT));
	HANDLE hUpdateChatDataThread = CreateThread(nullptr, 0, ConnectedThreadCaller, this, 0, nullptr);
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
