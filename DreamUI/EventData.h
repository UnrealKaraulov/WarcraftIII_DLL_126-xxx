#ifndef EVENT_DATA_H_INCLUDED_
#define EVENT_DATA_H_INCLUDED_

#include "GameStructs.h"
#include "GameStructs.h"
#include "RCString.h"



//Custom Events
const unsigned int EVENT_GAME_START = 1000;
const unsigned int EVENT_GAME_END = 1001;
const unsigned int EVENT_KEY_UP = 1002;
const unsigned int EVENT_KEY_DOWN = 1003;
const unsigned int EVENT_MOUSE_MOVE = 1004;
const unsigned int EVENT_MOUSE_UP = 1005;
const unsigned int EVENT_MOUSE_DOWN = 1006;
const unsigned int EVENT_MOUSE_SCROLL = 1007;
const unsigned int EVENT_ITEM_CLICK = 1008;
const unsigned int EVENT_PACKET = 1009;
const unsigned int EVENT_CONTROL_GROUP_ASSIGN = 1010;
const unsigned int EVENT_CONTROL_GROUP_SELECT = 1011;
const unsigned int EVENT_UNIT_ATTACK_MISS = 1012;
const unsigned int EVENT_UNIT_RECEIVE_DAMAGE = 1013;
const unsigned int EVENT_UNIT_CREATED = 1014;
const unsigned int EVENT_LOCAL_ACTION = 1015;
const unsigned int EVENT_LOCAL_CHAT = 1016;
const unsigned int EVENT_ITEM_CREATED = 1017;
const unsigned int EVENT_UNIT_ATTACK_RELEASED = 1018;
const unsigned int EVENT_UNIT_ACQUIRE_START = 1019;
const unsigned int EVENT_UNIT_ACQUIRE_READY = 1020;
const unsigned int EVENT_UNIT_ACQUIRE_STOP = 1021;
const unsigned int EVENT_FOCUS = 1022;
const unsigned int EVENT_LOCAL_NETEVENT = 1023;//任意本地netevent

//LastHit
//const unsigned int EVENT_LASTHIT_DATA_READY = 2000;
//const unsigned int EVENT_LASTHIT_UNIT_IDLE = 2001;

//UI
const unsigned int EVENT_CLICK = 3000;
const unsigned int EVENT_MOUSE_OVER = 3001;
const unsigned int EVENT_MOUSE_OUT = 3002;

//define event data structs
struct KeyboardEventData {
	int code;
	int vkcode;
	char chars[4];//废弃
	bool shift;
	bool ctrl;
	bool alt;
	bool byProgramm;
	war3::CKeyEvent* evtObj;

	void discard() { if (evtObj) evtObj->baseEvent.id = 0; }
	void resetCode(int newCode) { code = newCode; evtObj->keyCode = newCode; }
};

//左键=1 中键=2 右键=4 左边靠后的侧键=8 左边靠前的侧键=0x10
namespace MOUSECODE {
	const unsigned int MOUSE_LEFT = (1 << 0);
	const unsigned int MOUSE_MIDDLE = (1 << 1);
	const unsigned int MOUSE_RIGHT = (1 << 2);
}

struct MouseEventData {
	int mouseCode;
	float x;
	float y;
	war3::CSimpleButton* buttonPushed;
};

struct MouseEventScrollData {
	bool up;
	war3::CControlWheelEvent* evtObj;

	void discard() { if (evtObj) evtObj->baseMouseEvent.baseEvent.id = 0; }
};

struct ItemClickData {
	int mouseCode;
	int clickedItem;
	bool byProgramm;
	war3::CSimpleButtonClickEvent* evtObj;
	void discard() { if (evtObj) evtObj->baseEvent.id = 0; }
};

struct ActionEventData {
	war3::PacketSender* packetSender;
	bool byProgramm;
	unsigned int id;
	int type;
	unsigned int flag;
	int usingItem;
	float x;
	float y;
	int target;
	int transferItem;
	void discard() {}
};

struct ControlGroupEventData {
	unsigned int numGroup;
	int reason;
	//TODO: 单位组
	static const int REASON_ASSIGN = 0;
	static const int REASON_SELECT = 1;
	//...
};

struct UnitCreationEventData {
	int createdUnit;
};

struct ItemCreationEventData {
	int createdItem;
};

struct UnitDamagedEventData {
	int target;
	int source;
	bool isSpell;
	float damage;
	float damageRaw;
};

struct LocalChatEventData {
	char* content;
};

struct UnitAttackReleasedEventData {
	int attacker;
};

struct UnitAttackMissedEventData {
	int attacker;
};

struct UnitAcquireEventData {
	int eventUnit;
	int target;
};

#endif