#pragma once
#include "PacketHandler.h"

class Packetizer 
	: public CPacketHandler<REQ_CONNECT>
	, public CPacketHandler<RES_ACCEPT>
	, public CPacketHandler<REQ_MESSAGE_INPUT>
	, public CPacketHandler<RES_MESSAGE>
	, public CPacketHandler<REQ_DISCONNECT>
{
};