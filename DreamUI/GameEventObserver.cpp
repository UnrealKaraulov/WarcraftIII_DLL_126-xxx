#include "stdafx.h"

#include "ObjectHookManager.h"

#include <TlHelp32.h>
#include "GameEventObserver.h"
#include "FunctionTemplate.h"

#include "Foundation.h"
#include "Tools.h"
#include "BaseStructs.h"
#include "GameStructs.h"
#include "Observer.h"
#include "SystemTools.h"

#include "Game.h"
#include "GameTime.h"
#include "Timer.h"
#include "Input.h"
#include "Offsets.h"
#include "Event.h"
#include "RefreshManager.h"
#include "BinaryDumper.h"

ObjectHookManager* HookManager;

static war3::CObserver* War3NetEventObserver;
static war3::CNetData* War3NetData;
static war3::CNetObserver* War3NetObserver;
static war3::CObserver* War3InputObserver;

war3::UnitInstanceGenerator* War3UnitInstanceGenerator;
war3::ItemInstanceGenerator* War3ItemInstanceGenerator;
war3::TriggerInstanceGenerator* War3TriggerInstanceGenerator;
war3::AbilityAttackInstanceGenerator* War3AbilityAttackInstanceGenerator;
war3::AbilitySpellInstanceGenerator* War3AbilitySpellInstanceGenerator;

war3::MissileInstanceGenerator* War3MissileInstanceGenerator;
war3::MissileSplashInstanceGenerator* War3MissileSplashInstanceGenerator;
war3::MissileBounceInstanceGenerator* War3MissileBounceInstanceGenerator;
war3::MissileLineInstanceGenerator* War3MissileLineInstanceGenerator;
war3::ArtilleryInstanceGenerator* War3ArtilleryInstanceGenerator;
war3::ArtilleryLineInstanceGenerator* War3ArtilleryLineInstanceGenerator;

war3::CGameUI* War3GameUI;

Event CustomEvent;

static bool SafeMode;


//处理游戏还没正式开始前发生的事件
static std::list<Event*> PreGameEventList;
void GameEventObserver_ProcessPreGameEvents() {
	for (std::list<Event*>::iterator iter = PreGameEventList.begin();
		iter != PreGameEventList.end();
		++iter)
	{
		MainDispatcher()->dispatch(*iter);
		delete* iter;
	}
	PreGameEventList.clear();
}

void* OrgWorldFrame0x30;
unsigned int __fastcall WorldFrame0x30Hook(war3::CWorldFrameWar3* object) {
	unsigned int rv = dreamaero::generic_this_call<unsigned int>(
		OrgWorldFrame0x30,
		object
		);

	RefreshManager_Update();

	return rv;
}

void* OrgGameUIEventHandler;
OBSERVER_EVENT_HANDLER(GameUIEventHook) {
	//OutputDebug("0x%X", EVENT->id);
	switch (EVENT->id) {
	case war3::EVENT_ITEM_CLICK:
		ItemClick_Report(EVENT);
		break;
	}

	return dreamaero::generic_this_call<unsigned int>(
		OrgGameUIEventHandler,
		OBSERVER,
		EVENT
		);
}

static BinaryDumper* Dumper = NULL; //使用console debug

void* OrgNetEventHandler;
OBSERVER_EVENT_HANDLER(NetEventHook) {
	/*
	//if (EVENT->id != war3::EVENT_GAMETICK && EVENT->id != war3::EVENT_GAMEIDLETICK && EVENT->id != war3::EVENT_GAMEASYNCIDLETICK)
	if (EVENT->id >= 0xA0010 && EVENT->id <= 0xA0015){
		//OutputDebug("[%u]NetEvent ID = 0x%X, Name = %s", TimeRaw(), EVENT->id, RTTIClassNameGet(EVENT));
		uint8_t DumpData[0x50];
		unsigned int size = TryDumpObject(EVENT, DumpData, 0x50);
		if (!Dumper)
			Dumper = new BinaryDumper(NULL); //临时，没清理
		Dumper->dump(EVENT, size, RTTIClassNameGet(EVENT));
	}
	*/

	unsigned int time;
	war3::CWorldFrameWar3* worldFrame;
	switch (EVENT->id) {
	case war3::EVENT_GAMESTART:
		break;
	case war3::EVENT_GAMEREADY:

		HookManager->clearAllDynamic(); //清理上一次游戏的遗留
		Game_Init();

		War3GameUI = GameUIObjectGet();
		//HookManager->replace(War3GameUI, 57, true);
	//	OrgGameUIEventHandler = HookManager->apply(War3GameUI, 0xC, GameUIEventHook);

		worldFrame = War3GameUI->world;
		//	HookManager->replace(worldFrame, 57, true);
		//	OrgWorldFrame0x30 = HookManager->apply(worldFrame, 0x30, WorldFrame0x30Hook);

			//HookManager->analysis(War3GameUI->world, 57);

		break;
	case war3::EVENT_GAMETICK:
		time = War3NetData->time;
		GameTime_Update(time);
		Timer_Update(time);
		break;
	case war3::EVENT_GAMELEAVE:
		Game_Cleanup();
		break;
	}

	return dreamaero::generic_this_call<unsigned int>(
		OrgNetEventHandler,
		OBSERVER,
		EVENT
		);
}

void* OrgInputEventHandler;
OBSERVER_EVENT_HANDLER(InputEventHook) {
	//OutputDebug("InputEvent ID = 0x%X, Name = %s", EVENT->id, RTTIClassNameGet(EVENT));
	if (Input_Update(EVENT))
	{
		return dreamaero::generic_this_call<unsigned int>(
			OrgInputEventHandler,
			OBSERVER,
			EVENT
			);
	}
	return NULL;
}

OBSERVER_EVENT_HANDLER(TestEventHandler) {

	return 0;
}


//--------------------------------------------------------------------------------
// SimpleButton Hook TODO 自动hook每个simplebutton
//--------------------------------------------------------------------------------
static void* FakeSimpleButtonVtableData[1 + 0x1D];
static void** FakeSimpleButtonVtable = &(FakeSimpleButtonVtableData[1]);

static void* SimpleButtonVtable0x50;
static void* SimpleButtonVtable0x54;
static Event MouseEventObject;
static MouseEventData MouseEventObjectData;
void __cdecl SimpleButtonMouseEventHandler(war3::CSimpleButton* button, war3::CMouseEvent* evt) {
	//OutputScreen(10, "event: button 0x%X has event 0x%X", button, evt->baseEvent.id);
	switch (evt->baseEvent.id) {
	case war3::EVENT_MOUSE_DOWN:
		if (IsInGame()) {
			MouseEventObject.setId(EVENT_MOUSE_DOWN);
			MouseEventObjectData.mouseCode = reinterpret_cast<war3::CMouseEvent*>(evt)->keyCurrent;
			MouseEventObjectData.x = reinterpret_cast<war3::CMouseEvent*>(evt)->x;
			MouseEventObjectData.y = reinterpret_cast<war3::CMouseEvent*>(evt)->y;
			MouseEventObjectData.buttonPushed = button;
			MouseEventObject.setData<MouseEventData>(&MouseEventObjectData);
			MainDispatcher()->dispatch(&MouseEventObject);
		}
		break;
	case war3::EVENT_MOUSE_UP:
		if (IsInGame()) {
			MouseEventObject.setId(EVENT_MOUSE_UP);
			MouseEventObjectData.mouseCode = reinterpret_cast<war3::CMouseEvent*>(evt)->keyCurrent;
			MouseEventObjectData.x = reinterpret_cast<war3::CMouseEvent*>(evt)->x;
			MouseEventObjectData.y = reinterpret_cast<war3::CMouseEvent*>(evt)->y;
			MouseEventObjectData.buttonPushed = button;
			MouseEventObject.setData<MouseEventData>(&MouseEventObjectData);
			MainDispatcher()->dispatch(&MouseEventObject);
		}
		break;
	default:
		break;
	}

}

void __declspec(naked) SimpleButtonMouseDownEventHook() {
	__asm {
		push ecx;//保护
		mov eax, [esp + 8];
		push eax;
		mov eax, ecx;
		push eax;
		call SimpleButtonMouseEventHandler;
		add esp, 8;
		pop ecx;//恢复
		jmp SimpleButtonVtable0x50;
	}
}

void __declspec(naked) SimpleButtonMouseUpEventHook() {
	__asm {
		push ecx;//保护
		mov eax, [esp + 8];
		push eax;
		mov eax, ecx;
		push eax;
		call SimpleButtonMouseEventHandler;
		add esp, 8;
		pop ecx;//恢复
		jmp SimpleButtonVtable0x54;
	}
}

void SimpleButtonHook(war3::CSimpleButton* button) {
	memcpy_s(FakeSimpleButtonVtableData, sizeof(FakeSimpleButtonVtableData), dreamaero::pointer_calc<void*>(VTBL(button), -4), sizeof(FakeSimpleButtonVtableData));
	button->baseSimpleFrame.baseLayoutFrame.vtable = FakeSimpleButtonVtable;

	SimpleButtonVtable0x50 = FakeSimpleButtonVtable[0x50 / sizeof(void*)];
	FakeSimpleButtonVtable[0x50 / sizeof(void*)] = SimpleButtonMouseDownEventHook;

	SimpleButtonVtable0x54 = FakeSimpleButtonVtable[0x54 / sizeof(void*)];
	FakeSimpleButtonVtable[0x54 / sizeof(void*)] = SimpleButtonMouseUpEventHook;
}

void GameEventObserver_Init() {
	SafeMode = false;

	HookManager = GetObjectHookManager();
	void* data = dreamaero::offset_element_get<void*>(GameTlsDataGet(0xD), 0x10);
	War3NetData = dreamaero::offset_element_get<war3::CNetData*>(data, 0x8);
	War3NetObserver = dreamaero::pointer_calc<war3::CNetObserver*>(War3NetData, 0x8);
	War3NetEventObserver = &(War3NetObserver->observer_4);

	War3InputObserver = InputObserverGet();





	//Hook InputObserver
	HookManager->replace(War3InputObserver, 6);
	OrgInputEventHandler = HookManager->apply(War3InputObserver, 0x10, InputEventHook);

	//Hook NetEventObserver
	HookManager->replace(War3NetEventObserver, 6);
	OrgNetEventHandler = HookManager->apply(War3NetEventObserver, 0x10, NetEventHook);

}

void GameEventObserver_Cleanup() {
	//遍历所有被修改的对象并把vtable复原？
	//否则如果被强行卸载会出错

	//if (IsInGame()) { //说明在游戏内被强行退出
	HookManager->clearAllDynamic();
	Game_Cleanup();
	//}
}