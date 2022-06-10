#pragma once
#include "Server.h"

class CConnectionSuper
{
private:

protected:
	SOCKET m_ConnectionSocket;
	CServer* m_pServer;
	virtual void onConnect() = 0;
	virtual void onRecv(E_PACKET_TYPE ePacketType) = 0;
	virtual void onClose() = 0;
	std::vector<CPacketHandlerSuper*> m_vecHandler;

public:
	DWORD ConnectionThread();
	int Establish(SOCKET acceptedSocket, CServer* pServer);
	int Send(PACKET_HEADER* packet);
	int Recv(PACKET_HEADER* packet, int nLength);
	int Peek(PACKET_HEADER* packet);
	CServer* GetServer();

	//Test¿ë
	void SetSocket(SOCKET socket, CServer* server);
};
