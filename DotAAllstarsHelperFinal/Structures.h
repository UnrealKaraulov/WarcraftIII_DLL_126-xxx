#pragma once

#pragma pack(push,1)



struct ClickPortrainForId
{
	int abilid;
	int keycode;
	bool checkforcd;
};


struct ObjInfoAction
{
	int ObjId;
	int ObjId2;
	int Action;
	std::string CustomMessage;
	ObjInfoAction(int _ObjId, int _ObjId2, int _Action, std::string _CustomMessage)
	{
		ObjId = _ObjId;
		ObjId2 = _ObjId2;
		Action = _Action;
		CustomMessage = _CustomMessage;
	}
};



struct KeyChatActionStruct
{
	int VK;
	bool IsShift;
	bool IsCtrl;
	bool IsAlt;
	int SendToAll;
	std::string Message;
};


struct KeySelectActionStruct
{
	int VK;
	bool IsShift;
	bool IsCtrl;
	bool IsAlt;
	int GroupHandle;
	std::vector< unsigned char*> units;
};

struct KeyCalbackActionStruct
{
	int VK;
	bool IsShift;
	bool IsCtrl;
	bool IsAlt;
	int args[8];
};




struct CustomHPBar
{
	int unittypeid;
	unsigned int color;
	float scalex;
	float scaley;
};

struct Matrix1//Matrix 4x4
{
	float flt1;//0
	float flt2;//4
	float flt3;//8
	float flt4;//12
	float flt5;//16
	float flt6;//20
	float flt7;//24
	float flt8;//28
	float flt9;//32
	float flt10;//36
	float flt11;//40
	float flt12;//44
	float flt13;//48
	float flt14;//52
	float flt15;//56
	float flt16;//60
};


struct BarStruct
{
	unsigned char* _BarClass;		// 0
	int _unk1_flag;		// 4
	int _unk2_flag;		// 8
	int _unk3_pointer;	// C
	int _unk4;			// 10
	int _unk5;			// 14
	int _unk6;			// 18
	int _unk7;			// 1c
	int _unk8;			// 20
	int _unk9;			// 24
	int _unk10;			// 28
	int _unk11;			// 2c
	int _unk12;			// 30
	int _unk13;			// 34
	int _unk14;			// 38
	int _unk15_pointer;	// 3c
	int _unk16_pointer;	// 40
	float offset1;		// 44
	float offset2;		// 48
	float offset3;		// 4c
	float offset4;		// 50
	int _unk17_flag;	// 54
	float ScaleX;		// 58
	float ScaleY;		// 5c
	float Scale;		// 60
	int _unk18;			// 64
	int _unk19_pointer; // 68
	int _unk20;			// 6C
	int _unk21;			// 70
	int _unk22;			// 78
	int _unk23;			// 7C
	int _unk24;			// 80
	int _unk25;			// 84
	int bartype;		// 88
	int _unk26;			// 8C
	int vis1;			// 90
	int vis2;			// 94
	float offset5;		// 98
	float offset6;		// 9C
	float offset7;		// 100
	float offset8;		// 104
	float offset9;		// 108
	int _unk29;			// 10C
	int _unk30;			// 110
	int _unk31;			// 114
	int _unk32;			// 118
	int _unk33;			// 11C
	int _unk34;			// 120
	int _unk35;			// 124
	int _unk36;			// 128
	int _unk37;			// 12C
	int _unk38;			// 130
	int _unk39;			// 134
	int _unk40_pointer;	// 138
	int _unk41_pointer;	// 13C
	int _unk42;			// 140
	int _unk43_pointer;	// 144
	int _unk44_pointer;	// 148
	int _unk45;			// 14C
	int _unk46_pointer;	// 150
	int _unk47_pointer;	// 154
	int _unk48;			// 158
	int _unk49_pointer;	// 15C
	int _unk50_pointer;	// 160
	int _unk51;			// 164
	int _unk52_pointer;	// 168
	int _unk53_pointer;	// 16C
	int _unk54;			// 170
	int _unk55_pointer;	// 174
	int _unk56_pointer;	// 178
	int _unk57;			// 17C
	int _unk58_pointer;	// 180
	int _unk59_pointer;	// 184
	int _unk60;			// 188
	float offset10;		// 18C
	float offset11;		// 190
	float offset12;		// 194
	int _unk61_pointer;	// 198
	int _unk62;			// 19C
	int _unk63;			// 200
	int _unk64_pointer;	// 204
	int _unk65_pointer;	// 208
	int _unk66;			// 20C
	int _unk67;			// 210
	float offset13;		// 214
	unsigned char* unitaddr;		// 218
	int _unk68;			// 21C
	int _unk69;			// 220
};

struct StrTableNode
{
	void* unknownaddr;
	struct StringRep* curr;
};

struct String2H
{
	unsigned int			hash;//0x0
	void* unk1;//0x4
	void* unk2;//0x8
	void* prev;//0xc
	String2H* next;//0x10
	char* text;//0x14
	int					id;//0x18
};

struct StringRep {
	void** vtable;		//0x0
	unsigned int			refCount;	//0x4
	unsigned int			hash;		//0x8
	StrTableNode* table;		//0xC
	StringRep* prev;		//0x10
	/* txtnode */
	void* prevtxtnode;//0x14  ?
	StringRep* next;		//0x18
	char* text;		//0x1C
};//sizeof = 0x20

struct jRCString {
	void** vtable;		//0x0
	unsigned int			unk_4;		//0x4
	StringRep* stringRep;	//0x8
	jRCString()
	{
		vtable = NULL;
		unk_4 = NULL;
		stringRep = NULL;
	}
};//sizeof = 0xC 


struct ModelCollisionFixStruct
{
	std::string FilePath;
	float X, Y, Z, Radius;
	ModelCollisionFixStruct()
	{
		FilePath = std::string();
		X = Y = Z = Radius = 0.0f;
	}
};
struct ModelTextureFixStruct
{
	std::string FilePath;
	int TextureID;
	std::string NewTexturePath;
	ModelTextureFixStruct()
	{
		FilePath = std::string();
		NewTexturePath = std::string();
		TextureID = 0;
	}
};
struct ModelPatchStruct
{
	std::string FilePath;
	std::string patchPath;
	ModelPatchStruct()
	{
		FilePath = std::string();
		patchPath = std::string();
	}
};
struct ModelRemoveTagStruct
{
	std::string FilePath;
	std::string TagName;
	ModelRemoveTagStruct()
	{
		FilePath = std::string();
		TagName = std::string();
	}
};
struct ModelSequenceReSpeedStruct
{
	std::string FilePath;
	std::string AnimationName;
	float SpeedUp;
	ModelSequenceReSpeedStruct()
	{
		FilePath = std::string();
		AnimationName = std::string();
		SpeedUp = 0.0f;
	}
};
struct ModelScaleStruct
{
	std::string FilePath;
	float Scale, ScaleX, ScaleY, ScaleZ;
	ModelScaleStruct()
	{
		Scale = 0.0f;
		ScaleX = 0.0f;
		ScaleY = 0.0f;
		ScaleZ = 0.0f;
		FilePath = std::string();
	}
};
struct ModelSequenceValueStruct
{
	std::string FilePath;
	std::string AnimationName;
	int Indx;
	float Value;
	ModelSequenceValueStruct()
	{
		FilePath = std::string();
		AnimationName = std::string();
		Indx = 0;
		Value = 0.0f;
	}
};

struct ICONMDLCACHE
{
	uint64_t _hash;
	size_t hashlen;
	unsigned char* buf;
	size_t size;
};

struct FileRedirectStruct
{
	std::string NewFilePath;
	std::string RealFilePath;
	FileRedirectStruct()
	{
		NewFilePath = std::string();
		RealFilePath = std::string();
	}
};

struct FakeFileStruct
{
	char* filename;
	unsigned char* buffer;
	size_t size;
};


struct waroffsetdata
{
	unsigned char* offaddr;
	int offdata;
	bool newdataapp;
	int offnewdata;
	unsigned int FeatureFlag;
};


struct FeatureRestorer
{
	unsigned int FeatureFlag;
	int Bytes;
	int Address;
};





struct Mdx_Texture        //NrOfTextures = ChunkSize / 268
{
	int ReplaceableId;
	char FileName[260];
	unsigned int Flags;                       //#1 - WrapWidth
	//#2 - WrapHeight
};

struct Mdx_FLOAT3
{
	float x;
	float y;
	float z;
};

struct Mdx_Sequence      //NrOfSequences = ChunkSize / 132
{
	char Name[80];

	int IntervalStart;
	int IntervalEnd;
	FLOAT MoveSpeed;
	unsigned int Flags;                       //0 - Looping
	//1 - NonLooping
	FLOAT Rarity;
	unsigned int SyncPoint;

	FLOAT BoundsRadius;
	Mdx_FLOAT3 MinimumExtent;
	Mdx_FLOAT3 MaximumExtent;
};




struct Mdx_SequenceTime
{
	int* IntervalStart;
	int* IntervalEnd;
};



struct Mdx_Track
{
	int NrOfTracks;
	int InterpolationType;             //0 - None
	//1 - Linear
	//2 - Hermite
	//3 - Bezier
	unsigned int GlobalSequenceId;
};

struct Mdx_Tracks
{
	int NrOfTracks;
	unsigned int GlobalSequenceId;

};
struct Mdx_Node
{
	unsigned int InclusiveSize;

	char Name[80];

	unsigned int ObjectId;
	unsigned int ParentId;
	unsigned int Flags;                         //0        - Helper
	//#1       - DontInheritTranslation
	//#2       - DontInheritRotation
	//#4       - DontInheritScaling
	//#8       - Billboarded
	//#16      - BillboardedLockX
	//#32      - BillboardedLockY
	//#64      - BillboardedLockZ
	//#128     - CameraAnchored
	//#256     - Bone
	//#512     - Light
	//#1024    - EventObject
	//#2048    - Attachment
	//#4096    - ParticleEmitter
	//#8192    - CollisionShape
	//#16384   - RibbonEmitter
	//#32768   - Unshaded / EmitterUsesMdl
	//#65536   - SortPrimitivesFarZ / EmitterUsesTga
	//#131072  - LineEmitter
	//#262144  - Unfogged
	//#524288  - ModelSpace
	//#1048576 - XYQuad
};

struct Mdx_GeosetAnimation
{
	unsigned int InclusiveSize;

	FLOAT Alpha;
	unsigned int Flags;                       //#1 - DropShadow
	//#2 - Color
	Mdx_FLOAT3 Color;

	unsigned int GeosetId;

};




struct DelayedPress
{
	unsigned long NeedPressMsg;
	WPARAM NeedPresswParam;
	LPARAM NeedPresslParam;
	unsigned long TimeOut;
	bool ISNULL;
	bool IsAlt;
	bool IsCtrl;
	bool IsShift;
	bool IsCustom;

	int IsNull()
	{
		return ISNULL;
	}

	void Null(int yes)
	{
		ISNULL = yes;
	}

	DelayedPress()
	{
		ISNULL = NeedPressMsg
			= NeedPresswParam = NeedPresslParam
			= TimeOut = IsAlt = IsCtrl = IsShift = IsCustom = 0;
	}
};

struct KeyActionStruct
{
	int VK;
	int btnID;
	int altbtnID;
	bool IsSkill;
	bool IsShift;
	bool IsCtrl;
	bool IsAlt;
	bool IsRightClick;
	bool IsQuickCast;
	unsigned long LastPressTime;
};

struct SpellBonusItemStruct
{
	int id;
	int pc;
};

#pragma pack(pop)