#include "stdafx.h"
#ifndef UISTRUCTS_H_INCLUDED
#define UISTRUCTS_H_INCLUDED

#include "StructHelper.h"
#include "StringStructs.h"
#include "BaseStructs.h"

namespace war3 {

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif // _MSC_VER

	struct CSimpleButton;
	struct CIconCover;
	struct CHeroBarButton;
	struct CResourceCover;
	struct CHeroBar;
	struct CPeonBar;
	struct CMinimap;
	struct CInfoBar;
	struct CCommandBar;
	struct CWorldFrameWar3;
	struct CSimpleFrame;
	struct CSimpleConsole;
	struct CSimpleMessageFrame;
	struct CPortraitButton;
	struct CUpperButtonBar;
	struct CTimeOfDayIndicator;
	struct CFramePointRelative;
	struct SimpleTopButtonListArray;
	struct SimpleTopButtonList;
	struct CChatEditBar;
	struct CUberToolTipWar3;
	struct CCameraWar3;
	struct CStatBar;
	struct CSimpleTexture;
	struct CSimpleFontString;
	struct SimpleFrameTextureSettings;
	struct CUnit;
	struct CCommandButtonData;
	struct CAbility;


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
		uint8_t			unk_4[0x198];	//0x4
		CCameraWar3* camera;			//0x19C
	};

	struct SimpleTopButtonListArray {
		SimpleTopButtonList* pButtonLists[0x300];
	};

	struct SimpleTopButtonList {
		void* firstButton;
		unsigned int					firstButtonType;//尚未搞明白
		//...
	};

	struct CChatEditBox {
		void** vtable;			//0x0
		uint8_t						unk_4[0x1E0];	//0x4
		char* text;			//0x1E4
	};

	struct CChatEditBar {
		void** vtable;			//0x0
		uint8_t						unk_4[0xAC];	//0x4
		unsigned int					flags;			//0xB0 关闭时101101b 打开时101100b
		uint8_t						unk_B4[0x128];	//0xB4
		unsigned int					visible;		//0x1DC	关闭时0 打开时1
		CChatEditBox* chatEditBox;	//0x1E0
		//...
	};

	struct CSimpleConsole {
		void** vtable;						// = 0x93BD2C
		uint8_t unk_4[0x4];
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
		uint8_t unk_4[0x8A8];
		SimpleTopButtonListArray* pButtonListArray;			//0x8AC
		uint8_t unk_8B0[0x4];
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
		uint8_t						colorAlpha;			//0x68
		uint8_t						colorBlue;			//0x69
		uint8_t						colorGreen;			//0x6A
		uint8_t						colorRed;			//0x6B
		uint8_t						unk_6C[4];			//0x6C	
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
		CObserver* clickEventObserver;	//0x124 CGameUI*?
		unsigned int					clickEventId;		//0x128
		CObserver* mouseEventObserver;	//0x12C	6F603060 CGameUI*?
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
		uint8_t		unk12C[0x58];	//0x12C
		unsigned int	unk184;			//0x184
		unsigned int	unk188;			//0x188
		char		tooltip[0x400];	//0x18C 长度正确？
		uint8_t		unk58C[0x8];	//0x58C
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
		CAbility* ability;		//0x6D4 基本技能(例如move)这里是NULL
	};//sizeof = 0x6D8

	//需要搞清楚CShrinkingButton (可能只有虚函数)
	struct CCommandButton {
		CSimpleButton				baseSimpleButton;	//0x0
		uint8_t unk168[0x28];
		CCommandButtonData* commandButtonData;		//0x190
		uint8_t unk194[0x2C];
	};//sizeof = 0x1C0

	struct CHeroBarButton {//FIXME 同时也是闲农民按键? 研究peonbar
		CCommandButton				baseCommandButton;	//0x0
		unsigned int unk1C0;
		CUnit* hero;				//0x1C4
		unsigned int					heroTypeId;			//0x1C8
		uint8_t unk1CC[0x44];
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
		uint8_t					unk_144[0x8];		//0x144
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
		uint8_t					unk_48[0x68];		//0x48
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
		uint8_t					unk_18C[0x4];		//0x18C
		unsigned int				unk_190;			//0x190
		unsigned int				unk_194;			//0x194
		unsigned int				unk_19C;			//0x19C
		unsigned int				unk_1A0;			//0x1A0 = 17
		uint8_t					unk_1A4[0x4];		//0x1A4
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
		uint8_t				unk_180[0x2C];				//0x180
		unsigned int					flag_1AC;					//0x1AC
		unsigned int					flag_1B0;					//0x1B0
		uint8_t				unk_1B4[0x18];				//0x1B4
		CUberToolTipWar3* uberToolTip;				//0x1CC
		uint8_t				unk_1D0[0x84];				//0x1D0
		CCameraWar3* camera;						//0x254
		unsigned int					flags_258;					//0x258
		uint8_t				unk_25C[0x160];				//0x25C
		CWorldFrameWar3* world;						//0x3BC

		CMinimap* minimap;
		CInfoBar* infobar;

		CCommandBar* commandBar;					//0x3C8
		CResourceCover* resourceBar;				//0x3CC
		CUpperButtonBar* unkUpperButton3D0;			//0x3D0 CUpperButtonBar*
		CSimpleFrame* minimapScreenFrame;			//0x3D4
		CSimpleButton* unkButton3D8;				//0x3D8 CSimpleButton*
		CHeroBar* unkHeroBar3DC;				//0x3DC CHeroBar*
		CPeonBar* idlePeonBar;				//0x3E0 CPeonBar*
		CSimpleMessageFrame* unkSimpleMessageFrame3E4;	//0x3E4 CSimpleMessageFrame*
		CSimpleMessageFrame* unkSimpleMessageFrame3E8;	//0x3E8 CSimpleMessageFrame*
		CSimpleMessageFrame* unkSimpleMessageFrame3EC;	//0x3EC CSimpleMessageFrame*
		CSimpleMessageFrame* unkSimpleMessageFrame3F0;	//0x3F0 CSimpleMessageFrame*
		CPortraitButton* portraitButton;				//0x3F4 CPortraitButton*
		CTimeOfDayIndicator* timeOfDayIndicator;			//0x3F8 CTimeOfDayIndicator*

		CChatEditBar* chatEditBar;				//0x3FC
		uint8_t				unk_400[0x8];//0x400 CCinematicPanel* 
		CSimpleButton* minimapSignalButton;		//0x408
		CSimpleButton* minimapTerrainButton;		//0x40C
		CSimpleButton* minimapCreepButton;			//0x410
		CSimpleButton* minimapAllyButton;			//0x414
		CSimpleButton* minimapFormationButton;		//0x418
		uint8_t				unk_41C[0xC];
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
		uint8_t					unk_194[0x24];		//0x194
		float					unk_1B8;			//0x1B8 = 0.025
		float					unk_1BC;			//0x1BC
		unsigned int				unk_1C0;			//0x1C0
		unsigned int				unk_1C4;			//0x1C4
		float					unk_1C8;			//0x1C8
		float					unk_1CC;			//0x1CC
		float					unk_1D0;			//0x1D0
		float					unk_1D4;			//0x1D4	
		uint8_t					unk_1D8[0x4];		//0x1D8
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
		uint8_t						unk_234[0x8];			//0x234 
		unsigned int					needUpdate;				//0x23C
	}; //sizeof = 0x240

	struct CButtonFrame {
		CControl				baseControl;			//0x0
	}; //sizeof = 0x1E4

	struct CGlueButtonWar3 {
		CButtonFrame			baseButtonFrame;		//0x0
		unsigned int				unk_1E4;				//0x1E4
	}; //sizeof = 0x1E8

#ifdef _MSC_VER
#pragma pack(pop)
#endif // _MSC_VER

}

#endif