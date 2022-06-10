#include "pch.h"
#include "ChatConnection.h"

void CChatConnection::onConnect()
{

}

void CChatConnection::onRecv()
{

}

void CChatConnection::onClose()
{

}

int CChatConnection::SendChatData(std::vector<std::wstring> vecChatData)
{
	int nRet = 0;
	size_t nVecSize = vecChatData.size();
	nRet += ::send(m_ConnectionSocket, (const char*)&nVecSize, sizeof(size_t), 0);

	for (size_t i = 0; i < nVecSize; i++)
	{
		int nMsgLength = (int)vecChatData[i].size() * (int)sizeof(wchar_t);
		nRet += ::send(m_ConnectionSocket, (const char*)&nMsgLength, sizeof(nMsgLength), 0);
		nRet += ::send(m_ConnectionSocket, (const char*)vecChatData[i].c_str(), nMsgLength, 0);
	}
	return nRet;
}

std::wstring CChatConnection::GetCurrentTimeString()
{
	time_t curTime = time(NULL);
	tm pTime;
	localtime_s(&pTime, &curTime);
	std::wstring strRet = L"[";
	strRet += std::to_wstring(pTime.tm_hour) + L":";
	strRet += std::to_wstring(pTime.tm_min) + L"]";
	//strRet += std::to_wstring(pTime.tm_sec) + L"ÃÊ]";
	
	return strRet;
}

void CChatConnection::SetUserName(std::wstring strUserName)
{
	m_strUserName = strUserName;
}

std::wstring CChatConnection::GetName()
{
	return m_strUserName;
}

