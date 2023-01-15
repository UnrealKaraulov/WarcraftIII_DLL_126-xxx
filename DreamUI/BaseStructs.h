#include "stdafx.h"
#ifndef OBSERVER_STRUCT_H_INCLUDED
#define OBSERVER_STRUCT_H_INCLUDED

namespace war3 {

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif // _MSC_VER

	struct CUnit;
	struct CItem;
	struct CTriggerWar3;
	struct CPreselectUI;
	struct CUnitTip;
	struct CAbilityAttack;
	struct CAbilitySpell;

	struct CMissile;
	struct CMissileSplash;
	struct CMissileBounce;
	struct CMissileLine;
	struct CArtillery;
	struct CArtilleryLine;

	struct NativeFunc;

	struct HashGroup {
		unsigned int hashA;
		unsigned int hashB;
		HashGroup() noexcept : hashA(0xFFFFFFFF), hashB(0xFFFFFFFF) { }
		HashGroup(unsigned int hashA_, unsigned int hashB_) :
			hashA(hashA_), hashB(hashB_) {}
	};

	//union中的struct只能是纯struct，参见CUnit
	struct HashGroupRaw {
		unsigned int hashA;
		unsigned int hashB;

		HashGroup* toHashGroupPtr() { return (HashGroup*)this; }
	};

	struct CObserver {
		void** vtable;			//0x0
		unsigned int		refCount;		//0x4
		unsigned int		registry;		//0x8 ObserverRegistry 
	};//sizeof = 0xC

	struct ObserverEventReg {
		unsigned int		unk_0;					//0x0 = 0
		unsigned int		eventId1;				//0x4
		CObserver* observer;				//0x8
		unsigned int		eventId2;				//0xC
	};//sizeof = 0x10

	template <typename DataType>
	union DataNode {
		struct { //如果没被占用
			DataNode* nextNode;
			BYTE nullBytes[sizeof(DataType) - sizeof(DataNode*)];
		} unusedNode;
		DataType data;
	};

	template <typename DataType, unsigned int N>
	struct DataMemBlock {
		typedef DataNode<DataType> NodeType;
		enum { BLOCK_SIZE = N };
		DataMemBlock<DataType, N>* nextBlock;
		NodeType dataNodes[N];
	};

	template <typename DataType, unsigned int N>
	struct CDataAllocator {
		typedef DataMemBlock<DataType, N> MemBlockType;
		unsigned int dataSize;			// == sizeof(DataType)
		unsigned int memBlockDataNum;	// == N
		unsigned int dataCount;
		MemBlockType* memBlockListHead;
		DataNode<DataType>* nextNode;
	};

	typedef CDataAllocator<ObserverEventReg, 0x400> ObserverEventRegAllocator;
	typedef CDataAllocator<NativeFunc, 0x100> NativeFuncAllocator;

	template <typename DataType>
	struct InstanceGeneratorVtable {
		DataType* (__thiscall* getInstance)();
		void* unk_4;
		void* unk_8;
	};

	template <typename DataType, unsigned int N>
	struct InstanceGenerator {
		InstanceGeneratorVtable<DataType>* vtable;
		CDataAllocator<DataType, N>	allocator;
	};

	typedef InstanceGenerator<CUnit, 0x100> UnitInstanceGenerator; //FIXME  不同版本Unit大小不一样！
	typedef InstanceGenerator<CItem, 0x40> ItemInstanceGenerator;
	typedef InstanceGenerator<CTriggerWar3, 0x80> TriggerInstanceGenerator;
	typedef InstanceGenerator<CAbilityAttack, 0x80> AbilityAttackInstanceGenerator;
	typedef InstanceGenerator<CAbilitySpell, 0x1> AbilitySpellInstanceGenerator;
	typedef InstanceGenerator<CMissile, 0x80> MissileInstanceGenerator;
	typedef InstanceGenerator<CMissileSplash, 0x20> MissileSplashInstanceGenerator;
	typedef InstanceGenerator<CMissileBounce, 0x20> MissileBounceInstanceGenerator;
	typedef InstanceGenerator<CMissileLine, 0x10> MissileLineInstanceGenerator;
	typedef InstanceGenerator<CArtillery, 0x10> ArtilleryInstanceGenerator;
	typedef InstanceGenerator<CArtilleryLine, 0x20> ArtilleryLineInstanceGenerator;

	template <typename ObjectType>
	struct ObjectDynamicArray {
		unsigned int maxCount;			//0x0
		unsigned int currentCount;		//0x4
		ObjectType* objectArray;	//0x8
		unsigned int unk_C;				//0xC
	};

	struct PreselectUIData {
		CUnitTip* unitTip;
		float								unk_4;
		float								unk_8;
		ObjectDynamicArray<CPreselectUI*>	visiblePreselectUIArray;
		ObjectDynamicArray<CPreselectUI*>	freePreselectUIArray;
		ObjectDynamicArray<CUnit*>			autoPreselectUnitArray;
	};

	struct AGILE_TYPE_DATA {
		void** vtable;			//0x0
		unsigned int		hashkey;		//0x4
		uint8_t			unk_8[0x10];	//0x8
		int				this_id;		//0x18
		uint8_t			unk_1C[0x50];	//0x1C
		unsigned int		inited_6C;		//0x6C ?
		uint8_t			unk_70[0x8];	//0x70
		int				parent_id;		//0x78
		unsigned int		inited_7C;		//0x7C ?
	}; //sizeof = 0x80?

	typedef unsigned int HASHKEY_STR;
	typedef unsigned int HASHKEY_NONE; //?

	template <typename NodeType, typename KeyType>
	struct TSHashTable {
		void** vtable;			//0x0
		uint8_t		unk_4[0x24];	//0x4
	}; //sizeof = 0x28?

	template <typename NodeType, typename KeyType, unsigned int N>
	struct TAllocatorHashTable {
		TSHashTable<NodeType, KeyType> baseTSHashTable;
	}; //sizeof = 0x28?

#ifdef _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

}//namespace war3
#endif