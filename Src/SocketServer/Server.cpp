#include "pch.h"
#include "Server.h"
#include "ConnectionSuper.h"
#include "ChatConnection.h"
#include "../SocketPacket/_SocketPacket.h"

DWORD WINAPI AcceptThreadCaller(void* pContext)
{
	CServer* server = (CServer*)pContext;
	return server->AcceptThread();
}

DWORD WINAPI DisAcceptThreadCaller(void* pContext)
{
	CServer* server = (CServer*)pContext;
	return server->DisAcceptThread();
}

int CServer::StartUp(ST_SERVER_INIT stInit)
{
	try {
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = INADDR_ANY;
		service.sin_port = htons(stInit.wPort);
		int nExcep = ::bind(m_ListenSocket, (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Bind");

		nExcep = listen(m_ListenSocket, 200);
		if (SOCKET_ERROR == nExcep) throw std::exception("Failed to Listen");

		// Fill the m_queReady
		m_vecConnectionInstance = stInit.vecConnections;
		for (auto iter : m_vecConnectionInstance)
			m_queReady.push(iter);
		if (m_queReady.empty()) throw std::exception("Connection Pool doesn't exist");

		printf("[SUCCESS] Server StartUp...\n");

		hAcceptThread = ::CreateThread(nullptr, 0, AcceptThreadCaller, this, 0, nullptr);
		hDisAcceptThread = ::CreateThread(nullptr, 0, DisAcceptThreadCaller, this, 0, nullptr);
		

		return 0;
	}
	catch (std::exception& e)
	{
		printf("[Error] : %s\n %d\n", e.what(), WSAGetLastError());
		return 1;
	}
}

void CServer::ShutDown()
{
	REQ_DISCONNECT pckDisconnect;
	Broadcast(&pckDisconnect);
	WaitForSingleObject(hAcceptThread, INFINITE);
	WaitForSingleObject(hDisAcceptThread, INFINITE);
}

void CServer::Broadcast(PACKET_HEADER* packet)
{
	std::set<CConnectionSuper*>::iterator iter;
	for (iter = m_setConnected.begin(); iter != m_setConnected.end(); iter++)
	{
		(*iter)->Send(packet);
	}
}

void CServer::InsertConnectedSet(CConnectionSuper* newConnection)
{
	mtx_setConnected.lock();
	m_setConnected.insert(newConnection);
	mtx_setConnected.unlock();

	wprintf(L"[INFO] Client connected!! (%d/%d)\n", (int)m_setConnected.size(), MAX_CONNECTION_SIZE);
}

std::vector<std::wstring> CServer::GetChatData()
{
	return m_vecChatData;
}

bool CServer::UpdateChatData(std::wstring strMessage)
{
	mtx_ChatData.lock();
	if (MAX_CHATDATA_SIZE < m_vecChatData.size())	
		m_vecChatData.erase(m_vecChatData.begin());
	m_vecChatData.push_back(strMessage);

	REQ_DISCONNECT pckMessage;
	Broadcast(&pckMessage);

	mtx_ChatData.unlock();	

	return true;
}

void CServer::DisConnect(CConnectionSuper* pConnection)
{
	mtx_setConnected.lock();
	m_setConnected.erase(pConnection);
	mtx_setConnected.unlock();

	wprintf(L"[INFO] Client disconnected!! (%d/%d)\n", (int)m_setConnected.size(), MAX_CONNECTION_SIZE);

	mtx_queDiscon.lock();
	m_queDiscon.push(pConnection);
	mtx_queDiscon.unlock();
}

DWORD CServer::AcceptThread()
{
	while (true)
	{
		if (m_queReady.empty())
			continue;

		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);

		SOCKET hConnectionSocket = ::accept(m_ListenSocket, &RemoteInfo, &nRemoteInfoSize);

		mtx_queReady.lock();
		CConnectionSuper* newConnection = m_queReady.front();
		m_queReady.pop();
		mtx_queReady.unlock();

		newConnection->Establish(hConnectionSocket, this);
	}
}

// 클라이언트와 연결 해제
DWORD CServer::DisAcceptThread()
{
	while (true)
	{
		if (m_queDiscon.empty())
			continue;

		//mutex 처리
		m_queReady.push(m_queDiscon.front());
		m_queDiscon.pop();
	}
}
