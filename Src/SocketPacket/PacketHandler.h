#pragma once
#include "PacketHandlerSuper.h"

template <typename T>
class CPacketHandler : CPacketHandlerSuper
{
	E_PACKET_TYPE GetType()
	{
		return T::m_nType;
	}
	void OnRecv(void* pData, size_t tDataSize)
	{

	}
	virtual void OnRecv(T* packet);
};