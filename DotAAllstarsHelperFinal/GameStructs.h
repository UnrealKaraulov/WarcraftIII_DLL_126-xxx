#pragma once

namespace war3 {
#pragma pack(push, 1)

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
	struct CStatBar;
	struct SimpleFrameTextureSettings;
	struct CSimpleTexture;

	struct CFramePointRelative;
	struct CSimpleConsole;

	struct CSelectionWar3;

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

	struct HashGroup {
		unsigned int hashA;
		unsigned int hashB;
		HashGroup() noexcept : hashA(0xFFFFFFFF), hashB(0xFFFFFFFF) { }
		HashGroup(unsigned int hashA_, unsigned int hashB_) :
			hashA(hashA_), hashB(hashB_) {}
	};

	struct HashGroupRaw {
		unsigned int hashA;
		unsigned int hashB;

		HashGroup* toHashGroupPtr() { return (HashGroup*)this; }
	};

	struct StringRep {
		void** vtable;		//0x0
		unsigned int			refCount;	//0x4
		unsigned int			unk_8;		//0x8
		unsigned int			list_C;		//0xC
		unsigned int			unk_10;		//0x10
		unsigned int			unk_14;		//0x14
		StringRep* next;		//0x18
		char* text;		//0x1C
	};//sizeof = 0x20

	struct RCString {
		void** vtable;		//0x0
		unsigned int			unk_4;		//0x4
		StringRep* stringRep;	//0x8
	};//sizeof = 0xC

	struct CCameraWar3 {
		void** vtable;					//0x0
		//0x34 CCamera
		//0x3C CSoundListener
		//0x40 CCamera
	};

	struct CCamera {
		void** vtable;
		//0x3C	大地图x
		//0x40	大地图y
		//0x44	大地图z ?
		//0x8C	distance
	};//sizeof = 0x178

	struct CWorldFrameWar3 {
		void** vtable;					//0x0	 = 0x94157C
		unsigned char			unk_4[0x198];	//0x4
		CCameraWar3* camera;			//0x19C
	};

	struct SimpleTopButtonList {
		void* firstButton;
		unsigned int					firstButtonType;//尚未搞明白
		//...
	};

	struct SimpleTopButtonListArray {
		SimpleTopButtonList* pButtonLists[0x300];
	};


	struct CChatEditBox {
		void** vtable;			//0x0
		unsigned char						unk_4[0x1E0];	//0x4
		char* text;			//0x1E4
	};

	struct CChatEditBar {
		void** vtable;			//0x0
		unsigned char						unk_4[0xAC];	//0x4
		unsigned int					flags;			//0xB0 关闭时101101b 打开时101100b
		unsigned char						unk_B4[0x128];	//0xB4
		unsigned int					visible;		//0x1DC	关闭时0 打开时1
		CChatEditBox* chatEditBox;	//0x1E0
		//...
	};

	struct CSimpleConsole {
		void** vtable;						// = 0x93BD2C
		unsigned char unk_4[0x4];
		CFramePointRelative* frameRelativePoint;			//0x8
		//...
	};//sizeof = 0x134

	struct CFramePointRelative {
		void** vtable;
		CSimpleConsole* simpleConsole;	//0x04 这里可以是任何Frame
		unsigned int					unk_8;			//0x08
		float						relativeX;		//0x0C
		float						relativeY;		//0x10
	};//sizeof = 0x504 (通过DataAllocator_6F4C1AB0得知)

	struct CFramePointAbsolute {
		void** vtable;
		float						absoluteX;		//0x4
		float						absuluteY;		//0x8
	};//sizeof >= 0x10

	//一个全局对象指针GLOBAL_SIMPLETOP
	struct CSimpleTop {
		void** vtable;
		unsigned char unk_4[0x8A8];
		SimpleTopButtonListArray* pButtonListArray;			//0x8AC
		unsigned char unk_8B0[0x4];
		unsigned int						buttonListArrayCount;		//0x8B4
	};

	struct SimpleFrame_DC_Element {
		unsigned int unk_0;		//0x0	=0
		unsigned int unk_4;		//0x4	=this_4
		unsigned int unk_8;		//0x8	=~this_4
	};//sizeof = 0xC

	struct SimpleFrame_DC {
		SimpleFrame_DC_Element elements[5];
	};//sizeof = 3C

	struct FONTHASHOBJ {
		void** vtable;		//0x0
		//...
	};

	struct TEXTBLOCK {
		void** vtable;		//0x0
		//...
	};

	struct FRAMENODE {
		void** vtable;
	};

	struct CPreselectUI {
		void** vtable;						//=0x9409E0
		unsigned int unk_4;//always 0 ?
		CUnit* unit;						//指向所属单位
		CStatBar* statBarHP;					//血条
	};//sizeof = 0x10

	struct CLayoutFrame {
		void** vtable;
		unsigned int					pointCount;			//0x4	=9					LayoutFrame
		unsigned int					pTL;				//0x8	=0					LayoutFrame
		unsigned int					pTC;				//0xC	=0					LayoutFrame
		unsigned int					pTR;				//0x10	=0					LayoutFrame
		unsigned int					pL;					//0x14	=0					LayoutFrame
		unsigned int					pC;					//0x18	=0					LayoutFrame
		unsigned int					pR;					//0x1C	=0					LayoutFrame
		unsigned int					pBL;				//0x20	=0					LayoutFrame
		unsigned int					pBC;				//0x24	=0					LayoutFrame
		unsigned int					pBR;				//0x28	=0					LayoutFrame
		unsigned int					unk_2C;				//0x2C	=0					LayoutFrame
		unsigned int					unk_30;				//0x30	=0					LayoutFrame
		unsigned int					unk_34;				//0x34	=0					LayoutFrame
		unsigned int					unk_38;				//0x38	=0					LayoutFrame
		unsigned int					unk_3C;				//0x3C	=this_3C			LayoutFrame	指向自己的指针
		FRAMENODE* unk_40;				//0x40	=~this_3C			LayoutFrame	参考6F605C70
		float						borderB;			//0x44	=0.0				LayoutFrame
		float						borderL;			//0x48	=0.0				LayoutFrame
		float						borderU;			//0x4C	=0.0				LayoutFrame	
		float						borderR;			//0x50	=0.0				LayoutFrame
		unsigned int					unk_54;				//0x54						LayoutFrame 坐标改变时改变
		float						width;				//0x58	=0.0				LayoutFrame	内容改变时改变
		float						height;				//0x5C	=0.0				LayoutFrame 内容改变时改变
		float						scale;				//0x60	=0.0				LayoutFrame 整体缩放比例
		void* unk_64;				//0x64						LayoutFrame	SimpleFrame void (__thiscall *v4)(_DWORD); 
	};//sizeof = 0x68

	struct CSimpleRegion {
		CLayoutFrame /*0x68 bytes*/	baseLayoutFrame;	//0x0						
		unsigned char						colorAlpha;			//0x68
		unsigned char						colorBlue;			//0x69
		unsigned char						colorGreen;			//0x6A
		unsigned char						colorRed;			//0x6B
		unsigned char						unk_6C[4];			//0x6C	
		unsigned int					unk_70;				//0x70	0
		CLayoutFrame				parent;				//0x74	0
		unsigned int					unk_78;				//0x78	0
		unsigned int					unk_7C;				//0x7C	0
		float						unk_80;				//0x80	1.0

	};//sizeof = 0x80

	struct CSimpleFrame {
		CLayoutFrame /*0x68 bytes*/	baseLayoutFrame;	//0x0						LayoutFrame
		CSimpleTop* unk_68;				//0x68	=*6FACE758			SimpleFrame
		CLayoutFrame* parent;				//0x6C	=0					SimpleFrame
		unsigned int					unk_70;				//0x70	=0					SimpleFrame
		unsigned int					unk_74;				//0x74	=0					SimpleFrame
		unsigned int					unk_78;				//0x78	=0					SimpleFrame
		unsigned int					unk_7C;				//0x7C	=0					SimpleFrame	Storm#403 删除
		unsigned int					unk_80;				//0x80	=0					SimpleFrame
		unsigned int					unk_84;				//0x84  =0					CToolTipWar3 初始化在6F609980
		unsigned int					unk_88;				//0x88	=0xFF000000			SimpleFrame 只初始化了第一个BYTE，是BYTE变量？
		unsigned int					unk_8C;				//0x8C	=0					SimpleFrame	flags?
		unsigned int					visible;			//0x90	=0,1				SimpleFrame	ToolTip是0	SimpleFrame是1 显示时是1，隐藏时是0
		unsigned int					needUpdate;			//0x94	=1 bool?			SimpleFrame	在6F609AD0被使用运行完归0	   显示时是1，隐藏时是0
		float						unk_98;				//0x98	=0.0				SimpleFrame
		float						unk_9C;				//0x9C	=0.0				SimpleFrame
		float						unk_A0;				//0xA0	=0.0				SimpleFrame
		float						unk_A4;				//0xA4	=0.0				SimpleFrame
		float						unk_A8;				//0xA8	=0.0				SimpleFrame
		float						unk_AC;				//0xAC	=0.0				SimpleFrame
		float						unk_B0;				//0xB0	=0.0				SimpleFrame
		float						unk_B4;				//0xB4	=0.0				SimpleFrame
		unsigned int					unk_B8;				//0xB8	=1					SimpleFrame
		unsigned int					unk_BC;				//0xBC	=1					SimpleFrame
		unsigned int					unk_C0;				//0xC0	=1					SimpleFrame
		unsigned int					unk_C4;				//0xC4	=1					SimpleFrame
		unsigned int					unk_C8;				//0xC8	=0					SimpleFrame
		SimpleFrameTextureSettings* textureSettings;	//0xCC	=0					SimpleFrame, CToolTipWar3
		unsigned int					unk_D0;				//0xD0	=0					SimpleFrame
		unsigned int					unk_D4;				//0xD4	=this_D4			SimpleFrame 指向自己的指针
		unsigned int					unk_D8;				//0xD8	=~this_D4			SimpleFrame
		SimpleFrame_DC				unk_DC;				//0xDC						SimpleFrame 初始化过程见 6F609D14
		unsigned int					unk_118;			//0x118	=0					SimpleFrame
		unsigned int					unk_11C;			//0x11C	=this_11C			SimpleFrame 指向自己的指针
		unsigned int					unk_120;			//0x120	=~this_11C			SimpleFrame
	};//sizeof = 0x124

	struct CIconCover {
		CSimpleFrame/*0x124 bytes*/	baseSimpleFrame;
		char						infoTextBuffer[0x904];		//0x124
	};//sizeof = 0xA30 ? 0xA28?

	struct CSimpleButton {
		CSimpleFrame/*0x124 bytes*/	baseSimpleFrame;	//0x0
		unsigned char* clickEventObserver;	//0x124 CGameUI*?
		unsigned int					clickEventId;		//0x128
		unsigned char* mouseEventObserver;	//0x12C	6F603060 CGameUI*?
		unsigned int					mouseOverEventId;	//0x130
		unsigned int					mouseOutEventId;	//0x134
		unsigned int					currentState;		//0x138
		unsigned int					enabled;			//0x13C
		unsigned int					mouseButtonFlags;	//0x140	=0x10	指定什么鼠标键可以触发按钮
		unsigned int					unk_144;			//0x144
		unsigned int					unk_148;			//0x148
		unsigned int					unk_14C;			//0x14C
		float						unk_150;			//0x150 =0.001
		float						unk_154;			//0x154 =-0.001
		CSimpleTexture* textureDisabled;	//0x158
		CSimpleTexture* textureEnabled;		//0x15C
		CSimpleTexture* texturePushed;		//0x160
		CSimpleTexture* currentTexture;		//0x164
	};//sizeof = 0x168

	struct CSimpleCheckbox {
		CSimpleButton				baseSimpleButton;	//0x0
		unsigned int					isChecked;
		CSimpleTexture* checkBoxTexture;
	};//sizeof = 0x170

	struct CCommandButtonData {
		void** vtable;			//0x0
		unsigned int	abilityId;		//0x4 物品按钮和基本技能这里是0
		unsigned int	orderId_8;		//0x8
		unsigned int	orderId_C;		//0xC
		unsigned int	flag10;			//0x10
		unsigned int	unk14;			//0x14
		unsigned int	unk18;			//0x18
		HashGroup	agentHash;		//0x1C
		unsigned int	unk24;			//0x24
		unsigned int	isSpell;		//0x28
		char		title[0x100];	//0x2C 长度正确？
		unsigned char		unk12C[0x58];	//0x12C
		unsigned int	unk184;			//0x184
		unsigned int	unk188;			//0x188
		char		tooltip[0x400];	//0x18C 长度正确？
		unsigned char		unk58C[0x8];	//0x58C
		unsigned int	manaCost;		//0x594
		unsigned int	unk598;			//0x598
		unsigned int	displayOrder;	//0x59C ?
		unsigned int	unk5A0;			//0x5A0 英雄技能是2 物品是0
		unsigned int	unk5A4;			//0x5A4 英雄技能是1 物品是0
		unsigned int	unk5A8;			//0x5A8 英雄技能是1 物品是0
		unsigned int	hotkey;			//0x5AC
		unsigned int	unk5B0;			//0x5B0
		unsigned int	unk5B4;			//0x5B4
		unsigned int	unk5B8;			//0x5B8 英雄技能是1 物品是0
		unsigned int	unk5BC;			//0x5BC 英雄技能是1 物品是0
		unsigned int	unk5C0;			//0x5C0 英雄技能是0 物品是1
		unsigned int	unk5C4;			//0x5C4 英雄技能是0 物品是1
		char		iconPath[0x100];//0x5C8 长度正确？
		unsigned int	unk6C8;			//0x6C8
		unsigned int	unk6CC;			//0x6CC
		unsigned int	unk6D0;			//0x6D0
		unsigned char* ability;		//0x6D4 基本技能(例如move)这里是NULL
	};//sizeof = 0x6D8

	//需要搞清楚CShrinkingButton (可能只有虚函数)
	struct CCommandButton {
		CSimpleButton				baseSimpleButton;	//0x0
		unsigned char unk168[0x28];
		CCommandButtonData* commandButtonData;		//0x190
		unsigned char unk194[0x2C];
	};//sizeof = 0x1C0

	struct CHeroBarButton {//FIXME 同时也是闲农民按键? 研究peonbar
		CCommandButton				baseCommandButton;	//0x0
		unsigned int unk1C0;
		CUnit* hero;				//0x1C4
		unsigned int					heroTypeId;			//0x1C8
		unsigned char unk1CC[0x44];
	};//sizeof = 0x210

	struct CResourceCover {//TODO 寻找与ResourceBar关系
		void** vtable;						// = 0x93BC6C
		//...
	};

	struct CSimpleStatusBar {
		CSimpleFrame				baseSimpleFrame;	//0x0
		unsigned int					valueChanged;		//0x124
		float						valueMin;			//0x128
		float						valueMax;			//0x12C = 100.0
		float						value;				//0x130 = 50.0
		CSimpleTexture* texture;			//0x134
	};//sizeof = 0x138

	struct CSimpleFontString {
		CSimpleRegion				baseSimpleRegion;	//0x0
		unsigned int					unk_84;				//0x84	=0					
		unsigned int					unk_88;				//0x88	=0					
		FONTHASHOBJ* unk_8C;				//0x8C	=0					
		float						fontHeight;			//0x90	=0					?? = fontHeight * scale
		unsigned int					unk_94;				//0x94	=0					
		unsigned int					unk_98;				//0x98	=0					
		const char* text;				//0x9C	=0					flag?
		TEXTBLOCK* textBlock;			//0xA0	=0					
		float						unk_A4;				//0xA4	=0.0				
		float						unk_A8;				//0xA8	=0.0				
		unsigned int					unk_AC;				//0xAC	=0xFF000000			
		float						unk_B0;				//0xB0	=0.0				
		float						unk_B4;				//0xB4	=0.0				
		float						unk_B8;				//0xB8	=0.0				?? = 0xB8 * scale + borderL
		float						unk_BC;				//0xBC	=0.0				?? = 0xBC * scale + borderB
		unsigned int					flags;				//0xC0	=0x212				
	};//sizeof = 0xC4

	struct CStatBar {
		CSimpleStatusBar			baseSimpleStatusBar;//0x0
		unsigned int unk_138;								//0x138 type
		unsigned int unk_13C;
		CSimpleFontString* fontString;			//0x140	XP bar text
		CSimpleTexture* texture;			//0x144
		unsigned int unk_148;
		unsigned int unk_14C;
		float unk_150;									//疑似血条偏移程度?
		CUnit* owner;				//0x154
	};//sizeof = 0x158

	struct CSimpleTexture {
		CSimpleRegion				baseSimpleRegion;	//0x0
		unsigned int					unk_84;				//0x84
		unsigned int					unk_88;				//0x88
		unsigned int					unk_8C;				//0x8C
		unsigned int					unk_90;				//0x90
		float						unk_94;				//0x94
		float						unk_98;				//0x98
		float						unk_9C;				//0x9C
		float						unk_A0;				//0xA0
		float						unk_A4;				//0xA4
		float						unk_A8;				//0xA8
		float						unk_AC;				//0xAC
		float						unk_B0;				//0xB0
		float						unk_B4;				//0xB4
		float						unk_B8;				//0xB8
		float						unk_BC;				//0xBC
		float						unk_C0;				//0xC0
		float						unk_C4;				//0xC4
		float						unk_C8;				//0xC8
		float						unk_CC;				//0xCC
		float						unk_D0;				//0xD0
		float						unk_D4;				//0xD4
		float						unk_D8;				//0xD8
		float						unk_DC;				//0xDC
		float						unk_E0;				//0xE0
		unsigned int					unk_E4;				//0xE4
	};//sizeof = 0xE8

	struct CBuffBar {
		void** vtable;
	};//sizeof = 0x160

	struct SimpleFrameTextureSettings {
		CSimpleTexture* textureBackground;		//0x0
		CSimpleTexture* textureLeft;			//0x4
		CSimpleTexture* textureRight;			//0x8
		CSimpleTexture* textureTop;				//0xC
		CSimpleTexture* textureBottom;			//0x10
		RCString			RCS_bgTexturePath;		//0x14	其实是RCStaticString，只是vtable不同	背景资源路径
		RCString			RCS_borderTexturePath;	//0x20	其实是RCStaticString，只是vtable不同	边框资源路径
		unsigned int			borderFlags;			//0x2C	
		unsigned int			unk_30;					//0x30
		unsigned int			unk_34;					//0x34
		float				borderWidth;			//0x38	0.025	UnitTip=0.01
		float				unk_3C;					//0x3C	0.0
		float				paddingTop;				//0x40	0.0		UnitTip = 0.0019
		float				paddingBottom;			//0x44	0.0		^
		float				paddingLeft;			//0x48	0.0		^
		float				paddingRight;			//0x4C	0.0		^
	};//sizeof = 0x50

	struct CToolTipWar3 {
		CSimpleFrame			baseSimpleFrame;	//0x0						CSimpleFrame
		void* parentObject;		//0x124
		CSimpleFontString* fontStr_128;		//0x128						CToolTipWar3		
		CSimpleFontString* fontStr_12C;		//0x12C						CToolTipWar3	
		CSimpleFontString* fontStr_130;		//0x130						CToolTipWar3
		CSimpleFontString* fontStr_134;		//0x134						CToolTipWar3
		CSimpleFontString* fontStr_138;		//0x138						CToolTipWar3
		CSimpleFontString* fontStr_13C;		//0x13C						CToolTipWar3
		CSimpleTexture* texture_140;		//0x140						CToolTipWar3
		CSimpleTexture* texture_144;		//0x144						CToolTipWar3
		CSimpleTexture* texture_148;		//0x148						CToolTipWar3
		CSimpleTexture* texture_14C;		//0x14C						CToolTipWar3
		CBuffBar* buffBar;			//0x150	=0					CToolTipWar3
		unsigned int				unk_154;			//0x154	=4					CToolTipWar3
		unsigned int				unk_158;			//0x158	=4					CToolTipWar3
		unsigned int				unk_15C;			//0x15C	=4					CToolTipWar3
		unsigned int				unk_160;			//0x160	=4					CToolTipWar3
		unsigned int				unk_164;			//0x164	=0					CToolTipWar3
		unsigned int				unk_168;			//0x168	=0					CToolTipWar3
		unsigned int				unk_16C;			//0x16C	=0					CToolTipWar3
		unsigned int				unk_170;			//0x170	=0					CToolTipWar3
		unsigned int				unk_174;			//0x174 =0
		unsigned int				unk_17C;			//0x17C =1
	};

	struct CUnitTip {
		CSimpleFrame			baseSimpleFrame;	//0x0						CSimpleFrame
		CUnit* unit;				//0x124						CUnitTip	6F2C9670
		SimpleFrameTextureSettings* resData2;			//0x128						CUnitTip
		CSimpleFontString* fontStr_12C;		//0x12C						CUnitTip	单位类型名
		CSimpleFontString* fontStr_130;		//0x130						CUnitTip	等级
		CSimpleFontString* fontStr_134;		//0x134						CUnitTip	玩家名
		unsigned int				unk_138;			//0x138						单位名更新了要设为1
		unsigned int				updateSize;			//0x13C						单位等级回调更新了要设为1
		unsigned int				unk_140;			//0x140						玩家名更新了要设为1
		unsigned char					unk_144[0x8];		//0x144
		void* levelUpdateFunc;	//0x14C						6F35B740 返回unit
	}; //sizeof = 0x150

	struct CLayer {
		CObserver				baseObserver;
		unsigned int				unk_C;				//0xC	3
		unsigned int				unk_10;				//0x10	4
		unsigned int				unk_14;				//0x14	5
		unsigned int				unk_18;				//0x18	6 = offset unk_18
		unsigned int				unk_1C;				//0x1C	7 = ~offset unk_18
		unsigned int				unk_20;				//0x20	8 
		unsigned int				unk_24;				//0x24	9
		unsigned int				unk_28;				//0x28	10
		unsigned int				unk_2C;				//0x2C	11
		unsigned int				unk_30;				//0x30	12
		unsigned int				unk_34;				//0x34	13
		unsigned int				unk_38;				//0x38	14
		unsigned int				unk_3C;				//0x3C	15
		unsigned int				unk_40;				//0x40	16
		unsigned int				unk_44;				//0x44	17
		unsigned char					unk_48[0x68];		//0x48
		unsigned int				flag_B0;			//0xB0
	}; //sizeof = 0xB4

	//Frame的研究：
	//float	bottom;		//0xF8
	//float	left;		//0xFC
	//float	top;		//0x100
	//float	right;		//0x104
	//bool	visible;	//0x108

	struct CFrame {
		CLayer					baseLayer;
		CLayer					baseLayer_B4;
	}; //sizeof = 0x168

	struct CScreenFrame {
		CFrame					baseFrame;			//0x0
		unsigned int				unk_168;			//0x168
		unsigned int				unk_16C;			//0x16C
		unsigned int				vector_170_0;		//0x170
		unsigned int				vector_170_4;		//0x174
		unsigned int				vector_170_8;		//0x178
		unsigned int				vector_170_C;		//0x17C
	};//sizeof = 180

	struct CGlueMgr {
		CScreenFrame			baseScreenFrame;	//0x0
		unsigned int				unk_180;			//0x180 = 17
		void* currentFrame;		//0x184
		unsigned int				unk_188;			//0x188
		unsigned char					unk_18C[0x4];		//0x18C
		unsigned int				unk_190;			//0x190
		unsigned int				unk_194;			//0x194
		unsigned int				unk_19C;			//0x19C
		unsigned int				unk_1A0;			//0x1A0 = 17
		unsigned char					unk_1A4[0x4];		//0x1A4
		unsigned int				unk_1A8;			//0x1A8
	};//sizeof = 1AC

	struct FrameNameHashNode {
		unsigned int				hashKey;
		unsigned int				unk_4;
		unsigned int				unk_8;
		unsigned int				unk_C;
		unsigned int				unk_10;
		char* frameName;
		void* initFunction;
		unsigned int				frameId;
	};

	struct TSHashTable_UFRAMENAMECREATEHANDLER {
		void** vtable;
	};

	//TODO SimpleGrid
	struct CCommandBar {
		CSimpleFrame			baseSimpleFrame;

		//...
	}; //sizeof = 0x1C0

	//--------------------------------------------------------------------------------
	// Game UI
	//--------------------------------------------------------------------------------
	//TODO
	//0xACE66C: 游戏外为GlueMgr*，游戏内为GameUI*
	//0xACE758: SimpleTop *, +0x8AC = firstbutton, +0x8B4 = count(?)


	//游戏内UI
	//以下BorderB/L/U/R为屏幕2d值, 范围[0,1], 屏幕2d原点为左下角

	struct CGameUI {
		CScreenFrame				baseScreenFrame;			//0x0
		unsigned char						unk_180[0x2C];				//0x180
		unsigned int					flag_1AC;					//0x1AC
		unsigned int					flag_1B0;					//0x1B0
		unsigned char						unk_1B4[0x18];				//0x1B4
		unsigned char* uberToolTip;				//0x1CC
		unsigned char						unk_1D0[0x84];				//0x1D0
		CCameraWar3* camera;						//0x254
		unsigned int					flags_258;					//0x258
		unsigned char						unk_25C[0x160];				//0x25C
		CWorldFrameWar3* world;						//0x3BC

		unsigned char* minimap;
		unsigned char* infobar;

		CCommandBar* commandBar;					//0x3C8
		CResourceCover* resourceBar;				//0x3CC
		unsigned char* unkUpperButton3D0;			//0x3D0 CUpperButtonBar*
		CSimpleFrame* minimapScreenFrame;			//0x3D4
		CSimpleButton* unkButton3D8;				//0x3D8 CSimpleButton*
		unsigned char* unkHeroBar3DC;				//0x3DC CHeroBar*
		unsigned char* idlePeonBar;				//0x3E0 CPeonBar*
		unsigned char* unkSimpleMessageFrame3E4;	//0x3E4 CSimpleMessageFrame*
		unsigned char* unkSimpleMessageFrame3E8;	//0x3E8 CSimpleMessageFrame*
		unsigned char* unkSimpleMessageFrame3EC;	//0x3EC CSimpleMessageFrame*
		unsigned char* unkSimpleMessageFrame3F0;	//0x3F0 CSimpleMessageFrame*
		unsigned char* portraitButton;				//0x3F4 CPortraitButton*
		unsigned char* timeOfDayIndicator;			//0x3F8 CTimeOfDayIndicator*

		CChatEditBar* chatEditBar;				//0x3FC
		unsigned char						unk_400[0x8];				//0x400	CCinematicPanel* 
		CSimpleButton* minimapSignalButton;		//0x408
		CSimpleButton* minimapTerrainButton;		//0x40C
		CSimpleButton* minimapCreepButton;			//0x410
		CSimpleButton* minimapAllyButton;			//0x414
		CSimpleButton* minimapFormationButton;		//0x418
		unsigned char						unk_41C[0xC];
		CSimpleConsole* simpleConsole;				//0x428
		//...
	};//sizeof = 0x454

	struct CSpriteFrame_180 {
		unsigned int				unk_0;				//0x0
		unsigned int				unk_4;				//0x4
		unsigned int				unk_8;				//0x8
		unsigned int				unk_C;				//0xC
		unsigned int				unk_10;				//0x10
		unsigned int				unk_14;				//0x14
		unsigned int				unk_18;				//0x18 = 64
	}; //sizeof = 0x1C

	struct CSpriteFrame {
		CFrame					baseFrame;			//0x0
		float					unk_168;			//0x168
		float					unk_16C;			//0x16C
		unsigned int				unk_170;			//0x170
		unsigned int				unk_174;			//0x174
		unsigned int				unk_178;			//0x178
		unsigned int				unk_17C;			//0x17C
		CSpriteFrame_180		struct_180;			//0x180
		float					unk_19C;			//0x19C = 1.0
		unsigned int				unk_1A0;			//0x1A0
		unsigned int				unk_1A4;			//0x1A4
		float					unk_1A8;			//0x1A8 = 0.5
		unsigned int				unk_1AC;			//0x1AC = 1
	}; //sizeof = 0x1B0

	struct CModelFrame {
		CFrame					baseFrame;			//0x0
		unsigned int				unk_16C;			//0x16C
		unsigned int				unk_170;			//0x170
		unsigned int				unk_174;			//0x174
	}; //sizeof = 0x178

	struct CBackDropFrame {
		CModelFrame				baseModelFrame;		//0x0
		unsigned int				unk_178;			//0x178
		unsigned int				unk_17C;			//0x17C
		unsigned int				unk_180;			//0x180
		unsigned int				unk_184;			//0x184
		unsigned int				unk_188;			//0x188
		unsigned int				unk_18C;			//0x18C
		unsigned int				unk_190;			//0x190
		unsigned char					unk_194[0x24];		//0x194
		float					unk_1B8;			//0x1B8 = 0.025
		float					unk_1BC;			//0x1BC
		unsigned int				unk_1C0;			//0x1C0
		unsigned int				unk_1C4;			//0x1C4
		float					unk_1C8;			//0x1C8
		float					unk_1CC;			//0x1CC
		float					unk_1D0;			//0x1D0
		float					unk_1D4;			//0x1D4	
		unsigned char					unk_1D8[0x4];		//0x1D8
	}; //sizeof = 0x1DC

	struct CHighlightFrame {
		CFrame					baseFrame;			//0x0
		unsigned int				unk_168;			//0x168 6F624080
		unsigned int				unk_16C;			//0x16C
		unsigned int				unk_170;			//0x170 = 1
		float					unk_174;			//0x174
		float					unk_178;			//0x178
		float					unk_17C;			//0x17C
		float					unk_180;			//0x180
		unsigned int				unk_184;			//0x184
		char					texturePath[0x104];	//0x188 
		unsigned int				unk_28C;			//0x28C = 0xFFFFFFFF
		float					unk_290;			//0x290
		unsigned int				unk_294;			//0x294 = 2
		float					unk_298;			//0x298
		float					unk_29C;			//0x29C = 0.01
		unsigned int				unk_2A0;			//0x2A0 = 2
	}; //sizeof = 0x2A4

	struct CControl {
		CSpriteFrame			baseSprintFrame;	//0x0
		unsigned int				unk_1B0;			//0x1B0
		CHighlightFrame* highlightFrame;		//0x1B4
		unsigned int				unk_1B8;			//0x1B8
		CBackDropFrame* unk_1BC;			//0x1BC
		CBackDropFrame* unk_1C0;			//0x1C0
		CBackDropFrame* unk_1C4;			//0x1C4
		CBackDropFrame* unk_1C8;			//0x1C8
		unsigned int				unk_1CC;			//0x1CC 初始化6F601D60
		unsigned int				unk_1D0;			//0x1D0 初始化6F601D60
		unsigned int				unk_1D4;			//0x1D4 初始化6F601DE0
		unsigned int				unk_1D8;			//0x1D8	初始化6F601DE0
		unsigned int				unk_1DC;			//0x1DC
		unsigned int				unk_1E0;			//0x1E0 = 1
	}; //sizeof = 0x1E4

	struct TextFrameShadowSettings {
		unsigned int				unk_0;				//0x0
		unsigned int				unk_4;				//0x4
	};

	struct TextFrameFontSettings {
		float					unk_0;				//0x0 +
		float					unk_4;				//0x4 -
	};

	struct CTextFrame {
		CControl					baseControl;			//0x0
		unsigned int					textLength;				//0x1E4
		char* text;					//0x1E8
		unsigned int					unk_1EC;				//0x1EC
		TextFrameShadowSettings* shadowSettings;			//0x1F0
		unsigned int					color;					//0x1F4 = 0xFFFFFFFF ARGB
		unsigned int					unk_1F8;				//0x1F8 = 0xFFFFFFFF
		unsigned int					unk_1FC;				//0x1FC = 0xFFFFFFFF
		unsigned int					unk_200;				//0x200 = 0xFF000000
		unsigned int					unk_204;				//0x204
		float						unk_208;				//0x208
		float						unk_20C;				//0x20C
		TextFrameFontSettings		fontSettings;			//0x210
		float						unk_218;				//0x218
		unsigned int					unk_21C;				//0x21C
		unsigned int					unk_220;				//0x220
		unsigned int					unk_224;				//0x224 = 1
		unsigned int					unk_228;				//0x228
		unsigned int					unk_22C;				//0x22C = 1
		unsigned int					unk_230;				//0x230 = 1
		unsigned char						unk_234[0x8];			//0x234 
		unsigned int					needUpdate;				//0x23C
	}; //sizeof = 0x240

	struct CButtonFrame {
		CControl				baseControl;			//0x0
	}; //sizeof = 0x1E4

	struct CGlueButtonWar3 {
		CButtonFrame			baseButtonFrame;		//0x0
		unsigned int				unk_1E4;				//0x1E4
	}; //sizeof = 0x1E8


	typedef void* (__fastcall* WAR3WNDPROC)(void* hwnd, unsigned int uMsg, void* wParam, void* lParam);

	typedef unsigned int HASHKEY_4CC;

	struct UnitDataNode {
		unsigned int	unk_0;			//0x0
		unsigned char		unk_4[0x10];	//0x4
		unsigned int	typeId;			//0x14
		int			buildTime;		//0x18
		unsigned int	unk_1C;			//0x1C
		int			goldCost;		//0x20
		int			woodCost;		//0x24
		unsigned char		unk_28[0x24C];	//0x28
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

	struct ItemDataNode {
		unsigned char	unk_0[0xA8];			//0x0
	};//sizeof = 0xA8

	//TODO 全部调用虚函数，而不是直接修改值。
	struct GxDeviceVTable {
		void* unk0;						//0x0
		void* unk4[8];					//0x4
		void* (*GetDeviceWindow)(void);	//0x24
	};

	struct CGxDevice {
		GxDeviceVTable* vtable;
	};

	struct CGxDeviceD3d {
		void** vtable;				//0x0
		unsigned char			unk_4[0x570];		//0x4
		void* hwnd;				//0x574
		WAR3WNDPROC		wndProc;			//0x578
		unsigned char			unk_57C[0x144];		//0x57C
		float			unk_6C0;			//0x6C0
		float			unk_6C4;			//0x6C4
		float			windowHeight;		//0x6C8
		float			windowWidth;		//0x6CC
		unsigned int		unk_6D0;			//0x6D0 窗口大小改变，这里设为1
		//..
	};

	struct CGxDeviceOpenGl {
		void** vtable;				//0x0
		unsigned char			unk_4[0x570];		//0x4
		void* hwnd;				//0x574
		WAR3WNDPROC		wndProc;			//0x578
		unsigned char	unk_57C[0x144];		//0x57C
		float			unk_6C0;			//0x6C0
		float			unk_6C4;			//0x6C4
		float			windowHeight;		//0x6C8
		float			windowWidth;		//0x6CC
		unsigned int		unk_6D0;			//0x6D0 窗口大小改变，这里设为1
	};

	struct ColorARGB {
		unsigned char b;
		unsigned char g;
		unsigned char r;
		unsigned char a;
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
		unsigned char					unk_4[0x2C4];	//0x4
		void* asdf;
	};//sizeof?

	struct CPlayerWar3 {
		void** vtable;
		unsigned char				unk_04[0x2C];
		unsigned short			slot;			//+30
		unsigned char				unk_32[0x02];
		CSelectionWar3* selection;		//+34
		unsigned char				unk_38[0x228];
		unsigned int			race;			//+260
		unsigned int			color;			//+264
		unsigned char				unk_268[0x10];
		unsigned int			team;			//+278
		unsigned char				unk_27C[0x54];
		unsigned char* techtree;		//+2D0
	};//sizeof?

	struct CGameWar3 {
		void** vtable;				//0x0
		unsigned int		unk_4;				//0x4
		unsigned int		jassStringId;		//0x8
		unsigned char			unk_C[0x1C];		//0xC
		unsigned short		localPlayerSlot;	//0x28
		unsigned char			unk_2A[0x1A];		//0x2A
		unsigned int		maxPlayers;			//+44 - Max player slots on the map.
		unsigned char			unk_48[0x04];
		unsigned int		activePlayers;		//+4C - Active players.
		unsigned char			unk_50[0x08];
		CPlayerWar3* players[16];		//+58
	};//sizeof?

	struct CSelectionWar3 {
		void** vtable;				//0x0
		unsigned char				unk_04[0x10];
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
		unsigned char unk_4[0x10];
	};//sizeof = 0x14

	struct CAgent {
		void** vtable;
		//...
	};

	struct CWidget {
		void** vtable;
		unsigned char			unk_04[0x8];
		HashGroup		hash;				//0xC
		//...
	};

	struct CUnit_174_30C { //1.24d之前
		unsigned char unk_174[0x64];
		HashGroupRaw				abilityHash;		//0x1DC	//以下偏移为按照1.24d以后
		unsigned char unk_1E4[0x44];
		float						impactZ;			//0x228 Projectile impact Z
		float						impactSwimZ;		//0x22C	Projectile impact Z (swimming)
		float						launchX;			//0x230	Projectile launch X
		float						launchY;			//0x234	Projectile launch Y
		float						launchZ;			//0x238 Projectile launch Z
		float						launchSwimZ;		//0x23C Projectile launch Z (swimming)
		unsigned char unk_240[0x8];
		unsigned int					unitTypeFlag;		//0x248
		unsigned char						unk_24C[0x38];		//0x24C
		float						x;					//0x284
		float						y;					//0x288
		unsigned char						unk_28C[0x48];		//0x28C
		ColorARGB					vertexColor;		//0x2D4	0xAARRGGBB
		unsigned char						unk_2D8[0x38];
	};

	struct CUnit_174_310 { //1.24d和之后
		unsigned char unk_174[0x68];
		HashGroupRaw				abilityHash;		//0x1DC	//以下偏移为按照1.24d以后
		unsigned char unk_1E4[0x44];
		float						impactZ;			//0x228 Projectile impact Z
		float						impactSwimZ;		//0x22C	Projectile impact Z (swimming)
		float						launchX;			//0x230	Projectile launch X
		float						launchY;			//0x234	Projectile launch Y
		float						launchZ;			//0x238 Projectile launch Z
		float						launchSwimZ;		//0x23C Projectile launch Z (swimming)
		unsigned char unk_240[0x8];
		unsigned int					unitTypeFlag;		//0x248
		unsigned char						unk_24C[0x38];		//0x24C
		float						x;					//0x284
		float						y;					//0x288
		unsigned char						unk_28C[0x48];		//0x28C
		ColorARGB					vertexColor;		//0x2D4	0xAARRGGBB
		unsigned char						unk_2D8[0x38];
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
		unsigned char unk_14[0xC];
		unsigned int					stateFlag;			//0x20 表示状态, 1 << 3 表示无敌 TODO 继续研究其他bit
		unsigned char unk_24[0xC];
		unsigned int					typeId;				//0x30
		unsigned char unk_34[0x1C];
		CPreselectUI* preSelectUI;		//0x50
		float						unk_54;				//0x54
		unsigned int					owner;				//0x58
		unsigned char unk_5C[0x44];
		HashGroup					hash_unkA0;			//0xA0
		unsigned char	unk_A8[0x38];
		float						defense;			//0xE0
		unsigned int					defenseType;		//0xE4
		unsigned char unk_E8[0x84];
		HashGroup					hash_unk16C;		//0x16C
		CUnit_174					unit_174;			//0x174
		/*
		unsigned char unk_174[0x68];//在这一段从1.24d? 加了4字节，< 1.24d为0x64

		//0x19C,0x1A0与当前OrderTarg有关

		HashGroup					abilityHash;		//0x1DC	//以下偏移为按照1.24d以后
		unsigned char unk_1E4[0x44];
		//TODO 0x194 = current action ?
		float						impactZ;			//0x228 Projectile impact Z
		float						impactSwimZ;		//0x22C	Projectile impact Z (swimming)
		float						launchX;			//0x230	Projectile launch X
		float						launchY;			//0x234	Projectile launch Y
		float						launchZ;			//0x238 Projectile launch Z
		float						launchSwimZ;		//0x23C Projectile launch Z (swimming)
		unsigned char unk_240[0x8];
		unsigned int					unitTypeFlag;		//0x248
		unsigned char						unk_24C[0x38];		//0x24C
		float						x;					//0x284
		float						y;					//0x288
		unsigned char						unk_28C[0x48];		//0x28C
		ColorARGB					vertexColor;		//0x2D4	0xAARRGGBB
		unsigned char						unk_2D8[0x38];
		*/
	};//sizeof = 0x30C ~ 0x310

	struct CItem {
		void** vtable;
		unsigned char			unk_04[0x8];
		HashGroup		hash;				//0xC
		unsigned char			unk_14[0x1C];
		unsigned int		typeId;				//0x30
		unsigned char			unk_34[0x28];		//0x34


		unsigned int		unkFlag_5C;			//0x5C

		unsigned char			unk_60[0x28];		//0x60

		unsigned int		firstAbilityId;		//0x88
		HashGroup		hashOwner;			//0x8C
		unsigned char			unk_94[8];
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
		unsigned char		unk_4[0x7C];	//0x4
		unsigned int	hotkey_levels;	//0x80
		unsigned int* hotkey_items;	//0x84
		unsigned int	unk_88;			//0x88
		unsigned int	hotkey2_levels;	//0x8C
		unsigned int* hotkey2_items;	//0x90
		unsigned char		unk_94[0xA0];	//0x94
		unsigned int	tooltip_levels;	//0x134
		char** tooltip_items;	//0x138
		unsigned int	unk_13C;		//0x13C
		unsigned int	tooltip2_levels;//0x140
		char** tooltip2_items;	//0x144		
		unsigned char		unk_148[0x10];	//0x148
		unsigned int	desc_levels;	//0x158
		char** desc_items;		//0x15C
		unsigned int	unk_160;		//0x160
		unsigned int	desc2_levels;	//0x164
		char** desc2_items;	//0x168
		//...
	};

	struct AbilityDefData {
		void** vtable;
		unsigned char	unk_4[0x24]; //0x4
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
		unsigned char unk44[0x8];				//0x44
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
		unsigned char	unk_6C[0x8];
		FloatMini		buildTimeTotal;	//0x74
		unsigned char unk_7C[0x8];
		HashGroup		hashPoReal;		//0x84
		//...
	};//sizeof = ?

	struct CAbilityUpgradeInProgress
	{
		CAbility		baseAbility;
		unsigned char	unk_6C[0x8];
		FloatMini		buildTimeTotal;	//0x74
		unsigned char unk_7C[0x8];
		HashGroup		hashPoReal;		//0x84
		unsigned char unk_8C[0x1C];
		unsigned int		upgradeTypeId;	//0xA8
		//...
	};

	struct CAbilityQueue
	{
		CAbility		baseAbility;
		unsigned char unk_6C[0x8];
		FloatMini		queueTimeTotal;	//0x74
		unsigned char unk_7C[0x8];
		HashGroup		hashPoReal;		//0x84
		unsigned char unk_8C[0x14];
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
		unsigned char unk_70[0x14];
		unsigned int			dice_weap0;		//0x88
		unsigned int			dice_weap1;		//0x8C
		unsigned char unk_90[0x4];
		unsigned int			sides_weap0;	//0x94
		unsigned int			sides_weap1;	//0x98
		unsigned char unk_9C[0x4];
		unsigned int			dmgPlus_weap0;	//0xA0
		unsigned int			dmgPlus_weap1;	//0xA4
		unsigned char unk_A8[0x4];
		unsigned int			buffs_weap0;	//0xAC
		unsigned int			buffs_weap1;	//0xB0
		unsigned char unk_B4[0x28];
		unsigned int			weapTp_weap0;	//0xDC
		unsigned int			weapTp_weap1;	//0xE0
		unsigned char unk_E4[0x10];
		unsigned int			atkType_weap0;	//0xF4
		unsigned int			atkType_weap1;	//0xF8
		unsigned char unk_FC[0x58];
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
		unsigned char unk_204[0x40];
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
		unsigned char unk_2B8[0x10];
		FloatMini			FMini_unk2C8;	//			TODO
		//padding
		unsigned char unk_2D0[0x108];
		//padding
		CAgentTimer			timer_unk3D8;	//0x3D8		TODO
		unsigned char unk_3EC[0xC];
		FloatMini			unk_3F8;		//			TODO
		FloatMini			unk_400;		//			TODO
		FloatMini			unk_408;		//			TODO
	};//sizeof = 0x410

	struct CAbilityCargoHold {
		void** vtable;			//0x0
		unsigned char unk_4[0x2C];
		CUnit* abilityOwner;	//0x30
		unsigned int			id;				//0x34
		unsigned char unk_38[0x3C];
		unsigned int			cargoSpaceUsed;		//0x74
		unsigned char unk_78[0xC];
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
		unsigned char				unk_4[0x78];	//0x4
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
		unsigned char	unk_0[0x20];						//0x0
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
		unsigned char				unk_4[0x610];	//0x4
		unsigned int			replayState;		//0x614, 录像模式=2 ?
		unsigned char				unk_618[0x1658];//0x618
		unsigned int			time;			//0x1C70
		unsigned char				unk_1C74[0x640];		//0x1C74
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
		unsigned short		pixel_x;		//0x30 [0, 分辨率X]	2D坐标(实际像素)，
		unsigned short		pixel_y;		//0x32 [0, 分辨率Y]	左上角为原点(0, 0)
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
			unsigned char	unk_14[0x14];	//0x14
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
		unsigned char			id;
	};

	struct PacketAction {
		unsigned char			id;					//0x0	default 0x10
		unsigned short		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id like 0xD0004 or 'ewsp'
		HashGroup		hashUsedItem;		//0x7	hash group of item used
	};//sizeof = 15

	struct PacketActionPoint {
		unsigned char			id;					//0x0	default 0x11
		unsigned short		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id
		HashGroup		hashUsedItem;		//0x7	hash group of item used
		float			targetX;			//0xF
		float			targetY;			//0x13
	};//sizeof = 23

	struct PacketActionTarget {
		unsigned char			id;					//0x0	default 0x12
		unsigned short		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id
		HashGroup		hashUsedItem;		//0x7	hash group of item used
		float			targetX;			//0xF
		float			targetY;			//0x13
		HashGroup		hashWidgetTarget;	//0x17	hash group of target widget
	};//sizeof = 31

	struct PacketActionDropItem {
		unsigned char			id;					//0x0	default 0x13
		unsigned short		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id
		HashGroup		hashUsedItem;		//0x7	hash group of item used
		float			targetX;			//0xF
		float			targetY;			//0x13
		HashGroup		hashTarget;			//0x17	hash group of target widget
		HashGroup		hashItem;			//0x1F	hash group of transferred item
	};//sizeof = 39

	struct PacketActionIndirect {
		unsigned char			id;					//0x0	default 0x14
		unsigned short		flag;				//0x1	ability flag
		unsigned int		actionId;			//0x3	order id
		HashGroup		hashUsedItem;		//0x7	hash group of item used
		float			targetX;			//0xF
		float			targetY;			//0x13
		unsigned int		typeId;				//0x17	type id of target
		unsigned char			unk_1F[9];
		float			targetX2;			//0x24
		float			targetY2;			//0x28
	};//sizeof = 44

	struct PacketSelection {
		unsigned char			id;					//0x0	default 0x16
		unsigned char			mode;				//0x1	select=1, deselect=2
		unsigned short		count;				//0x2	count of manipulating widgets
		HashGroup		objects[12];		//0x4	hash group of widgets
	};

	struct PacketControlGroupAssign {
		unsigned char			id;					//0x0	default 0x17
		unsigned char			numGroup;			//0x1	0~9 for control group 1~10
		unsigned short		count;				//0x2	count of manipulating widgets
		HashGroup		objects[12];		//0x4	hash group of widgets
	};

	struct PacketControlGroupSelect {
		unsigned char			id;					//0x0	default 0x18
		unsigned char			numGroup;			//0x1	0~9 for control group 1~10
		unsigned char			unk_2;				//0x2	default 0x3
	};// sizeof = 3

	struct PacketSubgroup {
		unsigned char			id;					//0x0	default 0x19
		unsigned int		typeId;				//0x1	type id of new active subgroup
		HashGroup		object;				//0x5	hash of new active subgroup unit
	};// sizeof = 13

	struct PacketPreSubSelection {
		unsigned char			id;					//0x0	default 0x1A
	};//sizeof = 1

	struct PacketSelectionItem {
		unsigned char			id;					//0x0	default 0x1C
		unsigned char			unk_1;				//0x1	always 0x04
		HashGroup		hashItem;			//0x2	hash of item to select
	};//sizeof = 10

	struct PacketCancelRevive {
		unsigned char			id;					//0x0	default 0x1D
		HashGroup		hashHero;			//0x2	hash of reviving hero canceled
	};//sizeof = 9

	struct PacketCancelQueue {
		unsigned char			id;					//0x0	default 0x1E
		unsigned char			slot;				//0x1	0 ~ n for slots
		unsigned int		typeId;				//0x2	type id of canceled object
	};//sizeof = 6

#pragma pack(pop)

}//namespace war3
