#include "stdafx.h"
#include "Game.h"
#include "Tools.h"
#include "Storm.h"
#include "Offsets.h"
#include <cstdarg>
#include "FunctionTemplate.h"
#include "RCString.h"
#include <WinVer.h>
#pragma comment(lib,"version.lib")

static void* GameGlobalUI;
static void* GameProfileInt;
static void* GameProfileFloat;
static void* GameUnitArrayGet;
static war3::CGameWar3** GameGlobalObject;
static void* GameStateGet;
static void* GameObjectFromHash;
static void* GameHandleFromObject;
static void* GameTimeGet;
static void* GameUnitGetObject;
static void* GameItemGetObject;
static void* GameAbilityFromUnit;
static void* GameUIToggle;
static void* GameUnitSelectionPrioGet;
static void* GameSoundPlay;
static void* DefaultUnitTagGet;
static war3::MapInfo* GameMapInfo;
static war3::CGxDevice** CGxDevice;
static void* GlobalGlueMgr;
static void* InputObserverGetCall;
static void* PlayerNameGetCall;
static void* ObjectNameGetCall;
static void* GetIcon;
static void* GetIconObj;
//static void *AbilitySpellCheck;
static void* AgentTimerVTable;
static void* PlayerTechtreeGet;
static void** AgileData;
static void** CCommandButtonVtable;
static void* AbilityDefDataGetByAID;
static void* AbilityUIDefGetByAID;
static void* SelectionSubgroupSetFunc;
static void* SelectionSyncFunc;
static void* NativeFuncGet;
static void* NativeFuncCreate;
static void* JassStringRvToRCString;
static void* UnitDataHashTable;
static void* UnitDataNodeGet;
static void* ItemDataHashTable;
static void* ItemDataNodeGet;

typedef void(__fastcall* Prototype_GameUITextDisplay)(void* pthis, unsigned int dummy, float x, float y, const char* buffer, float duration, int unknown);
Prototype_GameUITextDisplay GameUITextDisplay;
void* GameMissileSpeedGet;

//------------------------------------------------
// implementation

war3::CGameUI* PrepareGameUI()
{
	war3::CGameUI* rv;
	__asm {
		mov ecx, 1;
		mov edx, 0;
		call GameUIToggle;
		mov rv, eax;
	}
	return rv;
}

//输出到屏幕
int OutputScreen(float duration, const char* format, ...) {
	if (!IsInGame() || !GameGlobalUI) return 0;

	char buffer[1024];
	int rv;
	va_list args;
	va_start(args, format);
	rv = vsprintf_s(buffer, 1024, format, args);
	va_end(args);

	PrepareGameUI();

	GameUITextDisplay(*((void**)GameGlobalUI), 0, 0.0, 0.0, buffer, duration, -1);

	return rv;
}

void PingMinimapEx(float x, float y, float duration, int red, int green, int blue, bool extraEffect)
{
	war3::CGameUI* GlobalUI = PrepareGameUI();
	int color = blue | ((green | ((red | 0xFFFFFF00) << 8)) << 8);
	int extra = extraEffect ? 1 : 0;

	void* func = Offset(MINIMAP_PING);
	dreamaero::generic_this_call<void>(func, GlobalUI->minimap, x, y, 0.f, duration, &color, extra);
}

unsigned long VersionGet(LPCSTR fileName, unsigned int section) {
	unsigned long version = 0;
	unsigned int infoSize = GetFileVersionInfoSizeA(fileName, NULL);
	if (infoSize) {
		char* buffer = (char*)malloc(infoSize + 1);
		if (GetFileVersionInfoA(fileName, NULL, infoSize, buffer)) {
			VS_FIXEDFILEINFO* pFixedInfo;	unsigned int infoLength;
			if (VerQueryValueA(buffer, "\\", reinterpret_cast<LPVOID*>(&pFixedInfo), &infoLength)) {
				switch (section) {
				case 1: {version = (pFixedInfo->dwFileVersionMS >> 0x10); break; }
				case 2: {version = (pFixedInfo->dwFileVersionMS & 0xFFFF); break; }
				case 3: {version = (pFixedInfo->dwFileVersionLS >> 0x10); break; }
				case 4: {version = (pFixedInfo->dwFileVersionLS & 0xFFFF); break; }
				default: {break; }
				}
			}
		}
		free(buffer);
	}
	return version;
}

unsigned long GameCodepage() {
	//  English (US)            = 0x00000409
	//  English (UK)            = 0x00000809
	//  French                  = 0x0000040c
	//  German                  = 0x00000407
	//  Spanish                 = 0x0000040a
	//  Italian                 = 0x00000410
	//  Czech                   = 0x00000405
	//  Russian                 = 0x00000419
	//  Polish                  = 0x00000415
	//  Portuguese (Brazilian)  = 0x00000416
	//  Portuguese (Portugal)   = 0x00000816
	//  Turkish                 = 0x0000041f
	//  Japanese                = 0x00000411
	//  Korean                  = 0x00000412
	//  Chinese (Traditional)   = 0x00000404
	//  Chinese (Simplified)    = 0x00000804
	//  Thai                    = 0x0000041e
	unsigned short langid = DreamStorm::FileGetLocale();
	switch (langid) {
	case 0x00000409:
		return 0;
	case 0x00000809:
		return 0;
	case 0x0000040c:
		return 0;
	case 0x00000407:
		return 0;
	case 0x0000040a:
		return 0;
	case 0x00000410:
		return 0;
	case 0x00000405:
		return 0;
	case 0x00000419:
		return 0;
	case 0x00000415:
		return 0;
	case 0x00000416:
		return 0;
	case 0x00000816:
		return 0;
	case 0x0000041f:
		return 0;
	case 0x00000411:
		return 0;
	case 0x00000412:
		return 0;
	case 0x00000404:
		return 0;
	case 0x00000804:
		return 936;
	case 0x0000041e:
		return 0;
	default:
		return 0;
	}
}

//'hfoo' --> "hfoo"
char IntegerIdToCharBuffer[5] = { 0 };
const char* IntegerIdToChar(unsigned int id) {
	unsigned int remainingValue = id;
	for (unsigned int byteno = 0; byteno < 4; byteno++) {
		IntegerIdToCharBuffer[3 - byteno] = remainingValue % 256;
		remainingValue /= 256;
	}
	return IntegerIdToCharBuffer;
}

const char* ObjectIdToNameChar(unsigned int typeId) {
	if (!typeId) return "";
	char* rv;
	__asm {
		mov ecx, typeId;
		xor edx, edx;
		call ObjectNameGetCall;
		mov rv, eax;
	}
	return rv;
}

unsigned int TimeGet() {
	unsigned int rv;
	__asm {
		call GameTimeGet;
		mov rv, eax
	}
	return rv;
}

//产生随机值
float RandomFloat(float lowerBound, float upperBound) {
	if (lowerBound >= upperBound) return lowerBound;
	return lowerBound + (upperBound - lowerBound) * (float)rand() / (float)RAND_MAX;
}

//读取游戏环境文本文件常数 - float
float GameDataProfileGetFloat(LPCSTR appName, LPCSTR keyName, unsigned int index) {
	float rv = 0;
	__asm {
		push index;
		push keyName;
		mov edx, appName;
		lea ecx, rv;
		call GameProfileFloat;
	}
	return rv;
}

//读取游戏环境文本文件常数 - int
int GameDataProfileGetInt(LPCSTR appName, LPCSTR keyName, unsigned int index) {
	int rv = 0;
	__asm {
		push index;
		mov edx, keyName;
		mov ecx, appName;
		call GameProfileInt;
		mov rv, eax;
	}
	return rv;
}

//<底层>获取所有单位
void** UnitGrabAll(unsigned int& count) {
	if (!GameGlobalUI) return NULL;
	void** pArray;
	unsigned int ucount;
	__asm {
		mov eax, GameGlobalUI;
		mov eax, [eax];
		mov ecx, [eax + 0x3BC];
		push 1;
		call GameUnitArrayGet;
		mov ecx, [eax + 4];
		mov ucount, ecx;
		mov ecx, [eax + 8];
		mov pArray, ecx;
	}
	count = ucount;
	return pArray;
}

//<底层>转换hash group到对象
void* ObjectFromHashOffset(war3::HashGroup* hash, int offsetInBytes) {
	if (!hash) return NULL;
	int off1 = offsetInBytes;
	unsigned int val1 = hash->hashA;
	unsigned int val2 = hash->hashB;
	if (!val1 || val1 == 0xFFFFFFFF) { return 0; }
	void* obj = 0;
	__asm {
		mov ecx, val1;
		mov edx, val2;
		call GameObjectFromHash;
		test eax, eax;
		je quit;
		add eax, off1;
		mov eax, [eax];
		mov obj, eax;
	quit:
	}
	return obj;
}

void* AgentFromHash(war3::HashGroup* hash) {
	void* obj = ObjectFromHash(hash);
	if (!obj || *(unsigned int*)((unsigned int)obj + 0x20))
		return NULL;
	else
		return *(void**)((unsigned int)obj + 0x54);
}

war3::CUnit* CUnitFromHash(war3::HashGroup* hash) {
	if (!hash) return NULL;
	war3::CAgent* agent = (war3::CAgent*)AgentFromHash(hash);
	return (agent && AgentTypeIdGet(agent) == '+w3u') ? (war3::CUnit*)agent : NULL;
}

war3::CItem* CItemFromHash(war3::HashGroup* hash) {
	if (!hash) return NULL;
	war3::CAgent* agent = (war3::CAgent*)AgentFromHash(hash);
	return (agent && AgentTypeIdGet(agent) == 'item') ? (war3::CItem*)agent : NULL;
}

void* ObjectFromHash(war3::HashGroup* hash) {
	if (!hash) return NULL;
	unsigned int val1 = hash->hashA;
	unsigned int val2 = hash->hashB;
	void* obj = 0;
	__asm {
		mov ecx, val1;
		mov edx, val2;
		call GameObjectFromHash;
		mov obj, eax
	}
	return obj;
}

//<底层>转换游戏内对象到jass handle
unsigned int ObjectToHandle(void* pObj) {
	if (!pObj) return 0;
	unsigned int result;
	__asm {
		mov ecx, GameGlobalObject
		mov ecx, [ecx]
		call GameStateGet
		mov ecx, eax
		push 0
		push pObj
		call GameHandleFromObject
		mov result, eax
	}
	return result;
}

//<底层>转换jass unit handle到对象
war3::CUnit* UnitGetObject(unsigned int handleId) {
	return handleId ?
		dreamaero::generic_fast_call<war3::CUnit*>(
			GameUnitGetObject,
			handleId
			) : NULL;
}

//<底层>转换jass item handle到对象
war3::CItem* ItemGetObject(unsigned int handleId) {
	return handleId ?
		dreamaero::generic_fast_call<war3::CItem*>(
			GameItemGetObject,
			handleId
			) : NULL;
}

//<底层>获得玩家所选单位, playerslot 0 ~ 15, syncOnly为真表示获取同步选择, 否则获取本地选择
//返回数量
unsigned int SelectedUnitGet(int playerSlot, void* unitArray[], bool syncOnly) {
	war3::CSelectionWar3* selection = (*GameGlobalObject)->players[playerSlot]->selection;
	war3::UnitList* unitList = syncOnly ? &(selection->unitSelectedSync) : &(selection->unitSelectedLocal);

	unsigned int count = 0;
	war3::UnitListNode* node = unitList->firstNode;
	while ((count < unitList->nodeCount) && node) {
		unitArray[count++] = node->unit;
		node = node->nextNode;
	}
	return count;
}

//<底层>获得玩家所选活跃子组, playerslot 0 ~ 15
unsigned int ActiveSubgroupGet(int playerSlot, void* unitArray[]) {
	if (!GameGlobalObject || !(*GameGlobalObject)) return 0;
	war3::CSelectionWar3* selection = (*GameGlobalObject)->players[playerSlot]->selection;
	war3::UnitList* subgroup = selection->activeSubgroup;
	unsigned int count = 0;
	if (subgroup) {
		war3::UnitListNode* node = subgroup->firstNode;
		while ((count < subgroup->nodeCount) && node) {
			unitArray[count++] = node->unit;
			node = node->nextNode;
		}
	}
	return count;
}

//<底层>获得GameState
war3::CGameState* GameStateObjectGet() {
	war3::CGameState* rv;
	__asm {
		mov ecx, GameGlobalObject
		mov ecx, [ecx]
		call GameStateGet
		mov rv, eax
	}
	return rv;
}

//<底层>获得地图信息
war3::MapInfo* MapInfoGet() {
	return GameMapInfo;
}



struct TypeDescriptor {
	unsigned long pVFTable;
	unsigned long spare;
	unsigned long unk0x8;
	const char* name;
};
struct RTTICompleteObjectLocator {
	unsigned long signature;
	unsigned long offset;
	unsigned long cdOffset;
	TypeDescriptor* pTypeDescriptor;
	unsigned long pClassHierarchyDescriptor;
};
//<底层>获取RTTI类名
const char* RTTIClassNameGet(void* object) {
	__try {
		void*** ppp = (void***)object;
		//VTable
		void** pp = *ppp;

		//VTable - 0x4
		pp = (void**)((unsigned int)pp - (unsigned int)0x4);
		void* p = *pp;
		RTTICompleteObjectLocator* p1 = (RTTICompleteObjectLocator*)p;
		TypeDescriptor* p2 = (TypeDescriptor*)p1->pTypeDescriptor;
		return (const char*)((unsigned int)p2 + (unsigned int)0xC);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return NULL;
	}
}

//<底层>尝试Dump Object
unsigned int TryDumpObject(void* object, uint8_t* buffer, unsigned int size) {
	unsigned int success_size = 0;
	__try {
		for (unsigned int i = 0; i < size; ++i) {
			buffer[i] = ((uint8_t*)object)[i];
			++success_size;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return success_size;
	}
	return success_size;
}

//播放音效(slk中字符串)
void SoundPlay(const char* soundlink, war3::CUnit* unit, unsigned int mode) {
	//OutputScreen(10,"soundplay");
	if (!soundlink) return;
	__asm {
		push 0;//?
		push 0;//unit;//?
		push 0;//?
		push 0;//?
		push 0;//?
		push 0;
		//lea ecx, mode; //
		//push ecx;//
		mov edx, soundlink;
		xor ecx, ecx//mov ecx, unit;
			call GameSoundPlay;
	}
}

//获得CGxDevice
war3::CGxDevice* GxDeviceGet() {
	return *CGxDevice;
}

//<底层>获得GameUI
war3::CGameUI* GameUIObjectGet() {
	return *reinterpret_cast<war3::CGameUI**>(GameGlobalUI);
}

//<底层>获得GlueMgr
war3::CGlueMgr* GlueMgrObjectGet() {
	return *reinterpret_cast<war3::CGlueMgr**>(GlobalGlueMgr);
}

//获得默认的UnitTip
war3::CUnitTip* DefaultUnitTipGet() {
	war3::CUnitTip* rv = NULL;
	__asm {
		call DefaultUnitTagGet;
		mov rv, eax;
	}
	return rv;
}

//获取线程数据
unsigned long* GlobalGameTlsIndex;
void* GameTlsDataGet(unsigned int index) {
	unsigned long tlsIndex = *GlobalGameTlsIndex;
	void* tlsValue = TlsGetValue(tlsIndex);
	if (tlsValue)
		return dreamaero::offset_element_get<void*>(tlsValue, index * 4);
	else
		return NULL;
}

unsigned long GameTlsIndexGet() {
	return *GlobalGameTlsIndex;
}

//获取PreselectUIData
void* GlobalPreselectUIData;
war3::PreselectUIData* PreselectUIDataGet() {
	return reinterpret_cast<war3::PreselectUIData*>(GlobalPreselectUIData);
}

//获取当前鼠标下的对象
void* ObjectUnderCursor;
void* ObjectUnderCursorGet() {
	return *reinterpret_cast<void**>(ObjectUnderCursor);
}

//获取InputObserver
war3::CObserver* InputObserverGet() {
	return dreamaero::generic_std_call<war3::CObserver*>(
		InputObserverGetCall
		);
}

//获取GlueMgr菜单HashTable
war3::TSHashTable_UFRAMENAMECREATEHANDLER* FrameNameHashTableGet() {
	return (war3::TSHashTable_UFRAMENAMECREATEHANDLER*)Offset(GLOBAL_FRAMENAMEHASHTABLE);
}

//获取已经存在的菜单HashNode
war3::FrameNameHashNode* FrameNameHashNodeGet(const char* name) {
	return dreamaero::generic_this_call<war3::FrameNameHashNode*>(
		Offset(FRAMENAMEHASHTABLE_GETHASHNODE),
		FrameNameHashTableGet(),
		name
		);
}

//获取录像运行模式
unsigned int ReplayState() {
	void* data = dreamaero::offset_element_get<void*>(GameTlsDataGet(0xD), 0x10);
	war3::CNetData* War3NetData = dreamaero::offset_element_get<war3::CNetData*>(data, 0x8);
	return War3NetData->replayState;
}

//玩家名字获取
const char* PlayerNameGet(int playerId, unsigned int arg) {
	return dreamaero::generic_fast_call<const char*>(
		PlayerNameGetCall,
		playerId,
		arg
		);
}

//获取Agent类别ID
inline unsigned int AgentTypeIdGet(war3::CAgent* agent)
{
	return dreamaero::generic_this_call<unsigned int>(
		VTBL(agent)[0x1C / sizeof(void*)],
		agent
		);
}


//判断是否agent
bool ObjectIsAgent(void* obj)
{
	if (obj)
	{
		unsigned long* hashObj = (unsigned long*)ObjectFromHash((war3::HashGroup*)(((unsigned long*)obj) + 0x3)); //hashgroup
		if (hashObj)
		{
			//OutputScreen(10, "obj type = %s", IntegerIdToChar(*(hashObj + 3)) );
			return *(hashObj + 3) == '+agl';
		}
	}
	return false;
}


//FramePoint
unsigned int GetUnitFramePoint(war3::CUnit* unit, FramePoint* point) {
	if (!unit || !point) return 0;//TODO 验证是否会bug
	float unk = 0.0;	//TODO 这是什么
	return dreamaero::generic_fast_call<unsigned int>(Offset(UNIT_FRAMEPOSITION_GET), unit, point, &unk);
}

char* GetIconFromFuncProfile(unsigned int typeId, unsigned int index) {
	if (!typeId) return NULL;
	char* rv;
	const char* typeIdStr = IntegerIdToChar(typeId);
	rv = dreamaero::generic_this_call<char*>(GetIcon, *(unsigned long*)GetIconObj, typeIdStr, "Art", 0, index);
	return rv;
}

char* GetBuffIconFromFuncProfile(unsigned int typeId, unsigned int index) {
	if (!typeId) return NULL;
	char* rv;
	const char* typeIdStr = IntegerIdToChar(typeId);
	rv = dreamaero::generic_this_call<char*>(GetIcon, *(unsigned long*)GetIconObj, typeIdStr, "Buffart", 0, index);
	return rv;
}

void GetIconPos(unsigned int typeId, unsigned int& x, unsigned int& y)//FIXME
{
	if (!typeId) return;
	const char* typeIdStr = IntegerIdToChar(typeId);
	char* posXstr = dreamaero::generic_this_call<char*>(GetIcon, *(unsigned long*)GetIconObj, typeIdStr, "Buttonpos", 0, 0);
	char* posYstr = dreamaero::generic_this_call<char*>(GetIcon, *(unsigned long*)GetIconObj, typeIdStr, "Buttonpos", 0, 1);
	x = posXstr ? atoi(posXstr) : 0;
	y = posYstr ? atoi(posYstr) : 0;
}

unsigned int GetAgentAgileDataHashKey(int agent_id) {
	return dreamaero::generic_fast_call<unsigned int>(Offset(AGILEDATA_HASHKEY_GET), &agent_id);
}

war3::AGILE_TYPE_DATA* GetAgentAgileTypeData(int agent_id) {
	war3::AGILE_TYPE_DATA* rv = NULL;
	void* hash_table = dreamaero::pointer_calc<void*>(*AgileData, 0xC);
	if (hash_table) {
		unsigned int hash_key = GetAgentAgileDataHashKey(agent_id);
		rv = dreamaero::generic_this_call<war3::AGILE_TYPE_DATA*>(Offset(AGILETYPEDATA_NODE_GET), hash_table, hash_key, &agent_id);
	}
	return rv;
}

static std::map<std::pair<int, int>, bool> AgentAncestorData;
bool AgentHasAncestor(int agent_id, int ancestor_id) {
	if (agent_id == ancestor_id)
		return true;

	std::pair<int, int> dataKey(agent_id, ancestor_id);

	if (AgentAncestorData.count(dataKey))
	{
		return AgentAncestorData[dataKey];
	}

	war3::AGILE_TYPE_DATA* data = NULL;
	while (true) {
		data = GetAgentAgileTypeData(agent_id);
		if (data && agent_id != data->parent_id) {
			if (data->parent_id == ancestor_id)
			{
				AgentAncestorData[dataKey] = true;
				return true;
			}
			agent_id = data->parent_id;
		}
		else
			break;
	}
	AgentAncestorData[dataKey] = false;
	return false;
}

void DumpAgentAncestors(int agent_id) {
#ifndef _VMP
	;//OutputDebug("================================================================================\n");
	;//OutputDebug(" Ancestors of %s:\n", IntegerIdToChar(agent_id));
	;//OutputDebug("--------------------------------------------------------------------------------\n");
	int t = agent_id;
	war3::AGILE_TYPE_DATA* data = NULL;
	while (true) {
		data = GetAgentAgileTypeData(t);
		if (data && t != data->parent_id) {
			;//OutputDebug(" %s\n", IntegerIdToChar(data->parent_id));
			t = data->parent_id;
		}
		else
			break;
	}
	;//OutputDebug("================================================================================\n");
#endif
}

bool IsCommandButton(war3::CSimpleButton* btn) {
	return VTBL(btn) == CCommandButtonVtable;
}

//参考 6F02E640
war3::AbilityDefData* AbilityDefDataGet(int ability_id) {
	return dreamaero::generic_fast_call<war3::AbilityDefData*>(AbilityDefDataGetByAID, ability_id);
}

war3::AbilityDefData* AbilityDefDataGet(war3::CAbility* ability) {
	war3::AbilityDefData* rv = NULL;
	if (ability) {
		if (ability->defData) {
			rv = ability->defData;
		}
		else {
			rv = AbilityDefDataGet(ability->id);
		}
	}
	return rv;
}


void Tools_Init() {
	AgentAncestorData.clear();
	GameGlobalUI = Offset(GLOBAL_UI);
	GameUnitArrayGet = Offset(UNIT_ARRAY_GET);
	GameGlobalObject = (war3::CGameWar3**)(Offset(GLOBAL_WARCRAFT_GAME));
	GameStateGet = Offset(STATE_GET);
	GameObjectFromHash = Offset(OBJECT_FROM_HASH);
	GameHandleFromObject = Offset(HANDLE_FROM_OBJECT);
	GameTimeGet = Offset(GAME_TIME_ELAPSED_GET);
	GameUnitGetObject = Offset(UNIT_FROM_HANDLE);
	GameItemGetObject = Offset(ITEM_FROM_HANDLE);
	GameAbilityFromUnit = Offset(ABILITY_FROM_UNIT);
	GameUIToggle = Offset(UI_TOGGLE);
	GameUITextDisplay = (Prototype_GameUITextDisplay)Offset(UI_TEXT_DISPLAY);
	GameMissileSpeedGet = Offset(MISSILE_SPEED_GET);
	GameMapInfo = reinterpret_cast<war3::MapInfo*>(Offset(GLOBAL_MAP_INFO));
	GameProfileInt = Offset(PROFILE_GET_INT);
	GameProfileFloat = Offset(PROFILE_GET_FLOAT);
	GameUnitSelectionPrioGet = Offset(SELECTION_PRIO_GET);
	GameSoundPlay = Offset(SOUND_PLAY);
	CGxDevice = reinterpret_cast<war3::CGxDevice**>(Offset(GLOBAL_CGXDEVICE));
	DefaultUnitTagGet = Offset(WAR3_UNITTIP_GET);
	GlobalGlueMgr = Offset(GLOBAL_GLUEMGR);
	InputObserverGetCall = Offset(INPUTOBSERVER_GET);
	GlobalPreselectUIData = Offset(GLOBAL_PRESELECTUIDATA);
	ObjectUnderCursor = Offset(GLOBAL_OBJECT_UNDER_CURSOR);
	PlayerNameGetCall = Offset(PLAYER_NAME_GET);
	ObjectNameGetCall = Offset(OBJECT_NAME_GET);
	GlobalGameTlsIndex = reinterpret_cast<unsigned long*>(Offset(GLOBAL_TLS_INDEX));
	GetIcon = Offset(READFUNCPROFILE);
	GetIconObj = Offset(FUNCPROFILEOBJ);
	//AbilitySpellCheck = Offset(ABILITY_SPELL_CHECK);
	AgentTimerVTable = Offset(AGENTTIMER_VTABLE);
	PlayerTechtreeGet = Offset(PLAYER_TECHTREE_GET);
	AgileData = (void**)Offset(GLOBAL_AGILE_DATA);
	CCommandButtonVtable = (void**)Offset(COMMANDBUTTON_VTABLE);
	AbilityDefDataGetByAID = Offset(ABILITYDEFDATA_GET);
	AbilityUIDefGetByAID = Offset(ABILITYUIDEF_GET);
	SelectionSubgroupSetFunc = Offset(SELECTION_SUBGROUP_SET);
	SelectionSyncFunc = Offset(SELECTION_SYNC);
	NativeFuncGet = Offset(NATIVEFUNC_NODE_GET);
	NativeFuncCreate = Offset(NATIVEFUNC_NODE_CREATE);
	JassStringRvToRCString = Offset(JASS_STRING_RV_TO_RCSTRING);
	UnitDataHashTable = Offset(GLOBAL_UNITDATAHASHTABLE);
	UnitDataNodeGet = Offset(UNITDATA_NODE_GET);
	ItemDataHashTable = Offset(GLOBAL_ITEMDATAHASHTABLE);
	ItemDataNodeGet = Offset(ITEMDATA_NODE_GET);
}

