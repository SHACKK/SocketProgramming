#pragma once
#include "Server.h"

class CConnectionSuper
{
private:

protected:
	SOCKET m_ConnectionSocket;
	CServer* m_pServer;
	virtual void onConnect() = 0;
	virtual void onRecv() = 0;
	virtual void onClose() = 0;

public:
	DWORD ConnectionThread();
	int Establish(SOCKET acceptedSocket, CServer* pServer);
	int Send(LPCBYTE pData, size_t tSize);
	int Recv(LPBYTE pBuffer, size_t tBufferSize);
	int Peek(LPBYTE pBuffer, size_t tBufferSize);
	int Send(PACKET_HEADER* packet);
	int Recv(PACKET_HEADER* packet, int nLength);
	int Peek(PACKET_HEADER* packet);

	//Test¿ë
	void SetSocket(SOCKET socket, CServer* server);
};
