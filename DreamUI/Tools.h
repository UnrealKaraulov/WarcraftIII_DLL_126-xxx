#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include "BaseStructs.h"
#include "GameStructs.h"
#include "UIStructs.h"

#ifndef TOOLS_H_INCLUDED_
#define TOOLS_H_INCLUDED_

//��ʼ��ƫ�Ƶ�
void Tools_Init();

//�������Ļ
int OutputScreen(float duration, const char* format, ...);

//�ļ��汾��sectionȡֵ1��4��������ڰ汾1.24.4.6387��section 2��ֵ��24
unsigned long VersionGet(LPCSTR fileName, unsigned int section);

//��ȡ��Ϸcodepage
unsigned long GameCodepage();

//��ȡ��ǰ��Ϸʱ��
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

//�������ֵ
float RandomFloat(float lowerBound, float upperBound);

//��ȡ��Ϸ�����ı��ļ����� - float
float GameDataProfileGetFloat(LPCSTR appName, LPCSTR keyName, unsigned int index);

//��ȡ��Ϸ�����ı��ļ����� - int
int GameDataProfileGetInt(LPCSTR appName, LPCSTR keyName, unsigned int index);

//<�ײ�>��ȡ���е�λ
//�������Ϊ��λָ�������ָ��, ����ֵΪ��λ����
void** UnitGrabAll(unsigned int& count);

//<�ײ�>�ж϶����Ƿ�agent
bool ObjectIsAgent(void* obj);

//<�ײ�>ת��hash group������
void* ObjectFromHash(war3::HashGroup* hash);
void* ObjectFromHashOffset(war3::HashGroup* hash, int offsetInBytes);
void* AgentFromHash(war3::HashGroup* hash);
war3::CUnit* CUnitFromHash(war3::HashGroup* hash);
war3::CItem* CItemFromHash(war3::HashGroup* hash);
void* ObjectFromHash(war3::HashGroup* hash);

//<�ײ�>ת��jass handle�����
unsigned int ObjectToHandle(void* pObj);//תΪjass handle
war3::CUnit* UnitGetObject(unsigned int handleId);
war3::CItem* ItemGetObject(unsigned int handleId);

template <typename FieldType>
FieldType ObjectFieldGet(void* object, int offsetInBytes) {
	return *(reinterpret_cast<FieldType*>(reinterpret_cast<uint8_t*>(object) + offsetInBytes));
}

//<�ײ�>��������ѡ��λ, playerslot 0 ~ 15, syncOnlyΪ���ʾ��ȡͬ��ѡ��, �����ȡ����ѡ�� TODO���˵���ƺ�����ȷ
//��������
unsigned int SelectedUnitGet(int playerSlot, void* unitArray[], bool syncOnly);

//<�ײ�>��������ѡ��Ծ����, playerslot 0 ~ 15
unsigned int ActiveSubgroupGet(int playerSlot, void* unitArray[]);

//<�ײ�>���������ѡ��Ծ����
void ActiveSubgroupSet(war3::CUnit* targetUnit);

//<�ײ�>��õ�λѡ�����ȼ�
float UnitSelectionPrioGet(war3::CUnit* lpUnit);

//<�ײ�>��õ�λ���ܶ���
war3::CAbility* AbilityObjGet(war3::CUnit* lpUnit, unsigned int abilityTypeId);

//<�ײ�>��õ�λ�������ܶ���
war3::CAbilityAttack* AttackAbilityObjGet(unsigned int unitHandleId);

//<�ײ�>��ü�ʱ��ʣ��ʱ��
float AgentTimerRemainingTimeGet(war3::CAgentTimer* pAgentTimer);

//<�ײ�>����ӵ��ٶ�
float UnitTypeMissileSpeedGet(unsigned int unitTypeId, int weapon);

//����ӵ�λ��
bool BulletPositionGet(war3::ProjectilePosition* bullet, float& x, float& y, float& z);



//<�ײ�>���CGameWar3
//war3::CGameWar3* GameObjectGet();

//<�ײ�>���GameState
war3::CGameState* GameStateObjectGet();

//<�ײ�>���GameUI
war3::CGameUI* GameUIObjectGet();

war3::CGameUI* PrepareGameUI();

//<�ײ�>���GlueMgr
war3::CGlueMgr* GlueMgrObjectGet();

//<�ײ�>��õ�ͼ��Ϣ
war3::MapInfo* MapInfoGet();

//<�ײ�>��ȡRTTI����
const char* RTTIClassNameGet(void* object);

//<�ײ�>����Dump Object
unsigned int TryDumpObject(void* object, uint8_t* buffer, unsigned int size);

//������Ч(slk���ַ���)
void SoundPlay(const char* soundlink, war3::CUnit* unit, unsigned int mode);

//���CGxDevice
war3::CGxDevice* GxDeviceGet();

//���Ĭ�ϵ�UnitTip
war3::CUnitTip* DefaultUnitTipGet();

//��ȡ�߳�����
void* GameTlsDataGet(unsigned int index);
unsigned long GameTlsIndexGet();

//��ȡInputObserver
war3::CObserver* InputObserverGet();

//��ȡPreselectUIData
war3::PreselectUIData* PreselectUIDataGet();

//��ȡ��ǰ����µĶ���
void* ObjectUnderCursorGet();

//��ȡGlueMgr�˵�HashTable
war3::TSHashTable_UFRAMENAMECREATEHANDLER* FrameNameHashTableGet();

//��ȡ�Ѿ����ڵĲ˵�HashNode
war3::FrameNameHashNode* FrameNameHashNodeGet(const char* name);

//��ȡ¼������״̬ ������ϷΪ0 ����¼�񲥷�Ϊ2
const unsigned int REPLAY_STATE_NONE = 0;
const unsigned int REPLAY_STATE_LOADED = 1;
const unsigned int REPLAY_STATE_STREAMINGOUT = 2;
const unsigned int REPLAY_STATE_STREAMINGIN = 3;
const unsigned int REPLAY_STATE_STOPPED = 4;
const unsigned int REPLAY_STATE_UNKNOWN = 5;
unsigned int ReplayState();

//��ȡ�����
const char* PlayerNameGet(int playerId, unsigned int arg = 0);

//��ȡAgent���ID
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

//��ò�Ҫֱ�Ӷ�ȡ���������ṹ�е�ֵ����Ϊ�����ǻ���CAbilitySpell���麯���ġ�
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