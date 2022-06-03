#pragma once
class CPacketHandlerSuper
{
	virtual E_PACKET_TYPE GetType();
	virtual void OnRecv(void* pData, size_t tDataSize);
};