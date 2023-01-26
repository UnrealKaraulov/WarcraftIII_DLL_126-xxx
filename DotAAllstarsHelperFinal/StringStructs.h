#ifndef STRINGSTRUCTS_H_INCLUDED
#define STRINGSTRUCTS_H_INCLUDED

namespace war3 {
	#ifdef _MSC_VER
	#pragma pack(push, 1)
	#endif // _MSC_VER

	struct StringRep {
		void**				vtable;		//0x0
		uint32_t			refCount;	//0x4
		uint32_t			unk_8;		//0x8
		uint32_t			list_C;		//0xC
		uint32_t			unk_10;		//0x10
		uint32_t			unk_14;		//0x14
		StringRep*			next;		//0x18
		char*				text;		//0x1C
	};//sizeof = 0x20

	struct RCString {
		void**				vtable;		//0x0
		uint32_t			unk_4;		//0x4
		StringRep*			stringRep;	//0x8
	};//sizeof = 0xC

	#ifdef _MSC_VER
	#pragma pack(pop)
	#endif // _MSC_VER
}
#endif