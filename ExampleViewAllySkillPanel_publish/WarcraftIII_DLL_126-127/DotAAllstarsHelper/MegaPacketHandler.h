#pragma once


extern int PacketClassPtr;
extern int pGAME_SendPacket;
void SendPacket( unsigned char * packetData, unsigned int  size );

void  Packet_Uninitalize( );
void UninitializePacketHandler(   );
void InitializePacketHandler(   );
