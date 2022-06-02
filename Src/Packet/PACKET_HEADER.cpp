#include "pch.h"
#include "PACKET_HEADER.h"

const char* PACKET_HEADER::GetMagic()
{
	return &m_szMagic[0];
}

int PACKET_HEADER::GetPacketType()
{
	return m_nType;
}

int PACKET_HEADER::GetBodySize()
{
	return m_tSize;
}