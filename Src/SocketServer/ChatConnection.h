#pragma once
#include "ConnectionSuper.h"

#define CONNECTION_CLOSE_BY_CLIENT L"/CloseByClient"
#define CONNECTION_CLOSE_BY_SERVER L"/CloseByServer"

class CChatConnection : public CConnectionSuper
{
private:
	std::wstring m_strUserName;
public:
	void onConnect();
	void onRecv();
	void onClose();
	//std::wstring Recv();
	//int Send(std::wstring strMessage);
	int SendChatData(std::vector<std::wstring> vecChatData);
	std::wstring GetCurrentTimeString();
	void SetUserName(std::wstring strUserName);
	std::wstring GetName();
};