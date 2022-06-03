#pragma once
#include "Client.h"

class CChatClient : public CClient
{
private:

public:
	CChatClient() {};
	~CChatClient()
	{
		//Send(CONNECTION_CLOSE_BY_CLIENT);
		//this->Close();
	};
	std::wstring m_strUserName;
	DWORD ConnectedThread();
	bool Connect(ST_SERVER_INFO stServerInfo, std::wstring strUserName);
	int Send(PACKET_HEADER* packet);
	int Recv(PACKET_HEADER* packet, int nLength);
	int Peek(PACKET_HEADER* packet, int nLength);

	std::vector<std::wstring> RecvChatData();

	void setUserName(std::wstring strName);
	std::wstring getUserName();

	//Test¿ë
	std::vector<std::wstring> m_vecChatData;
};

