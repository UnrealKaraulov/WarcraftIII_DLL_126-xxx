#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include "BaseStructs.h"
#include "GameStructs.h"
#include "UIStructs.h"

#ifndef TOOLS_H_INCLUDED_
#define TOOLS_H_INCLUDED_

//初始化偏移等
void Tools_Init();

//输出到屏幕
int OutputScreen(float duration, const char* format, ...);

//文件版本，section取值1到4，例如对于版本1.24.4.6387，section 2的值是24
unsigned long VersionGet(LPCSTR fileName, unsigned int section);

//获取游戏codepage
unsigned long GameCodepage();

//读取当前游戏时间
unsigned int TimeGet();

//'hfoo' --> "hfoo"
const char* IntegerIdToChar(unsigned int id);

//'hfoo' --> "footman"
const char* ObjectIdToNameChar(unsigned int typeId);

//hash
template<typename ValueType>
void HashCombine(unsigned int* seed, ValueType value) {
	unsigned int t = *(reinterpret_cast<unsigned int*>((&value)));
	*seed = (*seed) ^ (t + 0x9e3779b9 + ((*seed) << 6) + ((*seed) >> 2));
}

//产生随机值
float RandomFloat(float lowerBound, float upperBound);

//读取游戏环境文本文件常数 - float
float GameDataProfileGetFloat(LPCSTR appName, LPCSTR keyName, unsigned int index);

//读取游戏环境文本文件常数 - int
int GameDataProfileGetInt(LPCSTR appName, LPCSTR keyName, unsigned int index);

//<底层>获取所有单位
//输出参数为单位指针的数组指针, 返回值为单位个数
void** UnitGrabAll(unsigned int& count);

//<底层>判断对象是否agent
bool ObjectIsAgent(void* obj);

//<底层>转换hash group到对象
void* ObjectFromHash(war3::HashGroup* hash);
void* ObjectFromHashOffset(war3::HashGroup* hash, int offsetInBytes);
void* AgentFromHash(war3::HashGroup* hash);
war3::CUnit* CUnitFromHash(war3::HashGroup* hash);
war3::CItem* CItemFromHash(war3::HashGroup* hash);
void* ObjectFromHash(war3::HashGroup* hash);

//<底层>转换jass handle与对象
unsigned int ObjectToHandle(void* pObj);//转为jass handle
war3::CUnit* UnitGetObject(unsigned int handleId);
war3::CItem* ItemGetObject(unsigned int handleId);

template <typename FieldType>
FieldType ObjectFieldGet(void* object, int offsetInBytes) {
	return *(reinterpret_cast<FieldType*>(reinterpret_cast<uint8_t*>(object) + offsetInBytes));
}

//<底层>获得玩家所选单位, playerslot 0 ~ 15, syncOnly为真表示获取同步选择, 否则获取本地选择 TODO这个说法似乎不正确
//返回数量
unsigned int SelectedUnitGet(int playerSlot, void* unitArray[], bool syncOnly);

//<底层>获得玩家所选活跃子组, playerslot 0 ~ 15
unsigned int ActiveSubgroupGet(int playerSlot, void* unitArray[]);

//<底层>设置玩家所选活跃子组
void ActiveSubgroupSet(war3::CUnit* targetUnit);

//<底层>获得单位选择优先级
float UnitSelectionPrioGet(war3::CUnit* lpUnit);

//<底层>获得单位技能对象
war3::CAbility* AbilityObjGet(war3::CUnit* lpUnit, unsigned int abilityTypeId);

//<底层>获得单位攻击技能对象
war3::CAbilityAttack* AttackAbilityObjGet(unsigned int unitHandleId);

//<底层>获得计时器剩余时间
float AgentTimerRemainingTimeGet(war3::CAgentTimer* pAgentTimer);

//<底层>获得子弹速度
float UnitTypeMissileSpeedGet(unsigned int unitTypeId, int weapon);

//获得子弹位置
bool BulletPositionGet(war3::ProjectilePosition* bullet, float& x, float& y, float& z);



//<底层>获得CGameWar3
//war3::CGameWar3* GameObjectGet();

//<底层>获得GameState
war3::CGameState* GameStateObjectGet();

//<底层>获得GameUI
war3::CGameUI* GameUIObjectGet();

war3::CGameUI* PrepareGameUI();

//<底层>获得GlueMgr
war3::CGlueMgr* GlueMgrObjectGet();

//<底层>获得地图信息
war3::MapInfo* MapInfoGet();

//<底层>获取RTTI类名
const char* RTTIClassNameGet(void* object);

//<底层>尝试Dump Object
unsigned int TryDumpObject(void* object, uint8_t* buffer, unsigned int size);

//播放音效(slk中字符串)
void SoundPlay(const char* soundlink, war3::CUnit* unit, unsigned int mode);

//获得CGxDevice
war3::CGxDevice* GxDeviceGet();

//获得默认的UnitTip
war3::CUnitTip* DefaultUnitTipGet();

//获取线程数据
void* GameTlsDataGet(unsigned int index);
unsigned long GameTlsIndexGet();

//获取InputObserver
war3::CObserver* InputObserverGet();

//获取PreselectUIData
war3::PreselectUIData* PreselectUIDataGet();

//获取当前鼠标下的对象
void* ObjectUnderCursorGet();

//获取GlueMgr菜单HashTable
war3::TSHashTable_UFRAMENAMECREATEHANDLER* FrameNameHashTableGet();

//获取已经存在的菜单HashNode
war3::FrameNameHashNode* FrameNameHashNodeGet(const char* name);

//获取录像运行状态 正常游戏为0 正常录像播放为2
const unsigned int REPLAY_STATE_NONE = 0;
const unsigned int REPLAY_STATE_LOADED = 1;
const unsigned int REPLAY_STATE_STREAMINGOUT = 2;
const unsigned int REPLAY_STATE_STREAMINGIN = 3;
const unsigned int REPLAY_STATE_STOPPED = 4;
const unsigned int REPLAY_STATE_UNKNOWN = 5;
unsigned int ReplayState();

//获取玩家名
const char* PlayerNameGet(int playerId, unsigned int arg = 0);

//获取Agent类别ID
inline unsigned int AgentTypeIdGet(war3::CAgent* agent);

//FramePoint
struct FramePoint {
	float x;
	float y;
};

unsigned int GetUnitFramePoint(war3::CUnit* unit, FramePoint* point);

unsigned int GetAgentAgileDataHashKey(int agent_id);
war3::AGILE_TYPE_DATA* GetAgentAgileTypeData(int agent_id);
bool AgentHasAncestor(int agent_id, int ancestor_id);
void DumpAgentAncestors(int agent_id);

char* GetIconFromFuncProfile(unsigned int typeId, unsigned int index = 0);
char* GetBuffIconFromFuncProfile(unsigned int typeId, unsigned int index = 0);

bool IsAbilitySpell(war3::CAbility*);
bool IsAbilityBuff(war3::CAbility* ability);
float AbilityManaCostGet(war3::CAbility* ability);
float AbilityCooldownGet(war3::CAbility* ability);
void GetIconPos(unsigned int typeId, unsigned int& row, unsigned int& column);
bool IsTechtreeAvailable(int playerId, int techId);

bool IsCommandButton(war3::CSimpleButton* btn);

void PingMinimapEx(float x, float y, float duration, int red, int green, int blue, bool extraEffect);

//最好不要直接读取下面两个结构中的值。因为逆向是基于CAbilitySpell的虚函数的。
war3::AbilityDefData* AbilityDefDataGet(int ability_id);
war3::AbilityDefData* AbilityDefDataGet(war3::CAbility* ability);
war3::AbilityUIDef* AbilityUIDefGet(int ability_id);
war3::AbilityUIDef* AbilityUIDefGet(war3::CAbility* ability);

war3::NativeFunc* CreateNativeFuncNode(const char* func);
war3::NativeFunc* GetNativeFuncNode(const char* func);

war3::UnitDataNode* GetUnitDataNode(unsigned int type_id);
war3::ItemDataNode* GetItemDataNode(unsigned int type_id);

const char* GetJassReturedString(unsigned int id);

#endif