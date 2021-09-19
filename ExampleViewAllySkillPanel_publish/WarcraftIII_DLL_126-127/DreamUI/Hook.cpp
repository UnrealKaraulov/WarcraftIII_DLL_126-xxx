#include "stdafx.h"
#include "Hook.h"
#include "Offsets.h"
#include "GameStructs.h"
#include "Input.h"
#include "War3Window.h"
#include "Game.h"

template <typename Type>
BOOL Patch(Type* p_base, uint32_t offset, Type* p_old_val, Type new_val) {
	DWORD old_protect;
	Type *p_addr = (Type*)((uint32_t)p_base + (int32_t)offset);

	if (VirtualProtect(p_addr, sizeof(Type), PAGE_READWRITE, &old_protect)) {
		if (p_old_val)
			*p_old_val = *p_addr;
		*p_addr = new_val;
		return (BOOL)VirtualProtect(p_addr, sizeof(Type), old_protect, &old_protect);
	}
	return false;
}

DWORD PacketSenderVtable0x24;

void DetourPacketAnalyze(war3::PacketSender * packetSender) {

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
	Patch<DWORD>(
		(DWORD *)Offset(DATASTORECACHE1460_VTABLE),
		0x24,
		&PacketSenderVtable0x24,
		(DWORD)&HookPacketSenderVtable0x24
		);
}