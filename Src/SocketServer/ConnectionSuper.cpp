#include "pch.h"
#include "ConnectionSuper.h"
#include "../SocketPacket/_SocketPacket.h"

DWORD WINAPI ConnectionThreadCaller(void* pContext)
{
	CConnectionSuper* connection = (CConnectionSuper*)pContext;
	return connection->ConnectionThread();
}

DWORD CConnectionSuper::ConnectionThread()
{
	PACKET_HEADER packet;
	
	onConnect();
	while (true)
	{
		Peek(&packet);
		if (!packet.MagicOK() || packet.GetPacketType() == E_PACKET::REQ_DISCONNECT)
			break;

		onRecv(packet.GetPacketType());
	}
	onClose();

	return 0;
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	m_ConnectionSocket = acceptedSocket;
	m_pServer = pServer;

	::CreateThread(nullptr, 0, ConnectionThreadCaller, this, 0, nullptr);
	return 0;
}

int CConnectionSuper::Send(PACKET_HEADER* packet)
{
	return ::send(m_ConnectionSocket, (const char*)&packet, (int)sizeof(packet), 0);
}

int CConnectionSuper::Recv(PACKET_HEADER* packet, int nLength)
{
	return ::recv(m_ConnectionSocket, (char*)packet, nLength, 0);
}

int CConnectionSuper::Peek(PACKET_HEADER* packet)
{
	return ::recv(m_ConnectionSocket, (char*)packet, 12, MSG_PEEK);
}

CServer* CConnectionSuper::GetServer()
{
	return m_pServer;
}

void CConnectionSuper::SetSocket(SOCKET socket, CServer* server)
{
	m_ConnectionSocket = socket;
	m_pServer = server;
}
