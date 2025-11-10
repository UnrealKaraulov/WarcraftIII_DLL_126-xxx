#include "Main.h"
#include "cppcrc.h"

#define MAX_PACKET_SIZE 1024

bool PacketInitialized = false;

struct CDataStore {
	void** vtable;				//0x0	Offset(DATASTORECACHE1460_VTABLE)
	void* packet;				//0x4	pointer to real packet
	unsigned int		unk_8;				//0x8	default 0
	unsigned int		unk_C;				//0xC	default 0x5B4
	unsigned int		sizePacket;			//0x10  sizeof(packet)
	int				recvbytes;				//0x14	default -1
};


int Handle_Jass_Packet(unsigned char* packetraw, size_t packetsize, int pid);

/*  I just use CNetCommandTeamChangeAlliance for send and recv packets  */
typedef int(__fastcall* RecvNetCommand)(int a1, int a2, CDataStore* packetdata, char pid, int a5, int a6, int a7, int a8, int a9);
RecvNetCommand CNetCommandTeamChangeAlliance_org;
RecvNetCommand CNetCommandTeamChangeAlliance_ptr;

typedef int(__thiscall* sub_6F4C2C10)(CDataStore* packetdata, unsigned char* outval);
sub_6F4C2C10 CDataStoreByteReader;

int __fastcall CNetCommandTeamChangeAlliance_my(int a1, int a2, CDataStore* packetdata, char pid, int a5, int a6, int a7, int a8, int a9)
{
	if (packetdata && packetdata->packet && packetdata->sizePacket >= 6 && ((unsigned char*)packetdata->packet)[packetdata->recvbytes ? packetdata->recvbytes : 1] == 0xFF)
	{
		unsigned char* packetraw = (unsigned char*)packetdata->packet;

		if (PacketInitialized)
		{
			int recvbytes = Handle_Jass_Packet(packetraw + packetdata->recvbytes - 1, packetdata->sizePacket - packetdata->recvbytes + 1, pid);

			while (recvbytes > 0)
			{
				unsigned char data = 0;
				CDataStoreByteReader(packetdata, &data);
				recvbytes -= 1;
			}

			packetdata->recvbytes -= 1;
		}

		return 1;

	}

	return CNetCommandTeamChangeAlliance_ptr(a1, a2, packetdata, pid, a5, a6, a7, a8, a9);
}

std::vector<unsigned char> BytesToSend;
std::vector<unsigned char> BytesToRecv;


unsigned char* PacketClassPtr;
unsigned char* pGAME_SendPacket;

typedef void* (__fastcall* GAME_SendPacket_p) (CDataStore* packet, unsigned long);
GAME_SendPacket_p GAME_SendPacket = NULL;

typedef int(__cdecl* p_6F53E6B0)();
p_6F53E6B0 sub_6F53E6B0;
typedef int(__fastcall* pGetBnetSockStr)(unsigned char* a1/*&stru_6FAD0090*/, int unk/*zero*/, int net_addr, int a3/*0*/, int* ptr_net_addr, int a5 /*0*/, int a6 /* 1 */);
pGetBnetSockStr GetBnetSockStr;

unsigned char* GetBnetChatSock()
{
	sub_6F53E6B0 = (p_6F53E6B0)(GameDll + 0x53E6B0);
	GetBnetSockStr = (pGetBnetSockStr)(GameDll + 0x688F30);
	int netaddr = sub_6F53E6B0();
	int bnetChat = GetBnetSockStr(GameDll + 0xAD0090, 0, netaddr, 0, &netaddr, 0, 1);
	if (bnetChat > 0)
	{
		return *(unsigned char**)(bnetChat + 0x414);
	}
	return 0;
}


unsigned char* GetHostBotChatSock()
{
	int hostbotsock = *(int*)(GameDll + 0xACFFA4);
	if (hostbotsock > 0)
	{
		hostbotsock = *(int*)(hostbotsock + 0x148);
		if (hostbotsock > 0)
		{
			return *(unsigned char**)(hostbotsock + 0x3C);
		}
	}
	return 0;
}


typedef void(__fastcall* GAME_SendPacketDir_p) (unsigned char* tcpoffs, unsigned char* zero, unsigned char* packetData, int len);
GAME_SendPacketDir_p GAME_SendPacketDir;


std::vector<unsigned char> send_data_buf;

void SendData(unsigned char* sockstr, unsigned char header, unsigned char packetid, unsigned char* data, int datalen)
{
	if (!sockstr)
		return;
	send_data_buf.clear();
	short totallen = (short)(datalen + 4);
	send_data_buf.push_back(header);
	send_data_buf.push_back(packetid);
	send_data_buf.push_back(((unsigned char*)&totallen)[0]);
	send_data_buf.push_back(((unsigned char*)&totallen)[1]);
	for (int i = 0; i < datalen; i++)
	{
		send_data_buf.push_back(data[i]);
	}
	GAME_SendPacketDir = (GAME_SendPacketDir_p)(GameDll + 0x6DF040);
	EnterCriticalSection((LPCRITICAL_SECTION) * (unsigned char**)(sockstr + 0x44));
	GAME_SendPacketDir(sockstr, sockstr, &send_data_buf[0], (int)send_data_buf.size());
	LeaveCriticalSection((LPCRITICAL_SECTION) * (unsigned char**)(sockstr + 0x44));
}

void SendIngameAction(unsigned char* data, int datalen)
{
	unsigned char* hostbotsocket = GetHostBotChatSock();
	if (hostbotsocket)
	{
		uint32_t crc_value = CRC32::CRC32::calc((const uint8_t*)data, (size_t)datalen);
		std::vector<unsigned char> senddata;
		senddata.push_back(((unsigned char*)&crc_value)[0]);
		senddata.push_back(((unsigned char*)&crc_value)[1]);
		senddata.push_back(((unsigned char*)&crc_value)[2]);
		senddata.push_back(((unsigned char*)&crc_value)[3]);
		for (int i = 0; i < datalen; i++)
		{
			senddata.push_back(data[i]);
		}
		SendData(hostbotsocket, 0xF7, 0x26, &senddata[0], (int)senddata.size());
	}
}



void SendPacket(unsigned char* packetData, unsigned int  size, bool asyns)
{
	// @warning: this function thread-unsafe, do not use it in other thread.
	// note: this is very useful function, in fact this function
	// does wc3 ingame action, so you can use it for anything you want,
	// including unit commands and and gameplay commands,
	// i suppose its wc3 single action W3GS_INCOMING_ACTION (c) wc3noobpl.

	if (asyns)
	{
		CDataStore packet;
		memset(&packet, 0, sizeof(CDataStore));
		packet.vtable = (void**)PacketClassPtr; // Packet Class
		packet.packet = packetData;
		packet.sizePacket = size;
		packet.unk_8 = 0;
		packet.unk_C = 0x5B4;
		packet.recvbytes = -1;
		if (!GAME_SendPacket)
			GAME_SendPacket = (GAME_SendPacket_p)(pGAME_SendPacket);
		GAME_SendPacket(&packet, 0);
	}
	else
	{
		SendIngameAction(packetData, (int)size);
	}
}


int CallbackTrigger = 0;

size_t size_of_sended_packet = 0;

void __stdcall Packet_Clear(int)
{
	BytesToSend.clear();
	BytesToRecv.clear();
	BytesToSend.push_back(0x50);
	// packet header
	BytesToSend.push_back(0xFF);
	// packet size
	BytesToSend.push_back(0);
	BytesToSend.push_back(0);
	BytesToSend.push_back(0);
	BytesToSend.push_back(0);
	// packet data
}

void __stdcall Packet_Initialize(int TriggerHandle)
{
	PacketInitialized = true;
	BytesToSend.clear();
	BytesToRecv.clear();
	CallbackTrigger = TriggerHandle;
}

void  Packet_Uninitalize()
{
	PacketInitialized = false;
	BytesToSend.clear();
	BytesToRecv.clear();
	CallbackTrigger = NULL;
}

void __stdcall Packet_PushInteger(unsigned int IntegerData)
{

	if (BytesToSend.size() >= MAX_PACKET_SIZE)
		return;

	if (BytesToSend.size() <= 2) // error ?
		return;

	BytesToSend.insert(BytesToSend.end(), (unsigned char*)&IntegerData, ((unsigned char*)&IntegerData) + 4);
	*(unsigned int*)&BytesToSend[2] += 4;
}

int __stdcall Packet_PopInteger(int)
{

	if (BytesToRecv.size() >= 4)
	{
		int retval = *(int*)&BytesToRecv[0];
		BytesToRecv.erase(BytesToRecv.begin(), BytesToRecv.begin() + 4);

		return retval;
	}

	return 0;
}

void __stdcall Packet_PushReal(float RealData)
{

	if (BytesToSend.size() >= MAX_PACKET_SIZE)
		return;
	if (BytesToSend.size() <= 2) // error ?
		return;
	BytesToSend.insert(BytesToSend.end(), (unsigned char*)&RealData, ((unsigned char*)&RealData) + 4);
	*(unsigned int*)&BytesToSend[2] += 4;
}

int __stdcall Packet_PopReal(int)
{
	if (BytesToRecv.size() >= 4)
	{
		int retval = *(int*)&BytesToRecv[0];
		BytesToRecv.erase(BytesToRecv.begin(), BytesToRecv.begin() + 4);
		return retval;
	}
	return 0;
}

void __stdcall Packet_Send(int)
{
	SendPacket(&BytesToSend[0], BytesToSend.size());
	BytesToSend.clear();
}

void __stdcall Packet_Send_Ex(int)
{
	SendPacket(&BytesToSend[0], BytesToSend.size(), false);
	BytesToSend.clear();
}

int PacketTriggerPlayerId = 0;
int __stdcall Packet_GetTriggerPlayerId(int)
{
	return PacketTriggerPlayerId;
}

int __stdcall CallTrigger(int TriggerHandle)
{
	TriggerExecute(TriggerHandle);
	return 0;
}

int Handle_Jass_Packet(unsigned char* packetraw, size_t _packetsize, int pid)
{
	if (!_packetsize || !packetraw)
		return 0;

	int readdata = 1;
	PacketTriggerPlayerId = pid;
	BytesToRecv.clear();
	BytesToRecv.assign(packetraw, packetraw + _packetsize);

	if (BytesToRecv.size() >= 6 && *(unsigned short*)&BytesToRecv[0] == 0xFF50)
	{
		//unsigned short header = *(unsigned short*)&BytesToRecv[0];

		BytesToRecv.erase(BytesToRecv.begin(), BytesToRecv.begin() + 2);

		unsigned int packetsize = *(int*)&BytesToRecv[0];

		BytesToRecv.erase(BytesToRecv.begin(), BytesToRecv.begin() + 4);

		readdata += 5;

		if (packetsize >= 0 && packetsize <= MAX_PACKET_SIZE && BytesToRecv.size() >= packetsize)
		{
			readdata += packetsize;
			if (packetsize >= 4)
			{
				readdata += packetsize + 4 + 2;

				//PrintText( "Get id!" );
				int packetid = *(int*)&BytesToRecv[0];

				if (packetid == 0xA1A2A3A0 && packetsize >= 8)
				{
					int packetid2 = *(int*)&BytesToRecv[4];

					if (packetid2 == 0xA1A2A3A0 || packetid == 0xA1A2A3A1)
					{
						BytesToRecv.erase(BytesToRecv.begin(), BytesToRecv.begin() + 4);
						BytesToRecv.erase(BytesToRecv.begin(), BytesToRecv.begin() + 4);
						packetsize -= 8;


						//FILE * f;
						//fopen_s( &f, "packetin.txt", "a+" );

						//fprintf_s( f, "Recv packets %i (real : %i) size. Header: %04X:\n", BytesToRecv.size( ), packetsize, header );
						//for ( int i = 0; i < packetsize; i++ )
						//{
						//	fprintf_s( f, "%02X", BytesToRecv[ i ] );
						//}
						//fprintf_s( f, "%s", "\n" );
						//fclose( f );

						//AddNewPaTestData(BytesToRecv, pid, packetsize, packetid == 0xA1A2A3A1);
					}
				}
				if (CallbackTrigger)
				{
					CallTrigger(CallbackTrigger);
				}
			}
			//BytesToRecv.erase( BytesToRecv.begin( ), BytesToRecv.begin( ) + packetsize );
			//}
			/*else
				break;
			*/
		}
	}
	return readdata;
}


void InitializePacketHandler()
{
	PacketClassPtr = GameDll + 0x932D2C;
	pGAME_SendPacket = GameDll + 0x54D970;

	CNetCommandTeamChangeAlliance_org = (RecvNetCommand)(GameDll + 0x5422B0);
	MH_CreateHook(CNetCommandTeamChangeAlliance_org, &CNetCommandTeamChangeAlliance_my, reinterpret_cast<void**>(&CNetCommandTeamChangeAlliance_ptr));
	MH_EnableHook(CNetCommandTeamChangeAlliance_org);
	CDataStoreByteReader = (sub_6F4C2C10)(GameDll + 0x4C2C10);

}


void  UninitializePacketHandler()
{
	if (CNetCommandTeamChangeAlliance_org)
		MH_DisableHook(CNetCommandTeamChangeAlliance_org);
}
