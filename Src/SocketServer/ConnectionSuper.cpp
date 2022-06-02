#include "pch.h"
#include "ConnectionSuper.h"

DWORD WINAPI ConnectionThreadCaller(void* pContext)
{
	CConnectionSuper* connection = (CConnectionSuper*)pContext;
	return connection->ConnectionThread();
}

DWORD CConnectionSuper::ConnectionThread()
{
	onConnect();
	PACKET_HEADER packet;
	
	while (true)
	{
		// Peek 후 Type이 Discconect이면 break;
		onRecv();
	}
	onClose();
	//::closesocket(m_ConnectionSocket);

	return 0;
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	m_ConnectionSocket = acceptedSocket;
	m_pServer = pServer;

	::CreateThread(nullptr, 0, ConnectionThreadCaller, this, 0, nullptr);
	return 0;
}

int CConnectionSuper::Send(LPCBYTE pData, size_t tSize)
{
	return ::send(m_ConnectionSocket, (const char*)pData, (int)tSize, 0);
}

int CConnectionSuper::Recv(LPBYTE pBuffer, size_t tBufferSize)
{
	return ::recv(m_ConnectionSocket, (char*)pBuffer, (int)tBufferSize, 0);
}

int CConnectionSuper::Peek(LPBYTE pBuffer, size_t tBufferSize)
{
	return ::recv(m_ConnectionSocket, (char*)&pBuffer, (int)tBufferSize, MSG_PEEK);
}

void CConnectionSuper::SetSocket(SOCKET socket, CServer* server)
{
	m_ConnectionSocket = socket;
	m_pServer = server;
}
