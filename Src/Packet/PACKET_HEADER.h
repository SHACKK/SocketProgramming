#pragma once

#pragma pack(push, 1)

enum class PACKET
{
	REQ_CONNECT,
	RES_ACCEPT,
	RES_WAIT,
	REQ_MESSAGEINPUT,
	RES_CHATDATA,
	REQ_DISCONNECT,
	BROADCAST_MESSAGE,
	BROADCAST_DISCONNECT
};


class PACKET_HEADER
{
protected:
	const char m_szMagic[4] = { 'S', 'H', 'A', 'K' };
	int m_nType;
	int m_tSize;
public:
	PACKET_HEADER() {};
	/*CPacketHeader(int nType, int tSize) 
	{
		m_nType = nType;
		m_tSize = tSize;
	};*/
	~PACKET_HEADER() {};

	const char* GetMagic();
	int GetPacketType();
	int GetBodySize();
};
#pragma pack(pop)

// peek���� Ÿ�� �������� case�� �ȿ��� ũ�� �ް� ũ�⸸ŭ recv
// �׷����� recv�� ũ����� �޴� �Լ����߰���.