#pragma once
class CPacketHandlerSuper
{
protected:
	std::vector<CPacketHandlerSuper*> m_vecHandler;
public:
	CPacketHandlerSuper()
	{
		m_vecHandler.push_back(this);
	}
	virtual E_PACKET_TYPE GetType();
	virtual void OnRecv(void* pData, size_t tDataSize);
};