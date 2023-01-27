#ifndef GAMESTRUCTS_H_INCLUDED_
#define GAMESTRUCTS_H_INCLUDED_

#include "BaseStructs.h"
#include "UIStructs.h"
#include "StringStructs.h"
namespace war3 {
#ifdef _MSC_VER
#pragma pack(push, 1)
#endif // _MSC_VER

	struct HashGroup;
	struct UnitList;
	struct UnitListNode;

	struct CGameWar3;//TODO 验证class
	struct CPlayerWar3;//TODO 验证class
	struct CSelectionWar3;//TODO 验证class
	struct CGameState;
	struct CUnit;
	struct CItem;
	struct CAgentTimer;
	struct CAbility;//TODO 验证class
	struct CAbilityAttack;
	struct CAbilitySpell;
	struct CPreselectUI;
	struct CStatBar;
	struct CTechTree;

	typedef LRESULT(__fastcall* WAR3WNDPROC)(HWND hwnd, unsigned int uMsg, WPARAM wParam, LPARAM lParam);

	typedef unsigned int HASHKEY_4CC;

	struct UnitDataNode {
		unsigned int	unk_0;			//0x0
		uint8_t		unk_4[0x10];	//0x4
		unsigned int	typeId;			//0x14
		int			buildTime;		//0x18
		unsigned int	unk_1C;			//0x1C
		int			goldCost;		//0x20
		int			woodCost;		//0x24
		uint8_t		unk_28[0x24C];	//0x28
		RCString	abilList;		//0x274
		RCString	str_280;		//0x280
		unsigned int	unk_28C;		//0x28C
		unsigned int	unk_290;		//0x290
		unsigned int	unk_294;		//0x294
		unsigned int	unk_298;		//0x298
		RCString	str_29C;		//0x29C
		RCString	str_2A8;		//0x2A8
		unsigned int	unk_2B4;		//0x2B4
	};//sizeof = 0x2BC
	typedef TAllocatorHashTable<UnitDataNode, HASHKEY_4CC, 0x40> UnitDataAllocatorHashTable;

	struct ItemDataNode {
		uint8_t	unk_0[0xA8];			//0x0
	};//sizeof = 0xA8
	typedef TAllocatorHashTable<ItemDataNode, HASHKEY_4CC, 0x20> ItemDataAllocatorHashTable;

	//TODO 全部调用虚函数，而不是直接修改值。
	struct GxDeviceVTable {
		void* unk0;						//0x0
		void* unk4[8];					//0x4
		HWND(*GetDeviceWindow)(void);	//0x24
		//..
	};

	struct CGxDevice {
		GxDeviceVTable* vtable;
	};

	struct CGxDeviceD3d {
		void** vtable;				//0x0
		uint8_t			unk_4[0x570];		//0x4
		HWND			hwnd;				//0x574
		WAR3WNDPROC		wndProc;			//0x578
		uint8_t			unk_57C[0x144];		//0x57C
		float			unk_6C0;			//0x6C0
		float			unk_6C4;			//0x6C4
		float			windowHeight;		//0x6C8
		float			windowWidth;		//0x6CC
		unsigned int		unk_6D0;			//0x6D0 窗口大小改变，这里设为1
		//..
	};

	struct CGxDeviceOpenGl {
		void** vtable;				//0x0
		uint8_t			unk_4[0x574];		//0x4
		HWND			hwnd;				//0x578
		uint8_t			unk_57C[0x144];		//0x57C
		float			unk_6C0;			//0x6C0
		float			unk_6C4;			//0x6C4
		float			windowHeight;		//0x6C8
		float			windowWidth;		//0x6CC
		unsigned int		unk_6D0;			//0x6D0 窗口大小改变，这里设为1
	};

	struct ColorARGB {
		uint8_t b;
		uint8_t g;
		uint8_t r;
		uint8_t a;
	};

	struct HashGroupCargo {
		HashGroup hash;
		unsigned int unkZero;
	};

	struct UnitListNode {
		unsigned int			unk_00;//UnitListNode **
		UnitListNode* nextNode;	//+0x4
		CUnit* unit;		//+0x8
	}; // sizeof = 0x0C

	struct UnitList {
		void** vtable;
		unsigned int			unkOffset_04;
		UnitListNode* lastNode;	//+0x8
		UnitListNode* firstNode;	//+0xC
		unsigned int			nodeCount;	//+0x10
	}; // sizeof = 0x14


	struct CGameState {
		void** vtable;
		uint8_t					unk_4[0x2C4];	//0x4
		void* asdf;
	};//sizeof?

	struct CGameWar3 {
		void** vtable;				//0x0
		unsigned int		unk_4;				//0x4
		unsigned int		jassStringId;		//0x8
		uint8_t			unk_C[0x1C];		//0xC
		uint16_t		localPlayerSlot;	//0x28
		uint8_t			unk_2A[0x1A];		//0x2A
		unsigned int		maxPlayers;			//+44 - Max player slots on the map.
		uint8_t			unk_48[0x04];
		unsigned int		activePlayers;		//+4C - Active players.
		uint8_t			unk_50[0x08];
		CPlayerWar3* players[16];		//+58
	};//sizeof?

	struct CPlayerWar3 {
		void** vtable;
		uint8_t				unk_04[0x2C];
		uint16_t			slot;			//+30
		uint8_t				unk_32[0x02];
		CSelectionWar3* selection;		//+34
		uint8_t				unk_38[0x228];
		unsigned int			race;			//+260
		unsigned int			color;			//+264
		uint8_t				unk_268[0x10];
		unsigned int			team;			//+278
		uint8_t				unk_27C[0x54];
		CTechTree* techtree;		//+2D0
	};//sizeof?

	struct CSelectionWar3 {
		void** vtable;				//0x0
		uint8_t				unk_04[0x10];
		UnitList			controlGroups[0x14];//0x14	//要搞清楚哪些是本地
		unsigned int			unk_1A4;
		CItem* itemLastSelected;	//0x1A8
		unsigned int			playerId;			//0x1AC
		unsigned int			unk_1B0[5];
		UnitList			unitSelectedSync;	//0x1C4
		CUnit* unit_b1;			//0x1D8
		unsigned int			unit_id;			//0x1DC
		CUnit* activeUnit;			//0x1E0
		CItem* itemSelected;		//0x1E4
		UnitList			unitSelectedLocal;	//0x1E8
		unsigned int			unk_1FC[4];
		UnitList* activeSubgroup;		//0x20C
	};//sizeof?

	struct CAgentTimer {
		void** vtable;			//0x0
		uint8_t unk_4[0x10];
	};//sizeof = 0x14

	struct CAgent {
		void** vtable;
		//...
	};

	struct CWidget {
		void** vtable;
		uint8_t			unk_04[0x8];
		HashGroup		hash;				//0xC
		//...
	};

	struct CUnit_174_30C { //1.24d之前
		uint8_t unk_174[0x64];
		HashGroupRaw				abilityHash;		//0x1DC	//以下偏移为按照1.24d以后
		uint8_t unk_1E4[0x44];
		float						impactZ;			//0x228 Projectile impact Z
		float						impactSwimZ;		//0x22C	Projectile impact Z (swimming)
		float						launchX;			//0x230	Projectile launch X
		float						launchY;			//0x234	Projectile launch Y
		float						launchZ;			//0x238 Projectile launch Z
		float						launchSwimZ;		//0x23C Projectile launch Z (swimming)
		uint8_t unk_240[0x8];
		unsigned int					unitTypeFlag;		//0x248
		uint8_t						unk_24C[0x38];		//0x24C
		float						x;					//0x284
		float						y;					//0x288
		uint8_t						unk_28C[0x48];		//0x28C
		ColorARGB					vertexColor;		//0x2D4	0xAARRGGBB
		uint8_t						unk_2D8[0x38];
	};

	struct CUnit_174_310 { //1.24d和之后
		uint8_t unk_174[0x68];
		HashGroupRaw				abilityHash;		//0x1DC	//以下偏移为按照1.24d以后
		uint8_t unk_1E4[0x44];
		float						impactZ;			//0x228 Projectile impact Z
		float						impactSwimZ;		//0x22C	Projectile impact Z (swimming)
		float						launchX;			//0x230	Projectile launch X
		float						launchY;			//0x234	Projectile launch Y
		float						launchZ;			//0x238 Projectile launch Z
		float						launchSwimZ;		//0x23C Projectile launch Z (swimming)
		uint8_t unk_240[0x8];
		unsigned int					unitTypeFlag;		//0x248
		uint8_t						unk_24C[0x38];		//0x24C
		float						x;					//0x284
		float						y;					//0x288
		uint8_t						unk_28C[0x48];		//0x28C
		ColorARGB					vertexColor;		//0x2D4	0xAARRGGBB
		uint8_t						unk_2D8[0x38];
	};

	union CUnit_174 {
		CUnit_174_30C pre_124d;
		CUnit_174_310 post_124d;
	};

	struct CUnit {//TODO +0x14C -1 
		void** vtable;
		unsigned int					refCount;			//0x4
		unsigned int					unk_8;				//0x8
		HashGroup					hash;				//0xC
		uint8_t unk_14[0xC];
		unsigned int					stateFlag;			//0x20 表示状态, 1 << 3 表示无敌 TODO 继续研究其他bit
		uint8_t unk_24[0xC];
		unsigned int					typeId;				//0x30
		uint8_t unk_34[0x1C];
		CPreselectUI* preSelectUI;		//0x50
		float						unk_54;				//0x54
		unsigned int					owner;				//0x58
		uint8_t unk_5C[0x44];
		HashGroup					hash_unkA0;			//0xA0
		uint8_t	unk_A8[0x38];
		float						defense;			//0xE0
		unsigned int					defenseType;		//0xE4
		uint8_t unk_E8[0x84];
		HashGroup					hash_unk16C;		//0x16C
		CUnit_174					unit_174;			//0x174
		/*
		uint8_t unk_174[0x68];//在这一段从1.24d? 加了4字节，< 1.24d为0x64

		//0x19C,0x1A0与当前OrderTarg有关

		HashGroup					abilityHash;		//0x1DC	//以下偏移为按照1.24d以后
		uint8_t unk_1E4[0x44];
		//TODO 0x194 = current action ?
		float						impactZ;			//0x228 Projectile impact Z
		float						impactSwimZ;		//0x22C	Projectile impact Z (swimming)
		float						launchX;			//0x230	Projectile launch X
		float						launchY;			//0x234	Projectile launch Y
		float						launchZ;			//0x238 Projectile launch Z
		float						launchSwimZ;		//0x23C Projectile launch Z (swimming)
		uint8_t unk_240[0x8];
		unsigned int					unitTypeFlag;		//0x248
		uint8_t						unk_24C[0x38];		//0x24C
		float						x;					//0x284
		float						y;					//0x288
		uint8_t						unk_28C[0x48];		//0x28C
		ColorARGB					vertexColor;		//0x2D4	0xAARRGGBB
		uint8_t						unk_2D8[0x38];
		*/
	};//sizeof = 0x30C ~ 0x310

	struct CItem {
		void** vtable;
		uint8_t			unk_04[0x8];
		HashGroup		hash;				//0xC
		uint8_t			unk_14[0x1C];
		unsigned int		typeId;				//0x30
		uint8_t			unk_34[0x28];		//0x34


		unsigned int		unkFlag_5C;			//0x5C

		uint8_t			unk_60[0x28];		//0x60

		unsigned int		firstAbilityId;		//0x88
		HashGroup		hashOwner;			//0x8C
		uint8_t			unk_94[8];
		HashGroupCargo	hashAbility[4];		//0x9C
		CAgentTimer		timer0;				//0xD0
		CAgentTimer		timer1;				//0xE4
	};//sizeof = 0xF8

	struct CDestructable {
		void** vtable;
		//...
	};

	/*6F05063C 可参考 */
	//2适用于flag2 & 0x80不为0的技能
	struct AbilityUIDef {
		void** table;			//0x0
		uint8_t		unk_4[0x7C];	//0x4
		unsigned int	hotkey_levels;	//0x80
		unsigned int* hotkey_items;	//0x84
		unsigned int	unk_88;			//0x88
		unsigned int	hotkey2_levels;	//0x8C
		unsigned int* hotkey2_items;	//0x90
		uint8_t		unk_94[0xA0];	//0x94
		unsigned int	tooltip_levels;	//0x134
		char** tooltip_items;	//0x138
		unsigned int	unk_13C;		//0x13C
		unsigned int	tooltip2_levels;//0x140
		char** tooltip2_items;	//0x144		
		uint8_t		unk_148[0x10];	//0x148
		unsigned int	desc_levels;	//0x158
		char** desc_items;		//0x15C
		unsigned int	unk_160;		//0x160
		unsigned int	desc2_levels;	//0x164
		char** desc2_items;	//0x168
		//...
	};

	struct AbilityDefData {
		void** vtable;
		uint8_t	unk_4[0x24]; //0x4
		AbilityUIDef* abilityUIDef; //0x28
		unsigned int uiDefAvailable;	//0x2C
		unsigned int abilityBaseTypeId;	//0x30
		//...
	};//sizeof = ?

	struct CAbility {
		void** vtable;			//0x0
		unsigned int unk4;
		unsigned int		flag;			//0x8
		HashGroup		hash;			//0xC
		unsigned int unk14;
		unsigned int unk18;
		unsigned int unk1C;
		unsigned int		flag2;			//0x20	& 0x200 表示是否在CD中
		HashGroup		nextAbilityHash;//0x24/
		unsigned int unk2C;
		CUnit* abilityOwner;	//0x30
		unsigned int		id;				//0x34
		unsigned int		unk38;			//0x38
		unsigned int		unk3C;			//0x3C
		unsigned int		unk40;			//0x40
		uint8_t unk44[0x8];				//0x44
		unsigned int		unk4C;			//0x4C
		unsigned int		level;			//0x50
		AbilityDefData* defData;		//0x54
		CAgentTimer		timer58;		//0x58
	};//sizeof = 0x6C

	struct FloatMini {
		void** vtable;			//0x0
		float			value;			//0x4
	};//sizeof = 0x8

	struct FloatMiniB {
		void** vtable;
		float			value;			//0x4
		unsigned int		unk_8;			//0x8
		unsigned int		unk_C;			//0xC
	};//sizeof = 0x10

	struct CAbilityBuildInProgress
	{
		CAbility		baseAbility;
		uint8_t	unk_6C[0x8];
		FloatMini		buildTimeTotal;	//0x74
		uint8_t unk_7C[0x8];
		HashGroup		hashPoReal;		//0x84
		//...
	};//sizeof = ?

	struct CAbilityUpgradeInProgress
	{
		CAbility		baseAbility;
		uint8_t	unk_6C[0x8];
		FloatMini		buildTimeTotal;	//0x74
		uint8_t unk_7C[0x8];
		HashGroup		hashPoReal;		//0x84
		uint8_t unk_8C[0x1C];
		unsigned int		upgradeTypeId;	//0xA8
		//...
	};

	struct CAbilityQueue
	{
		CAbility		baseAbility;
		uint8_t unk_6C[0x8];
		FloatMini		queueTimeTotal;	//0x74
		uint8_t unk_7C[0x8];
		HashGroup		hashPoReal;		//0x84
		uint8_t unk_8C[0x14];
		unsigned int		queueSlotCount;	//0xA0
		unsigned int		queueItemType[7];
		//...
	};

	struct CAbilitySpell {
		CAbility		baseAbility;	//0x0
		unsigned int		unk6C;			//0x6C = dword_6F92ED9C
		unsigned int		unk70;
		unsigned int		unk74;
		unsigned int		unk78;
		unsigned int		unk7C;
		FloatMini		floatMini80;	//value = dword_6FAAE470 似乎是施法时间
		FloatMini		floatMini88;	//value = dword_6FAAE470
		FloatMini		floatMini90;	//value = dword_6FAAE470
		FloatMini		floatMini98;	//value = dword_6FAAE470
		unsigned int		unkA0;			// = 0xFFFFFFFF
		unsigned int		unkA4;			// = 0xFFFFFFFF
		unsigned int		unkA8;			// = 0
		unsigned int		unkAC;			// = 0
		FloatMini		floatMiniB0;	//value = dword_6FAAE470
		unsigned int		unkB8;			// = 0
		CAgentTimer		timer_last;
		CAgentTimer		timer_cooldown;
	};//sizeof = 0xE4

	struct CAbilitySpellBook {
		CAbilitySpell	baseSpell;
		unsigned int		count;				//0xE4
		unsigned int		SpellTypeId[11];	//0xE8
		//
	};//sizeof = ?

	struct CAbilityAttack {
		CAbility			baseAbility;	//0x0
		HashGroup			hashAcquired;	//0x6C
		uint8_t unk_70[0x14];
		unsigned int			dice_weap0;		//0x88
		unsigned int			dice_weap1;		//0x8C
		uint8_t unk_90[0x4];
		unsigned int			sides_weap0;	//0x94
		unsigned int			sides_weap1;	//0x98
		uint8_t unk_9C[0x4];
		unsigned int			dmgPlus_weap0;	//0xA0
		unsigned int			dmgPlus_weap1;	//0xA4
		uint8_t unk_A8[0x4];
		unsigned int			buffs_weap0;	//0xAC
		unsigned int			buffs_weap1;	//0xB0
		uint8_t unk_B4[0x28];
		unsigned int			weapTp_weap0;	//0xDC
		unsigned int			weapTp_weap1;	//0xE0
		uint8_t unk_E4[0x10];
		unsigned int			atkType_weap0;	//0xF4
		unsigned int			atkType_weap1;	//0xF8
		uint8_t unk_FC[0x58];
		FloatMini			cool_weap0;		//0x154
		FloatMini			cool_weap1;		//0x15C
		unsigned int unk_164;					//			TODO
		FloatMiniB			dmgPt_weap0;	//0x168
		FloatMiniB			dmgPt_weap1;	//0x178
		unsigned int unk_188;					//			TODO
		FloatMiniB			backSw_weap0;	//0x18C
		FloatMiniB			backSw_weap1;	//0x19C
		FloatMiniB			factor;			//0x1AC
		FloatMini			FMini_unk1BC;	//0x1BC		TODO
		CAgentTimer			timer_unk1C4;	//0x1C4		TODO
		CAgentTimer			timer_cool;		//0x1D8
		CAgentTimer			timer_dmgPt;	//0x1EC
		CAgentTimer			timer_backSw;	//0x200
		uint8_t unk_204[0x40];
		FloatMini			range_weap0;	//0x254
		FloatMini			range_weap1;	//0x25C
		unsigned int unk_264;					//			TODO
		FloatMini			FMini_unk268;	//			TODO
		FloatMini			FMini_unk270;	//			TODO
		FloatMini			FMini_unk278;	//			TODO
		CAgentTimer			timer_unk280;	//			TODO
		CAgentTimer			timer_unk294;	//			TODO
		FloatMini			FMini_unk2A8;	//			TODO
		FloatMini			FMini_unk2B0;	//			TODO
		uint8_t unk_2B8[0x10];
		FloatMini			FMini_unk2C8;	//			TODO
		//padding
		uint8_t unk_2D0[0x108];
		//padding
		CAgentTimer			timer_unk3D8;	//0x3D8		TODO
		uint8_t unk_3EC[0xC];
		FloatMini			unk_3F8;		//			TODO
		FloatMini			unk_400;		//			TODO
		FloatMini			unk_408;		//			TODO
	};//sizeof = 0x410

	struct CAbilityCargoHold {
		void** vtable;			//0x0
		uint8_t unk_4[0x2C];
		CUnit* abilityOwner;	//0x30
		unsigned int			id;				//0x34
		uint8_t unk_38[0x3C];
		unsigned int			cargoSpaceUsed;		//0x74
		uint8_t unk_78[0xC];
		HashGroupCargo		cargoObjectHash[8];	//0x84	TODO 找到大小
	};//sizeof = ?

	//TODO CUnit等继承
	struct CWar3Image {
		void** vtable;			//0x0
		unsigned int unk_4[0x4];
		CObserver			observer_unk14;	//0x14
		unsigned int unk_20[0x3];
	};//sizeof = 0x2C

	//--------------------------------------------------------------------------------
	// Projectiles
	//--------------------------------------------------------------------------------

	struct CBulletBase {
		CWar3Image			baseWar3Image;
		HashGroup			sourceHash;		//0x2C	发出子弹的物体
		unsigned int unk_34[0x3];
		FloatMini			attack;			//0x40	攻击力（含除暴击以外附加值）
		FloatMini			crit;			//0x48	暴击所附加攻击力
		FloatMini			speed;			//0x50	移动速度
		unsigned int			weaponSound;	//0x58	武器声音 Jass::WEAPON_TYPE_X
		unsigned int			attackType;		//0x5C	攻击类型 Jass::ATTACK_TYPE_X
		unsigned int unk_60;					//0x60	flag，值通常=00 01 00 00，第一个unsigned char改为0xFF会无伤害	TODO 研究一下攻击miss与暴击
		CAgentTimer			timer_unk64;	//0x64	TODO
	};//sizeof = 0x78

	struct ProjectilePosition {
		void** vtable;
		unsigned int			refCount;
		HashGroup			posHash;
	};//sizeof = 0x10

	struct SmartPosition {
		void** vtable;
		unsigned int			refCount; //?
		HashGroup			posHash;
	};//sizeof = 0x10

	struct SplashData {
		FloatMini			factorMedium;
		FloatMini			factorSmall;
		FloatMini			areaFull;
		FloatMini			areaMedium;
		FloatMini			areaSmall;
	};//sizeof = 0x28

	struct CBullet {
		CBulletBase			baseBulletBase;
		ProjectilePosition	posData;		//0x78	可以使用0x6F47A580 thiscall (xyz*) 返回大地图坐标（中途似乎先得到了另一种xyz?）
		FloatMini			arc;			//0x88	missile arc
	};//sizeof = 0x90

	struct CMissile {
		CBullet				baseBullet;
		FloatMini			FMini_unk90;	//0x90
		FloatMini			FMini_unk98;	//0x98
		HashGroup			targetHash;		//0xA0	missile目标物体, miss时为无(0xFFFFFFFF x2)
		unsigned int unk_A8;
	};//sizeof = 0xAC

	struct CMissileSplash {
		CMissile			baseMissile;
		unsigned int unk_AC;	//=0
		unsigned int unk_B0;	//某种flag
		SplashData			splashData;			//0xB4
	};//sizeof = 0xDC

	struct CMissileBounce {
		CMissile			baseMissile;
		unsigned int unk_AC;
		unsigned int unk_B0;	//某种flag
		unsigned int unk_B4;	//=1
		FloatMini			dmgAreaFull;	//0xB8
		FloatMini			dmgLoss;		//0xC0
	};//sizeof = 0xC8

	struct CMissileLine {
		CMissile			baseMissile;
		unsigned int			typeId;			//0xAC 攻击者的单位类型id ?
		unsigned int unk_B0;
		unsigned int unk_B4;	//某种flag
		FloatMini			dmgLoss;		//0xB8
		FloatMini			dmgSpillDist;	//0xC0 溅射距离
		FloatMini			dmgSpillRadius;	//0xC8 溅射半径
		FloatMini			angle;			//0xD0 方向(弧度)
		SmartPosition		smartpos_unkD8;
		SmartPosition		smartpos_unkE8;
		unsigned int unk_F8;
		CAgentTimer			timer_unkFC;	//0xFC
	};//sizeof = 0x110

	struct CArtillery {
		CBullet				baseBullet;
		unsigned int unk_90;
		HashGroup			targetHash;		//0x94
		unsigned int unk_9C;
		unsigned int unk_A0;	//某种flag
		SplashData			splashData;		//0xA4
	};//sizeof = 0xCC

	struct CArtilleryLine {
		CArtillery			baseArtillery;
		unsigned int			typeId;			//0xCC 攻击者的单位类型id
		FloatMini			dmgLoss;		//0xD0
		FloatMini			dmgSpillDist;	//0xD8
		FloatMini			dmgSpillRadius;	//0xE0
		FloatMini			angle;			//0xE8 方向(弧度)
		unsigned int unk_F0[0x8];//TRefCnt ?
		void* unk_110;//TRefCnt* ?
		CAgentTimer			timer_unk114;	//0x114
	};//sizeof = 0x128
	//--------------------------------------------------------------------------------

	struct CTriggerWar3 {
		void** vtable;			//0x0
		uint8_t				unk_4[0x78];	//0x4
	};//sizeof = 0x7C

	struct CSplatEmitter;	//size = 0xBC	TODO 验证class
	struct CDoodads;
	struct CDoodadCustomData;
	struct CSpawn;			//size = 0x14

	//0FC30010 - 0FC54910(End:0FC554FF0)
	//0x24900 = 149760
	struct MapDataBlock {
		float		unk_0;		//0x0 0.0
		int		unk_4;		//0x4 -1
	};


	struct MapData {
		unsigned int			unk_0;			//0x0	= 1?
		unsigned int			unk_4;			//0x4	= 1?
		unsigned int			unk_8;			//0x8	= 1?

		unsigned int			v1a;			//0xC	例如 37280
		unsigned int			v1b;			//0x10	例如 37249
		MapDataBlock* dataBlocks1;	//0x14	应该是地图数据
		unsigned int			unk_18;			//0x18	例子 32

		unsigned int			v2a;			//0x2C	例子		7075
		unsigned int			v2b;			//0x30			7065
		MapDataBlock* dataBlocks2;	//0x34	应该是地图数据
		unsigned int			unk_38;			//0x38	例子 25

		unsigned int			v3a;			//0x3C	例子		4608
		unsigned int			v3b;			//0x40			4382
		MapDataBlock* dataBlocks3;	//0x44	应该是地图数据
		unsigned int			unk_48;			//0x48	例子 256

		unsigned int			unk_4C;			//0x4C	FFFFFF80
		unsigned int			unk_50;			//0x50	例子 300
		unsigned int			unk_54;			//0x54	-1
		unsigned int			unk_58;			//0x58	0
		unsigned int			counter;		//0x5C	53030402
		//...
	};

	struct MapInfo_14 {
		uint8_t	unk_0[0x20];						//0x0
	};

	struct MapInfo {
		void* unk_0;								//0x0	8byte 后面是地图名字符串
		unsigned int unk_4;
		MapData* mapData;			//0x8
		CDoodads* doodads;			//0xC	
		CDoodadCustomData* doodadCustomData;	//0x10
		MapInfo_14* unk_14;
		CSplatEmitter* splatEmitters[5];	//0x18
		CSpawn* spawn;				//0x2C
		void* textTagManager;		//0x30
		void* unk_34;
		char					mapPath[100];		//0x38

		//下面似乎是各种Ability类的Generator
	};

	struct CNetData {
		void** vtable;			//0x0
		uint8_t				unk_4[0x610];	//0x4
		unsigned int			replayState;		//0x614, 录像模式=2 ?
		uint8_t				unk_618[0x1658];//0x618
		unsigned int			time;			//0x1C70
		uint8_t				unk_1C74[0x640];		//0x1C74
		unsigned int			replaySpeedFactorNumerator;	//0x22B4
		unsigned int			replaySpeedFactorDenominator; //0x22B8
		//..
	};

	struct CNetObserver {
		void** vtable;			//0x0
		CObserver			observer_4;		//0x4
		CObserver			observer_10;	//0x10
	};

	//--------------------------------------------------------------------------------
	// Event
	//--------------------------------------------------------------------------------
	const unsigned int EVENT_FOCUS = 0x40040065;

	const unsigned int EVENT_KEYDOWN = 0x40060064;
	const unsigned int EVENT_KEYUP = 0x40060066;
	const unsigned int EVENT_CHAR = 0x40060067;

	const unsigned int EVENT_MOUSE_DOWN = 0x400500C8;
	const unsigned int EVENT_MOUSE_UP = 0x400500C9;
	const unsigned int EVENT_MOUSE_MOVE = 0x400500CA;
	const unsigned int EVENT_MOUSE_SCROLL = 0x400500CD;

	/* send
	0x62C1C0 40060064 keydown fastcall (&uint32KeyVal, lpObjCInputObserver)
	0x62C250 40060065
	0x62C2E0 40060066 keyup fastcall (&uint32KeyVal, lpObjCInputObserver)
	0x62C140 40060067

	0x62C370 400500C8
	0x62C3E0 400500C9
	0x62C450 400500CA
	0x62C4C0 400500CD
	0x62C530 400500CB
	0x62C5A0 400500CC
	0x62C610 40060068
	0x62C6A0 40040064
	0x62C720 40040065
	*/

	//当点击屏幕上方菜单，或闲置农民按钮时ID = 0
	const unsigned int EVENT_ABILITY_CLICK = 0x00030064; //当点击的是技能栏中的按钮时
	const unsigned int EVENT_ITEM_CLICK = 0x00030065; //当点击的是物品栏中的按钮时
	const unsigned int EVENT_TRAINABLE_CLICK = 0x00030066; //当点击的是训练中的按钮时
	const unsigned int EVENT_MINIMAPSIGNAL_CLICK = 0x00000015; //Minimap Signal
	const unsigned int EVENT_MINIMAPTERRAIN_CLICK = 0x00000016; //Minimap Terrain
	const unsigned int EVENT_ALLYCOLORMODE_CLICK = 0x00000017; //Minimap AllyColor
	const unsigned int EVENT_MINIMAPCREEP_CLICK = 0x00000018; //Minimap Creep
	const unsigned int EVENT_FORMATION_CLICK = 0x0000001B; //Formation Movement

	const unsigned int EVENT_GAMETICK = 0x40090092;
	const unsigned int EVENT_GAMEIDLETICK = 0x40090093;
	const unsigned int EVENT_GAMEASYNCIDLETICK = 0x40090094;
	const unsigned int EVENT_GAMESTART = 0x4009007E;
	const unsigned int EVENT_GAMEREADY = 0x4009007F;
	const unsigned int EVENT_GAMELEAVE = 0x40090081;

	const unsigned int EVENT_PLAYER_CHAT = 0x80210;

	//CD结束 = 0xD01B0
	const unsigned int EVENT_UNIT_ATTACK_RELEASED = 0xD01B1;
	const unsigned int EVENT_UNIT_ACQUIRE_START = 0xD0163;
	const unsigned int EVENT_UNIT_ACQUIRE_READY = 0xD01AF;
	const unsigned int EVENT_UNIT_ACQUIRE_REFRESH = 0xD0144;

	struct CEvent {
		void** vtable;			//0x0
		unsigned int		unk_4;			//0x4
		unsigned int		id;				//0x8
		void* object;			//0xC 这个应该是触发event的对象
	};//sizeof = 0x10

	struct CKeyEvent {
		CEvent			baseEvent;		//0x0
		unsigned int		keyCode;		//0x10
		unsigned int		unk_14;			//0x14
		unsigned int		unk_18;			//0x18
		unsigned int		unk_1C;			//0x1C
	};

	//左键=1 中键=2 右键=4 左边靠后的侧键=8 左边靠前的侧键=0x10
	struct CMouseEvent {
		CEvent			baseEvent;		//0x0
		unsigned int unk_10;
		unsigned int		keyCurrent;		//0x14 按下事件为有效按下键值，弹起事件为有效弹起键值
		unsigned int		keyFlag;		//0x18 按下事件为所有按下键值，弹起事件为剩余按下键值，拖拽事件为拖拽键值
		unsigned int unk_1C;
		unsigned int unk_20;
		float			x;				//0x24 [0, 0.8]		2D坐标(纹理)
		float			y;				//0x28 [0, 0.6]		左下角为原点(0, 0)
	};//sizeof = 0x2C

	struct CControlDragEvent {
		CMouseEvent		baseMouseEvent;	//0x0
		unsigned int unk_2C;
		uint16_t		pixel_x;		//0x30 [0, 分辨率X]	2D坐标(实际像素)，
		uint16_t		pixel_y;		//0x32 [0, 分辨率Y]	左上角为原点(0, 0)
		unsigned int unk_34[0x6];
	};

	struct CControlWheelEvent {
		CMouseEvent		baseMouseEvent;	//0x0
		unsigned int		code1;	//0x2C
		unsigned int		code2;	//0x30
	};

	struct CSimpleButtonClickEvent {
		CEvent			baseEvent;		//0x0	物品事件object为CInventoryBar, sizeof = 0x1C0
		unsigned int		keyFlag;		//0x10	同MouseEvent,但是应该只响应左右键
		/*
			注：以下实际上不属于event内容
			uint8_t	unk_14[0x14];	//0x14
			void*					simpleTop;		//0x28 CSimpleTop
			unsigned int				unk_2C;			//0x2C
			CSimpleButton*			button;				//0x30 同target
			float					x;				//0x34
			float					y;				//0x38
			unsigned int				unk_3C;			//0x3C
		*/
	};

	struct CNetEventGameTick {
		CEvent			baseEvent;		//0x0
		unsigned int		lastSyncTime;	//0x10
		//...
	};

	struct CPlayerChatEvent {
		CEvent			baseEvent;		//0x0
		CPlayerWar3* player;			//0x10
		RCString		contentRCString;//0x14
	};


	//--------------------------------------------------------------------------------
	// Packet
	//--------------------------------------------------------------------------------

	struct PacketSender {
		void** vtable;				//0x0	Offset(DATASTORECACHE1460_VTABLE)
		void* packet;				//0x4	pointer to real packet
		unsigned int		unk_8;				//0x8	default 0
		unsigned int		unk_C;				//0xC	default 0x5B4
		unsigned int		sizePacket;			//0x10
		int				unk_14;				//0x14	default -1
	};

	//TODO：非游戏默认class应当修改前缀
	struct PacketGeneral {
		uint8_t			id;
	};

	struct PacketAction {
		uint8_t			id;					//0x0	default 0x10
		uint16_t		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id like 0xD0004 or 'ewsp'
		HashGroup		hashUsedItem;		//0x7	hash group of item used
	};//sizeof = 15

	struct PacketActionPoint {
		uint8_t			id;					//0x0	default 0x11
		uint16_t		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id
		HashGroup		hashUsedItem;		//0x7	hash group of item used
		float			targetX;			//0xF
		float			targetY;			//0x13
	};//sizeof = 23

	struct PacketActionTarget {
		uint8_t			id;					//0x0	default 0x12
		uint16_t		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id
		HashGroup		hashUsedItem;		//0x7	hash group of item used
		float			targetX;			//0xF
		float			targetY;			//0x13
		HashGroup		hashWidgetTarget;	//0x17	hash group of target widget
	};//sizeof = 31

	struct PacketActionDropItem {
		uint8_t			id;					//0x0	default 0x13
		uint16_t		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id
		HashGroup		hashUsedItem;		//0x7	hash group of item used
		float			targetX;			//0xF
		float			targetY;			//0x13
		HashGroup		hashTarget;			//0x17	hash group of target widget
		HashGroup		hashItem;			//0x1F	hash group of transferred item
	};//sizeof = 39

	struct PacketActionIndirect {
		uint8_t			id;					//0x0	default 0x14
		uint16_t		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id
		HashGroup		hashUsedItem;		//0x7	hash group of item used
		float			targetX;			//0xF
		float			targetY;			//0x13
		unsigned int		typeId;				//0x17	type id of target
		uint8_t			unk_1F[9];
		float			targetX2;			//0x24
		float			targetY2;			//0x28
	};//sizeof = 44

	struct PacketSelection {
		uint8_t			id;					//0x0	default 0x16
		uint8_t			mode;				//0x1	select=1, deselect=2
		uint16_t		count;				//0x2	count of manipulating widgets
		HashGroup		objects[12];		//0x4	hash group of widgets
	};

	struct PacketControlGroupAssign {
		uint8_t			id;					//0x0	default 0x17
		uint8_t			numGroup;			//0x1	0~9 for control group 1~10
		uint16_t		count;				//0x2	count of manipulating widgets
		HashGroup		objects[12];		//0x4	hash group of widgets
	};

	struct PacketControlGroupSelect {
		uint8_t			id;					//0x0	default 0x18
		uint8_t			numGroup;			//0x1	0~9 for control group 1~10
		uint8_t			unk_2;				//0x2	default 0x3
	};// sizeof = 3

	struct PacketSubgroup {
		uint8_t			id;					//0x0	default 0x19
		unsigned int		typeId;				//0x1	type id of new active subgroup
		HashGroup		object;				//0x5	hash of new active subgroup unit
	};// sizeof = 13

	struct PacketPreSubSelection {
		uint8_t			id;					//0x0	default 0x1A
	};//sizeof = 1

	struct PacketSelectionItem {
		uint8_t			id;					//0x0	default 0x1C
		uint8_t			unk_1;				//0x1	always 0x04
		HashGroup		hashItem;			//0x2	hash of item to select
	};//sizeof = 10

	struct PacketCancelRevive {
		uint8_t			id;					//0x0	default 0x1D
		HashGroup		hashHero;			//0x2	hash of reviving hero canceled
	};//sizeof = 9

	struct PacketCancelQueue {
		uint8_t			id;					//0x0	default 0x1E
		uint8_t			slot;				//0x1	0 ~ n for slots
		unsigned int		typeId;				//0x2	type id of canceled object
	};//sizeof = 6

#ifdef _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

}//namespace war3

#endif