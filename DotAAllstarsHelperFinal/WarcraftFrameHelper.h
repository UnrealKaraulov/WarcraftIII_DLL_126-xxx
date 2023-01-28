
/* Warcraft Frame Class by Karaulov (using ingame ui)*/


#pragma once
#include "Main.h"
#include <MinHook.h>
#include <vector>
#include <string>


#define IsKeyPressed(CODE) ((GetAsyncKeyState(CODE) & 0x8000) > 0)




namespace NWar3Frame
{
	// thanks DreamHacks (dreamdota) for structures:

#pragma pack(push, 1)

	struct CObserver {
		void** vtable;			//0x0
		unsigned int		refCount;		//0x4
		unsigned int		registry;		//0x8 ObserverRegistry 
	};//sizeof = 0xC 

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
		unsigned int				unk_1CC;			//0x1CC 
		unsigned int				unk_1D0;			//0x1D0
		unsigned int				unk_1D4;			//0x1D4 
		unsigned int				unk_1D8;			//0x1D8	
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



	struct CFrameEventStruct
	{
		int int1;//0
		int int2;//4
		int Event;//8   //4 = show
		int int4;//C
		CFrameEventStruct()
		{
			int1 = int2 = Event = int4 = 0;
		}
	};


	enum class  CFramePosition : unsigned int
	{
		TOP_LEFT = 0,
		TOP_CENTER = 1,
		TOP_RIGHT = 2,
		LEFT = 3,
		CENTER = 4,
		RIGHT = 5,
		BOTTOM_LEFT = 6,
		BOTTOM_CENTER = 7,
		BOTTOM_RIGHT = 8
	};

	enum class  CFrameType : unsigned int
	{
		FRAMETYPE_FRAME = 0,
		FRAMETYPE_BACKDROP = 1,
		FRAMETYPE_ITEM = 2,
		FRAMETYPE_POPUPMENU = 3,
		FRAMETYPE_EDITBOX = 4,
		FRAMETYPE_BUTTON = 5,
		FRAMETYPE_TEXTBUTTON = 6,
		FRAMETYPE_SPRITE = 7,
		FRAMETYPE_HIGHLIGHT = 8,
		FRAMETYPE_SCROLLBAR = 9,
		FRAMETYPE_MODEL = 10
	};

	enum class CFrameBackdropType : unsigned int
	{
		ControlFrame = 0,
		ControlBackdrop = 1,
		ControlPushedBackdrop = 2,
		ControlDisabledBackdrop = 3
	};

#pragma pack(pop)

	extern void(*GlobalEventCallback)();

	class   CWar3Frame {
	public:
		bool FrameOk;
		bool Visibled;
		CFrameType FrameType;
		bool FrameDestroyable;
		bool AnimateStarted;
		float anim_offset;
		static CWar3Frame* FindCWar3Frame(unsigned char* FrameAddr);
		void SetAnimOffset(float off);
		void SetFrameDestroyabled(bool destoyable);
		unsigned char* FrameAddr;
		int FrameId;
		int CustomValue[10];
		bool Focused;
		bool Pressed;
		bool SkipCallbackForInternalFunctions;
		bool SkipCallback;
		std::string FrameName;
		bool SkipOtherEvents;
		unsigned int ClickTime;
		std::vector<unsigned int> RegisteredEventId;
		void RegisterEventCallback(unsigned int EventId);
		static  unsigned char* GetCurrentFrameAddr();
		int(*FrameCallback)(CWar3Frame* frame, unsigned char* FrameAddr, unsigned int EventId);
		static void SetGlobalEventCallback(void(*)());
		static unsigned char* GetFrameItem(const char* name, int id = 0);
		static void __fastcall Wc3ChangeMenu_my(int a1, int  a2);
		static int __fastcall FrameEventCallback(unsigned char* FrameAddr, int dummy, unsigned int EventId);
		static void __fastcall SetAnimOffset(unsigned char* real_frame, float* a2, float unk_val);
		void SetSkipAnotherCallback(bool skip = true);
		static void UninitializeAllFrames(bool freeframes = false);
		static void Init(unsigned char* GameDll);
		void SetModel(const char* modelpath, int MdlType = 1 /* -1 */, int Flags = 0);
		static void Wc3PlaySound(const char* name);
		static std::string DumpAllFrames();
		const char* GetText();
		unsigned int GetTextMaxLength();
		void Enable(bool enable);
		bool IsEnabled();
		bool IsFocused();
		bool IsPressed();
		void SetTextFrameFontHeight(float y);
		void SetTextFrameFontWidth(float x);
		bool IsChecked();
		void SetFrameCustomValue(int value, int id = 1);
		int GetFrameCustomValue(int id = 1);
		void SetChecked(bool checked);
		void Show(bool enable);
		void SetCallbackFunc(int(*callback)(CWar3Frame* frame, unsigned char* FrameAddr, unsigned int EventId));
		void Update(bool force = false);
		void AddItem(const char* name);
		void SetMaxLen(unsigned int  maxlen);
		void SetText(const char* text, unsigned int len = 0);
		void SetFrameAbsolutePosition(CFramePosition orginPosition, float absoluteX, float absoluteY, unsigned int flag = 1);
		void SetFrameRelativePosition(CFramePosition orginPosition, int dstFrameAddr, CFramePosition toPosition, float relativeX, float relativeY, unsigned int flag = 1);
		void SetTexture(const char* path, const char* border = NULL, bool tiled = false, CFrameBackdropType backgtype = CFrameBackdropType::ControlFrame);
		static void InitCallbackHook();
		static void UninitializeCallbackHook();
		static void LoadFrameDefFiles(const char* filename, bool force = false);
		bool CheckIsOk();
		bool IsVisibled();
		unsigned char* Load(const char* name, int id = 0, bool showerror = false);
		void SetFrameType(CFrameType newframetype);
		CWar3Frame(const char* name, int id = 0, bool show = false, unsigned char* relativeframe = NULL, bool showerror = false);
		//CWar3Frame( const char * name, int relativeframe = NULL, bool show = false, int id = 0, bool showerror = false );
		unsigned char* CWar3FrameFromAddress(unsigned char* FrameAddr, bool show = false, bool showerror = false);
		void DestroyThisFrame();
		void SetFocus(bool focused = true);
		void UpdateFlagsV2(unsigned int addflag = 0);
		static void UpdateFlagsV2Addr(unsigned char* FrameAddr, unsigned int addflag = 0);
		void SetCursor(bool enabled = true);
		void Click();
		void SetScrollBarValuesCount(int count, int current = 0);
		int GetScrollBarValue();
		int GetScrollBarMaxValue();
		void ClearMenuItems();
		void SetFlag(unsigned int flag);
		static void SetChangeMenuEventCallback(void(*changeMenuEventCallback)());
		static void FocusFrame(unsigned char* pFrameAddr);
		void SetFrameFocused();
		void StartAnimate(int anim_id = 0);
		void StopAnimate();
		unsigned char* GetFrameBackdropAddress(CFrameBackdropType backtype);
		void FillToParentFrame(CFrameBackdropType backtype, bool fill);
		void SetFrameScale(CFrameBackdropType backtype, float xscale, float yscale);
		void UpdateScale();
		CWar3Frame();
		~CWar3Frame();
	};

	extern std::vector<CWar3Frame*> FramesList;

	const int CFrameEvent_ShowFrame = 4;


	/* Without auto frametype detection :( (need check vtable addr ? )*/

	extern bool CFrameCallbackInitialized;
	extern bool CFrameInitialized;
	extern int FramesCount;
	extern CGlueMgr** GlobalGlueObj;
	//extern int pGameGlobalUIaddr = 0;
	extern int(__thiscall* FrameEventHandler)(unsigned char* FrameAddr, unsigned int EventId);
	extern int(__thiscall* FrameEventHandler_ptr)(unsigned char* FrameAddr, unsigned int EventId);
	//extern void( __stdcall * FixFrameHookError1 )( int a1 );
	//extern void( __stdcall * FixFrameHookError1_ptr )( int a1 );
	//extern void __stdcall FixFrameHookError1_my( int a1 )
	//{
	//	if ( !a1 )
	//		return;
	//	FixFrameHookError1_ptr( a1 );
	//}
	extern CFrameEventStruct gEvent;
	extern int(__thiscall* SetTextFrameFont)(unsigned char* FrameAddr, const char* font, float scale, int flag);
	//extern int( __fastcall * CreateTexture )( const char * path, int *unkdata, int oldval, int visibled );
	extern int(__thiscall* GetCursorAddr)(int num);
	extern int(__fastcall* LoadFrameDefFile)(const char* filename, unsigned char* var1, unsigned char* var2, unsigned char* cstatus);
	extern unsigned char* (__fastcall* CreateNewCFrame) (const char* FrameName, unsigned char* rframeaddr, int UnkIdx, int UnkValue, int id);
	extern unsigned char* (__fastcall* GetFrameItemAddr)(const char* name, int id);
	extern int(__thiscall* PopupMenuAddItem)(unsigned char* FrameAddr, const char* a2, int flag); //flag = -2
	extern int(__thiscall* ClearPopupMenu)(unsigned char* FrameAddr);
	extern int(__thiscall* EditboxSetMaxLen /* sub_6F616250*/)(unsigned char* FrameAddr, unsigned int maxlen);
	extern int(__thiscall* TextFrameSetText)(unsigned char* FrameAddr, const char* text);
	extern int(__thiscall* EditBoxFrameSetText)(unsigned char* frameaddr, const char* newtext, int unk);
	extern void(__fastcall* SoundPlay/*sub_6F32F310*/)(int, const char* name, int, int, int, int, int, int);
	extern int(__thiscall* SimulateFrameEvent)(unsigned char* FrameAddr, CFrameEventStruct* Event);
	extern void(__thiscall* SetAbsolutePosition)(unsigned char* SrcFrameAddr /* or offset 180*/, CFramePosition orginPosition, float absoluteX, float absoluteY, unsigned int flag);//flag = 1
	extern void(__thiscall* SetRelativePosition)(unsigned char* SrcFrameAddr/* or offset 180*/, CFramePosition orginPosition, int DstFrameAddr, CFramePosition toPosition, float relativeX, float relativeY, unsigned int flag);//flag = 1
	extern unsigned int(__thiscall* UpdatePosition)(unsigned char* FrameAddr /* or offset 180*/, unsigned int flag);//flag = 1 = force update
	extern unsigned int(__thiscall* UpdateFlags)(unsigned char* FrameAddr, unsigned int flag);//flag = 1 = force update ?
	extern unsigned int(__thiscall* DestroyCFrame)(unsigned char* FrameAddr, unsigned int flag);//flag = 1 ?
	extern unsigned int(__thiscall* SetFrameTexture)(unsigned char* FrameAddr, const char* texturepath, unsigned char flags/*border?*/, int tiled, const char* borderpath, int flag);
	extern void(__fastcall* Wc3ChangeMenu)(int, int);
	extern void(__fastcall* Wc3ChangeMenu_ptr)(int, int);
	extern void(__thiscall* UpdateFrameScale)(unsigned char* FrameAddr);
	extern int(__thiscall* Wc3SimulateClickEvent)(unsigned char* btnaddr, int unk);

	extern void(__thiscall* SetFrameWidth/*sub_6F605D90*/)(unsigned char* FrameAddr, float x);
	extern void(__thiscall* SetFrameHeight/*sub_6F605DB0*/)(unsigned char* FrameAddr, float y);

	extern const char* (__fastcall* Wc3GetSkinItemPath)(const char* name, const char* theme);
	extern const char* (__fastcall* Wc3GetSkinItemPath2)(const char* name, const char* theme);
	extern void Wc3SetDefaultSkinTheme(const std::string& theme);
	extern std::string Wc3SelectedSkinTheme;
	extern unsigned char* CStatusDefaultCStatus;
	extern unsigned char* CStatusLoadFramesVar1;
	extern unsigned char* CStatusLoadFramesVar2;
	extern unsigned char* CurrentFrameFocusedAddr;

	// sub_6F4E8720
	extern int(__fastcall* StartSpriteModelAnimated)(int spritemodel, int unknown_or_anim_id, void(__fastcall* SetAnimOffset)(unsigned char* real_frame, float* a2, float unk_val), unsigned char* real_frame, float unk_or_start_off);

	extern std::vector<std::string> LoadedFramedefFiles;

	namespace CFrameEventsInternal
	{
		const unsigned int FRAME_EVENT_TICK = 0x40160064;
		const unsigned int FRAME_EVENT_PRESSED = 0x40090064;
		const unsigned int FRAME_MOUSE_ENTER = 0x40090068;
		const unsigned int FRAME_MOUSE_LEAVE = 0x40090066;
		const unsigned int FRAME_MOUSE_UP = 0x4009006A;
		const unsigned int FRAME_MOUSE_WHEEL = 0x4009006B;
		const unsigned int FRAME_FOCUS_CHANGE = 0x40090068;
		const unsigned int FRAME_CHECKBOX_CHECKED_CHANGE = 0x400C0064;
		const unsigned int FRAME_EDITBOX_TEXT_CHANGED = 0x400B0065;
		const unsigned int FRAME_POPUPMENU_ITEM_CHANGE_START = 0x40090065;
		const unsigned int FRAME_POPUPMENU_ITEM_CHANGED = 0x40130064;
	};

	namespace CFrameEvents
	{
		const unsigned int FRAME_EVENT_PRESSED = 1;
		const unsigned int FRAME_MOUSE_ENTER = 2;
		const unsigned int FRAME_MOUSE_LEAVE = 3;
		const unsigned int FRAME_MOUSE_UP = 4;
		const unsigned int FRAME_MOUSE_DOWN = 5;
		const unsigned int FRAME_MOUSE_WHEEL = 6;
		const unsigned int FRAME_FOCUS_ENTER = FRAME_MOUSE_ENTER;
		const unsigned int FRAME_FOCUS_LEAVE = FRAME_MOUSE_LEAVE;
		const unsigned int FRAME_CHECKBOX_CHECKED = 7;
		const unsigned int FRAME_CHECKBOX_UNCHECKED = 8;
		const unsigned int FRAME_EDITBOX_TEXT_CHANGED = 9;
		const unsigned int FRAME_POPUPMENU_ITEM_CHANGE_START = 10;
		const unsigned int FRAME_POPUPMENU_ITEM_CHANGED = 11;
		const unsigned int FRAME_MOUSE_DOUBLECLICK = 12;
		const unsigned int FRAME_SPRITE_ANIM_UPDATE = 13;
	};



	extern std::vector<unsigned int> AvaiabledEvents;




	/* need research */

	/* Example */
	/*void StormTest( );*/
}