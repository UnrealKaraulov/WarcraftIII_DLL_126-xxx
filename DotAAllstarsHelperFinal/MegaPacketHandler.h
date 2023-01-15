#pragma once


extern unsigned char* PacketClassPtr;
extern unsigned char* pGAME_SendPacket;
void SendPacket(unsigned char* packetData, unsigned int  size);

void  Packet_Uninitalize();
void UninitializePacketHandler();
void InitializePacketHandler();
