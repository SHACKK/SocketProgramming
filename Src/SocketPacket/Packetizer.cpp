#include "pch.h"
#include "Packetizer.h"
#include "PacketHandlerSuper.h"
#include "PacketHandler.h"
#include "PacketHandler.cpp"

void CPacketizer::OnRecv(REQ_CONNECT* packet)
{
}

void CPacketizer::OnRecv(RES_ACCEPT* packet)
{
}

void CPacketizer::OnRecv(REQ_MESSAGE_INPUT* packet)
{
}

void CPacketizer::OnRecv(RES_MESSAGE* packet)
{
}

void CPacketizer::OnRecv(REQ_DISCONNECT* packet)
{
}

void CPacketizer::ProcessPacket(void* pData, size_t tDataSize)
{
	PACKET_HEADER& header = *(PACKET_HEADER*)pData;
	std::vector<CPackethandlerSuper*>::iterator iter;

}