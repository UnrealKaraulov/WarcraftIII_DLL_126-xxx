#include "stdafx.h"
#include "Hook.h"
#include "Offsets.h"
#include "GameStructs.h"
#include "Input.h"
#include "War3Window.h"
#include "Game.h"

template <typename Type>
int Patch(Type* p_base, unsigned int offset, Type* p_old_val, Type new_val) {
	unsigned long old_protect;
	Type* p_addr = (Type*)((unsigned int)p_base + (int)offset);

	if (VirtualProtect(p_addr, sizeof(Type), PAGE_READWRITE, &old_protect)) {
		if (p_old_val)
			*p_old_val = *p_addr;
		*p_addr = new_val;
		return (int)VirtualProtect(p_addr, sizeof(Type), old_protect, &old_protect);
	}
	return false;
}

unsigned long PacketSenderVtable0x24;

void DetourPacketAnalyze(war3::PacketSender* packetSender) {

	if (*(uint8_t*)(packetSender->packet) == 4
		&& packetSender->sizePacket == 5
		)
	{
	}
	else {
		//;//OutputDebug("[%u]Packet Id = 0x%X", TimeRaw(), *(uint8_t*)(packetSender->packet));
		//PacketAnalyze(packetSender);
	}
}


void __declspec(naked) HookPacketSenderVtable0x24() {
	__asm {
		pushad
		pushfd

		push ecx
		call DetourPacketAnalyze
		add esp, 4

		popfd
		popad
		jmp PacketSenderVtable0x24
	}
}

void Hook_Init() {
	//±¾µØ·â°ü DataStoreCache1460 vtable 0x24
	Patch<unsigned long>(
		(unsigned long*)Offset(DATASTORECACHE1460_VTABLE),
		0x24,
		&PacketSenderVtable0x24,
		(unsigned long)&HookPacketSenderVtable0x24
		);
}