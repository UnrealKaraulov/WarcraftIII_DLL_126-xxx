


/* Warcraft Frame Class by Karaulov (using ingame ui)*/

#include "WarcraftFrameHelper.h"

#include <fp_call.h>

#include "Storm.h"


namespace NWar3Frame
{

	// thanks DreamHacks (dreamdota) for structures:

	std::vector<CWar3Frame*> FramesList = std::vector<CWar3Frame*>();


	/* Without auto frametype detection :( (need check vtable addr ? )*/

	bool CFrameCallbackInitialized = false;
	bool CFrameInitialized = false;
	int FramesCount = 0;
	CGlueMgr** GlobalGlueObj = 0;
	unsigned char* CurrentFrameFocusedAddr = 0;
	// int pGameGlobalUIaddr = 0;
	int(__thiscall* FrameEventHandler)(unsigned char* FrameAddr, unsigned int EventId) = NULL;
	int(__thiscall* FrameEventHandler_ptr)(unsigned char* FrameAddr, unsigned int EventId) = NULL;
	// void( __stdcall * FixFrameHookError1 )( int a1 );
	// void( __stdcall * FixFrameHookError1_ptr )( int a1 );
	// void __stdcall FixFrameHookError1_my( int a1 )
	//{
	//	if ( !a1 )
	//		return;
	//	FixFrameHookError1_ptr( a1 );
	//}
	CFrameEventStruct gEvent = CFrameEventStruct();

	// int( __fastcall * CreateTexture )( const char * path, int *unkdata, int oldval, int visibled );
	int(__fastcall* LoadFrameDefFile)(const char* filename, unsigned char* var1, unsigned char* var2, unsigned char* cstatus) = NULL;
	unsigned char* (__fastcall* CreateNewCFrame) (const char* FrameName, unsigned char* rframeaddr, int UnkIdx, int UnkValue, int id) = NULL;
	unsigned char* (__fastcall* GetFrameItemAddr)(const char* name, int id) = NULL;
	int(__thiscall* PopupMenuAddItem)(unsigned char* FrameAddr, const char* a2, int flag) = NULL; //flag = -2
	int(__thiscall* ClearPopupMenu)(unsigned char* FrameAddr) = NULL;
	void(__thiscall* UpdateFrameScale)(unsigned char* FrameAddr) = NULL;
	void(__thiscall* SetFrameWidth/*sub_6F605D90*/)(unsigned char* FrameAddr, float x) = NULL;
	void(__thiscall* SetFrameHeight/*sub_6F605DB0*/)(unsigned char* FrameAddr, float y) = NULL;

	int(__thiscall* EditboxSetMaxLen /* sub_6F616250*/)(unsigned char* FrameAddr, unsigned int maxlen) = NULL;
	int(__thiscall* TextFrameSetText)(unsigned char* FrameAddr, const char* text) = NULL;
	int(__thiscall* EditBoxFrameSetText)(unsigned char* frameaddr, const char* newtext, int unk) = NULL;
	void(__fastcall* SoundPlay/*sub_6F32F310*/)(int, const char* name, int, int, int, int, int, int) = NULL;
	int(__thiscall* SimulateFrameEvent)(unsigned char* FrameAddr, CFrameEventStruct* Event) = NULL;
	void(__thiscall* SetAbsolutePosition)(unsigned char* SrcFrameAddr /* or offset 180*/, CFramePosition orginPosition, float absoluteX, float absoluteY, unsigned int flag) = NULL;//flag = 1
	void(__thiscall* SetRelativePosition)(unsigned char* SrcFrameAddr/* or offset 180*/, CFramePosition orginPosition, int DstFrameAddr, CFramePosition toPosition, float relativeX, float relativeY, unsigned int flag) = NULL;//flag = 1
	unsigned int(__thiscall* UpdatePosition)(unsigned char* FrameAddr /* or offset 180*/, unsigned int flag) = NULL;//flag = 1 = force update
	unsigned int(__thiscall* UpdateFlags)(unsigned char* FrameAddr, unsigned int flag) = NULL;//flag = 1 = force update ?
	unsigned int(__thiscall* DestroyCFrame)(unsigned char* FrameAddr, unsigned int flag) = NULL;//flag = 1 ?
	unsigned int(__thiscall* SetFrameTexture)(unsigned char* FrameAddr, const char* texturepath, unsigned char flags/*border?*/, int tiled, const char* borderpath, int flag) = NULL;
	int(__thiscall* GetCursorAddr)(int num) = NULL;
	int(__thiscall* Wc3SimulateClickEvent)(unsigned char* btnaddr, int unk) = NULL;
	int(__fastcall* StartSpriteModelAnimated)(int spritemodel, int unknown_or_anim_id, void(__fastcall* SetAnimOffset)(unsigned char* real_frame, float* a2, float unk_val), unsigned char* real_frame, float unk_or_start_off) = NULL;
	//	int( __thiscall * CFrameSetFocus)( unsigned char* FrameAddr, int flag );
	const char* (__fastcall* Wc3GetSkinItemPath)(const char* name, const char* theme) = NULL;
	const char* (__fastcall* Wc3GetSkinItemPath_ptr)(const char* name, const char* theme) = NULL;
	int(__thiscall* SetTextFrameFont)(unsigned char* FrameAddr, const char* font, float scale, int flag) = NULL;
	std::string Wc3SelectedSkinTheme = "";
	const char* __fastcall  Wc3GetSkinItemPath_my(const char* name, const char* theme)
	{
		if (Wc3SelectedSkinTheme.length() > 0)
		{
			const char* retval = Wc3GetSkinItemPath_ptr(name, Wc3SelectedSkinTheme.c_str());
			if (!retval)
				return  Wc3GetSkinItemPath_ptr(name, theme);
			return retval;
		}
		return Wc3GetSkinItemPath_ptr(name, theme);

	}
	void Wc3SetDefaultSkinTheme(const std::string& theme)
	{
		Wc3SelectedSkinTheme = theme;
	}

	void(__fastcall* Wc3ChangeMenu)(int, int) = NULL;
	void(__fastcall* Wc3ChangeMenu_ptr)(int, int) = NULL;
	void(*GlobalEventCallback)() = NULL;
	void(*ChangeMenuEventCallback)() = NULL;

	unsigned char* CStatusDefaultCStatus = 0;
	unsigned char* CStatusLoadFramesVar1 = 0;
	unsigned char* CStatusLoadFramesVar2 = 0;


	std::vector<std::string> LoadedFramedefFiles;


	std::vector<unsigned int> availabledEvents = std::vector<unsigned int>();



	void CWar3Frame::SetFrameDestroyabled(bool destoyable)
	{
		FrameDestroyable = destoyable;
	}

	void CWar3Frame::RegisterEventCallback(unsigned int EventId)
	{

		if (!FrameOk)
			return;


		RegisteredEventId.push_back(EventId);

		for (auto i : availabledEvents)
			if (i == EventId)
				return;

		availabledEvents.push_back(EventId);
	}

	// int CWar3Frame::GetGlobalGameUIAddr( )
	//{
	//	char asdf[ 512 ];
	//	sprintf_s( asdf, "%X %X", pW3XGlobalClass, pGameGlobalUIaddr );
	//	MessageBoxA( 0, asdf, asdf, 0 );
	//	return *( int* )pGameGlobalUIaddr;
	//}

	unsigned char* CWar3Frame::GetCurrentFrameAddr() // or GetGlobalClassAddr( )GAMEUI? 
	{
		if (!GlobalGlueObj || !(*GlobalGlueObj) || !(*GlobalGlueObj)->currentFrame)
			return 0;
		return (unsigned char*)(*GlobalGlueObj)->currentFrame;
	}

	unsigned char* CWar3Frame::GetFrameItem(const char* name, int id)
	{
		if (!name || name[0] == '\0')
			return 0;
		//CONSOLE_Print( "Get frame item:" + ( std::string )name );

		return GetFrameItemAddr(name, id);
	}

	void __fastcall CWar3Frame::Wc3ChangeMenu_my(int a1, int  a2)
	{
		if (ChangeMenuEventCallback)
			ChangeMenuEventCallback();
		//CONSOLE_Print( "Change menu" );
		CWar3Frame::UninitializeAllFrames();
		Wc3ChangeMenu_ptr(a1, a2);
		//CONSOLE_Print( "Change menu ok" );
	}

	CWar3Frame* CWar3Frame::FindCWar3Frame(unsigned char* FrameAddr)
	{
		for (auto frame : FramesList)
		{
			if (frame && frame->FrameOk && frame->FrameAddr == FrameAddr && frame->FrameName.length() > 0)
			{
				return frame;
			}
		}
		return NULL;
	}

	void __fastcall CWar3Frame::SetAnimOffset(unsigned char* real_frame, float* a2, float default_offset)
	{
		CWar3Frame* wc3frame = FindCWar3Frame(real_frame);
		if (wc3frame)
			*a2 = wc3frame->anim_offset;
		FrameEventCallback(real_frame, 0, CFrameEvents::FRAME_SPRITE_ANIM_UPDATE);
	}



	int __fastcall CWar3Frame::FrameEventCallback(unsigned char* FrameAddr, int dummy, unsigned int EventId)
	{
		if (!FrameAddr)
			return FrameEventHandler_ptr(FrameAddr, EventId);



		/*	if ( IsKeyPressed( VK_LMENU ) )
			{
				if ( EventId != CFrameEventsInternal::FRAME_EVENT_TICK )
				{
					char checkboxaddr[ 100 ];
					sprintf_s( checkboxaddr, "1 CB:%X -> EV:%X", FrameAddr, EventId );
					CONSOLE_Print( checkboxaddr );
				}

			}

	*/

		bool FoundCallbackFrame = false;
		unsigned int EventIdConverted = EventId;
		bool NeedSkipEvents = false;
		CWar3Frame* callbackframe = NULL;

		for (auto frame : FramesList)
		{
			if (frame && frame->FrameOk && !frame->SkipCallback && frame->FrameAddr == FrameAddr && frame->FrameName.length() > 0)
			{
				// fix me? : currently limited single event. Need add support multiple events at once.
				// l convert game events to events
				switch (EventId)
				{
				case CFrameEventsInternal::FRAME_MOUSE_ENTER:
				case CFrameEventsInternal::FRAME_MOUSE_LEAVE:
					if (frame->IsFocused() && !frame->Focused)
					{
						frame->Focused = true;
						EventIdConverted = CFrameEvents::FRAME_FOCUS_ENTER;
					}
					else if (!frame->IsFocused() && frame->Focused)
					{
						frame->Focused = false;
						EventIdConverted = CFrameEvents::FRAME_FOCUS_LEAVE;
					}
					else if (frame->IsFocused() && frame->Focused)
					{
						if (IsKeyPressed(VK_LBUTTON) && frame->IsPressed() && !frame->Pressed)
						{
							frame->Pressed = true;
							EventIdConverted = CFrameEvents::FRAME_MOUSE_DOWN;
							if (frame->ClickTime != 0)
							{
								if (GetTickCount() - frame->ClickTime < 300
									&& GetTickCount() - frame->ClickTime > 5)
								{
									EventIdConverted = CFrameEvents::FRAME_MOUSE_DOUBLECLICK;
								}
							}

							for (auto frame_ : FramesList)
							{
								if (frame_ && frame_->FrameOk && !frame_->SkipCallback && frame_->FrameName.length() > 0)
								{
									frame_->ClickTime = 0;
								}
							}

							frame->ClickTime = GetTickCount();
						}
					}
					break;
				case CFrameEventsInternal::FRAME_EVENT_PRESSED:
					frame->Pressed = true;
					EventIdConverted = CFrameEvents::FRAME_EVENT_PRESSED;
					break;
				case CFrameEventsInternal::FRAME_MOUSE_UP:
					if (frame->Pressed && !IsKeyPressed(VK_LBUTTON))
						EventIdConverted = CFrameEvents::FRAME_MOUSE_UP;
					frame->Pressed = false;
					if (frame->ClickTime != 0)
					{
						if (GetTickCount() - frame->ClickTime < 300
							&& GetTickCount() - frame->ClickTime > 5)
						{
							EventIdConverted = CFrameEvents::FRAME_MOUSE_DOUBLECLICK;
						}
					}

					for (auto frame_ : FramesList)
					{
						if (frame_ && frame_->FrameOk && !frame_->SkipCallback && frame_->FrameName.length() > 0)
						{
							frame_->ClickTime = 0;
						}
					}

					frame->ClickTime = GetTickCount();
					break;
				case CFrameEventsInternal::FRAME_EDITBOX_TEXT_CHANGED:
					EventIdConverted = CFrameEvents::FRAME_EDITBOX_TEXT_CHANGED;
					break;
				case CFrameEventsInternal::FRAME_MOUSE_WHEEL:
					EventIdConverted = CFrameEvents::FRAME_MOUSE_WHEEL;
					break;
				case CFrameEventsInternal::FRAME_CHECKBOX_CHECKED_CHANGE:
					if (frame->IsChecked())
						EventIdConverted = CFrameEvents::FRAME_CHECKBOX_CHECKED;
					else
						EventIdConverted = CFrameEvents::FRAME_CHECKBOX_UNCHECKED;
					break;
				case CFrameEventsInternal::FRAME_POPUPMENU_ITEM_CHANGE_START:
					EventIdConverted = CFrameEvents::FRAME_POPUPMENU_ITEM_CHANGE_START;
					break;
				case CFrameEventsInternal::FRAME_POPUPMENU_ITEM_CHANGED:
					EventIdConverted = CFrameEvents::FRAME_POPUPMENU_ITEM_CHANGED;
					break;
				default:
					break;
				}

				callbackframe = frame;
				FoundCallbackFrame = true;


				// Fix unknown events // need fix me ?

			/*	if ( !EventIdConverted )
				{
					if (  IsKeyPressed( VK_LBUTTON ) )
					{

						EventIdConverted = CFrameEvents::FRAME_MOUSE_DOWN;
					}
				}
*/

//if ( IsKeyPressed( VK_LMENU ) )
//{
//	CONSOLE_Print( "^ in list. Converted eventid:" + to_string( EventIdConverted ) );
//}

				if (availabledEvents.size() == 0 || availabledEvents.end() == find(availabledEvents.begin(), availabledEvents.end(), EventIdConverted))
				{
					if (GlobalEventCallback)
						GlobalEventCallback();
					return 0;
				}

				NeedSkipEvents = frame->SkipOtherEvents;
				break;
			}
		}

		if (!callbackframe)
		{
			return FrameEventHandler_ptr(FrameAddr, EventId);
		}


		if (!EventIdConverted)
		{
			if (NeedSkipEvents)
				return 0;
			return FrameEventHandler_ptr(FrameAddr, EventId);
		}





		////CONSOLE_Print( "2" );
		//int retval = FrameEventHandler_ptr( FrameAddr, EventId );
		//if ( EventId == CFrameEventsInternal::FRAME_EVENT_TICK
		//	return retval;
		//}
		////CONSOLE_Print( "Event " );


		////CONSOLE_Print( "3" );
		/*char tmps[ 50 ];
		sprintf_s( tmps, "Frame:%X Code:%X", FrameAddr, EventId );
		//CONSOLE_Print( tmps );
		*/
		//if ( retval )
		//{

		int retval = NeedSkipEvents ? 0 : FrameEventHandler_ptr(FrameAddr, EventId);

		if (callbackframe->RegisteredEventId.size() > 0)
		{
			if (callbackframe->FrameCallback)
			{

				bool FoundRegisteredEvent = false;

				if (callbackframe->RegisteredEventId.end() != find(callbackframe->RegisteredEventId.begin(), callbackframe->RegisteredEventId.end(), EventIdConverted))
				{
					/*		char checkboxaddr[ 100 ];
							sprintf_s( checkboxaddr, "2 CB:%X -> EV:%X -> FIX:%i", callbackframe->FrameAddr, EventId, EventIdConverted );
							MessageBox( 0, checkboxaddr, "", 0 );

		*/
					FoundRegisteredEvent = true;
				}


				if (FoundRegisteredEvent && callbackframe->CheckIsOk())
					if (!callbackframe->FrameCallback(callbackframe, callbackframe->FrameAddr, EventIdConverted))
					{
						//	//CONSOLE_Print( "4" );
						if (GlobalEventCallback)
							GlobalEventCallback();
						return 0;
					}
			}
		}



		////CONSOLE_Print( "4" );
		/*//CONSOLE_Print( "All callback called" );
		*/
		//}
		if (GlobalEventCallback)
			GlobalEventCallback();
		return retval;
	}

	void CWar3Frame::SetScrollBarValuesCount(int count, int current)
	{

		if (!FrameOk)
			return;
		if (FrameType == CFrameType::FRAMETYPE_SCROLLBAR)
		{


			float* maxval = (float*)(FrameAddr + 0x1F0);
			if (count > -1)
			{
				if (count > 0)
					*maxval = (float)count;
				else
					*maxval = 0.0f;
			}
			float* step = (float*)(FrameAddr + 0x1F8);
			*step = 1.0f;

			if (current > *maxval)
				current = 0;
			else if (current < 0)
				current = (int)*maxval;


			float* curval = (float*)(FrameAddr + 0x1F4);
			*curval = (float)(*maxval - current);

			//CONSOLE_Print( "Change value for scrollbar! MaxVal:" + to_string( *maxval ) + ". CurVal:" + to_string( *curval ) + ". MaxInt:" + to_string( count ) + ". CurInt:" + to_string( current ) );
			UpdateFlagsV2();
		}
	}

	int CWar3Frame::GetScrollBarValue()
	{

		if (!FrameOk)
			return 0;
		if (FrameType == CFrameType::FRAMETYPE_SCROLLBAR)
		{
			float* curval = (float*)(FrameAddr + 0x1F4);
			float* maxval = (float*)(FrameAddr + 0x1F0);
			return (int)(*maxval - *curval);
		}
		return 0;
	}

	int CWar3Frame::GetScrollBarMaxValue()
	{

		if (!FrameOk)
			return 0;
		if (FrameType == CFrameType::FRAMETYPE_SCROLLBAR)
		{
			float* maxval = (float*)(FrameAddr + 0x1F0);
			return (int)maxval;
		}
		return 0;
	}



	void CWar3Frame::SetSkipAnotherCallback(bool skip)
	{
		SkipOtherEvents = skip;
	}

	void CWar3Frame::UninitializeAllFrames(bool freeframes)
	{
		//CONSOLE_Print( "UninitializeAllFrames " );

		auto tmpframelist = FramesList;
		FramesList.clear();
		availabledEvents.clear();

		for (unsigned int i = 0; i < tmpframelist.size(); i++)
		{
			if (tmpframelist[i] && tmpframelist[i]->FrameAddr && tmpframelist[i]->FrameName.length() > 0 && tmpframelist[i]->CheckIsOk())
			{
				tmpframelist[i]->DestroyThisFrame();
				if (freeframes)
					delete tmpframelist[i];
			}
		}
		tmpframelist.clear();
	}




	void CWar3Frame::Init(unsigned char* GameDll)
	{
		CStatusDefaultCStatus = GameDll + 0xA8C804;
		CStatusLoadFramesVar1 = GameDll + 0xACD214;
		CStatusLoadFramesVar2 = GameDll + 0xACD264;
		FrameEventHandler = (int(__thiscall*)(unsigned char* frameaddr, unsigned int eventid))(GameDll + 0x62A580);
		LoadFrameDefFile = (int(__fastcall*)(const char* filename, unsigned char* var1, unsigned char* var2, unsigned char* cstatus)) (GameDll + 0x5D8DE0);
		CreateNewCFrame = (unsigned char* (__fastcall*) (const char* FrameName, unsigned char* rframeaddr, int UnkIdx, int UnkValue, int id))(GameDll + 0x5C9560);
		SimulateFrameEvent = (int(__thiscall*)(unsigned char* FrameAddr, CFrameEventStruct * EventId))(GameDll + 0x370710);
		SetAbsolutePosition = (void(__thiscall*)(unsigned char* FrameAddr180, CFramePosition orginPosition, float absoluteX, float absoluteY, unsigned int flag))(GameDll + 0x6061B0);
		SetRelativePosition = (void(__thiscall*)(unsigned char* SrcFrameAddr, CFramePosition orginPosition, int DstFrameAddr, CFramePosition toPosition, float relativeX, float relativeY, unsigned int flag))(GameDll + 0x606770);
		UpdatePosition = (unsigned int(__thiscall*)(unsigned char* FrameAddr /* or offset 180*/, unsigned int flag))(GameDll + 0x605CC0);
		UpdateFlags = (unsigned int(__thiscall*)(unsigned char* FrameAddr, unsigned int flag))(GameDll + 0x602370);
		DestroyCFrame = (unsigned int(__thiscall*)(unsigned char* FrameAddr, unsigned int flag))(GameDll + 0x606910);
		SetFrameTexture = (unsigned int(__thiscall*)(unsigned char* FrameAddr, const char* texturepath, unsigned char flags/*border?*/, int tiled, const char* borderpath, int flag))(GameDll + 0x6212D0);
		GetFrameItemAddr = (unsigned char* (__fastcall*)(const char* name, int id))(GameDll + 0x5FA970);
		Wc3ChangeMenu = (void(__fastcall*)(int, int))(GameDll + 0x5934F0);
		TextFrameSetText = (int(__thiscall*)(unsigned char* FrameAddr, const char* text))(GameDll + 0x611D40);
		SoundPlay = (void(__fastcall*)(int, const char* name, int, int, int, int, int, int))(GameDll + 0x32F310);
		EditboxSetMaxLen = (int(__thiscall*   /* sub_6F616250*/)(unsigned char* FrameAddr, unsigned int maxlen))(GameDll + 0x616250);
		EditBoxFrameSetText = (int(__thiscall*)(unsigned char* frameaddr, const char* newtext, int unk))(GameDll + 0x615B50);
		//settextaddr =
		PopupMenuAddItem = (int(__thiscall* /*sub_6F613260*/)(unsigned char* FrameAddr, const char* a2, int flag))(GameDll + 0x613260); //flag = -2
		ClearPopupMenu = (int(__thiscall*)(unsigned char* FrameAddr))(GameDll + 0x334CF0);
		//	FixFrameHookError1 = ( void( __stdcall * )( int a1 ) )( GameDll + 0x5F83F0 );
		//	CFrameSetFocus = (int( __thiscall * )( unsigned char* FrameAddr, int flag ))( GameDll + 0x336C20 );
		Wc3GetSkinItemPath = (const char* (__fastcall*)(const char* name, const char* theme))(GameDll + 0x31F530);
		GlobalGlueObj = (CGlueMgr**)(GameDll + 0xACE66C);
		//pGameGlobalUIaddr = ( GameDll + 0xAB4F80 );
		GetCursorAddr = (int(__thiscall*)(int num))(GameDll + 0x4D9830);
		Wc3SimulateClickEvent = (int(__thiscall*)(unsigned char* btnaddr, int unk)) (GameDll + 0x601F20);
		StartSpriteModelAnimated = (int(__fastcall*)(int spritemodel, int unknown_or_anim_id, void(__fastcall * SetAnimOffset)(unsigned char* real_frame, float* a2, float unk_val), unsigned char* real_frame, float unk_or_start_off))(GameDll + 0x4E8720);
		UpdateFrameScale = (void(__thiscall*)(unsigned char* FrameAddr))(GameDll + 0x605700);
		SetFrameWidth = (void(__thiscall* /*sub_6F605D90*/)(unsigned char* FrameAddr, float x))(GameDll + 0x605D90);
		SetFrameHeight = (void(__thiscall* /*sub_6F605DB0*/)(unsigned char* FrameAddr, float y)) (GameDll + 0x605DB0);
		SetTextFrameFont = (int(__thiscall*)(unsigned char* FrameAddr, const char* font, float scale, int flag))(GameDll + 0x5FB960);
		CurrentFrameFocusedAddr = GameDll + 0xACE67C;
		CFrameInitialized = true;

	}

	void CWar3Frame::SetFlag(unsigned int flag)
	{

		if (!FrameOk)
			return;

		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			*(int*)(FrameAddr + 0x1D4) = flag;
		}
	}

	void CWar3Frame::SetFrameFocused()
	{

		if (!FrameOk)
			return;

		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			return;
		}

		*(unsigned char**)CurrentFrameFocusedAddr = FrameAddr;
	}

	void CWar3Frame::FocusFrame(unsigned char* pFrameAddr)
	{
		if (CurrentFrameFocusedAddr)
			*(unsigned char**)CurrentFrameFocusedAddr = pFrameAddr;
	}

	void CWar3Frame::ClearMenuItems()
	{

		if (!FrameOk)
			return;

		if (FrameType == CFrameType::FRAMETYPE_POPUPMENU)
		{
			ClearPopupMenu(FrameAddr);
		}
	}

	void CWar3Frame::Click()
	{
		if (!FrameOk)
			return;
		Wc3SimulateClickEvent(this->FrameAddr, 1);
	}


	void CWar3Frame::StartAnimate(int anim_id)
	{

		if (!FrameOk)
			return;
		if (AnimateStarted)
			return;

		if (FrameType == CFrameType::FRAMETYPE_SPRITE
			|| FrameType == CFrameType::FRAMETYPE_MODEL)
		{
			if (*(int*)(FrameAddr + 372))
			{
				int SpriteMdlAddr = **(int**)(FrameAddr + 376);
				if (SpriteMdlAddr)
				{
					StartSpriteModelAnimated(SpriteMdlAddr, anim_id, SetAnimOffset, FrameAddr, 0.0f);
				}
			}
			AnimateStarted = true;
		}
	}

	void CWar3Frame::StopAnimate()
	{

		if (!FrameOk)
			return;

		if (FrameType == CFrameType::FRAMETYPE_SPRITE
			|| FrameType == CFrameType::FRAMETYPE_MODEL)
		{
			if (*(int*)(FrameAddr + 372))
			{
				int SpriteMdlAddr = **(int**)(FrameAddr + 376);
				if (SpriteMdlAddr)
				{
					StartSpriteModelAnimated(SpriteMdlAddr, 0, 0, 0, 0.0f);
				}
			}
		}
		AnimateStarted = false;
	}

	void CWar3Frame::SetModel(const char* modelpath, int MdlType, int Flags)
	{

		if (!FrameOk)
			return;
		//CONSOLE_Print( "SetModel " );

		if (FrameType == CFrameType::FRAMETYPE_SPRITE)
		{
			int framevtable = *(int*)(FrameAddr);
			if (framevtable)
			{
				int SetSpriteFrameModel_addr = *(int*)(framevtable + 0xE8);
				if (SetSpriteFrameModel_addr)
				{
					auto SetSpriteFrameModel = (void(__thiscall*)(unsigned char* FrameAddr, const char* modelpath, int MdlType, unsigned int flag))(SetSpriteFrameModel_addr);
					if (SetSpriteFrameModel)
						SetSpriteFrameModel(FrameAddr, modelpath, MdlType, Flags);
				}

			}
		}
		else if (FrameType == CFrameType::FRAMETYPE_MODEL)
		{
			int framevtable = *(int*)(FrameAddr);
			if (framevtable)
			{
				int SetSpriteFrameModel_addr = *(int*)(framevtable + 0xEC);
				if (SetSpriteFrameModel_addr)
				{

					auto SetSpriteFrameModel = (void(__thiscall*)(unsigned char* FrameAddr, const char* modelpath, unsigned int flag))(SetSpriteFrameModel_addr);

					SetSpriteFrameModel(FrameAddr, modelpath, Flags);
				}

			}
		}

	}

	void CWar3Frame::Wc3PlaySound(const char* name)
	{
		//CONSOLE_Print( "Wc3PlaySound " );

		SoundPlay(0, name, 0, 0, 0, 0, 0, 0);
	}

	const char* CWar3Frame::GetText()
	{
		//CONSOLE_Print( "GetText " );


		if (!FrameOk)
			return "";

		int textaddr = 0;

		switch (FrameType)
		{
		case CFrameType::FRAMETYPE_FRAME:
			break;
		case CFrameType::FRAMETYPE_BACKDROP:
			break;
		case CFrameType::FRAMETYPE_ITEM:
			return *(const char**)(FrameAddr + 0x1E8) ? *(const char**)(FrameAddr + 0x1E8) : "";
		case CFrameType::FRAMETYPE_POPUPMENU: // Title
			textaddr = *(int*)(FrameAddr + 484); // sizeof CControl // + 684 for ? // + 392 for ?
			if (!textaddr)
				return "";
			textaddr = *(int*)(textaddr + 484); // sizeof CControl // + 684 for ? // + 392 for ?
			if (!textaddr)
				return "";

			return *(const char**)(textaddr + 0x1E8) ? *(const char**)(textaddr + 0x1E8) : "";
			break;
		case CFrameType::FRAMETYPE_EDITBOX:
			return *(const char**)(FrameAddr + 0x1E4) ? *(const char**)(FrameAddr + 0x1E4) : "";
			break;
		default:
			break;
		}

		return "";
	}

	// PopupMenu/Editbox/Textbox ... 
	void CWar3Frame::SetText(const char* text, unsigned int len)
	{
		//CONSOLE_Print( "SetText " );

		if (!FrameOk)
			return;

		unsigned char* TargetAddr = FrameAddr;



		switch (FrameType)
		{
		case CFrameType::FRAMETYPE_POPUPMENU: // Title
			TargetAddr = *(unsigned char**)(TargetAddr + 484); // sizeof CControl // + 684 for ? // + 392 for ?
			if (!TargetAddr)
				return;
			TargetAddr = *(unsigned char**)(TargetAddr + 484); // sizeof CControl // + 684 for ? // + 392 for ?
			if (!TargetAddr)
				return;
			/*	TargetAddr = *( int* )( TargetAddr + 484 );
			if ( !TargetAddr )
			return;*/
			break;
		case CFrameType::FRAMETYPE_EDITBOX: // Text
			if (SkipCallbackForInternalFunctions)
				SkipCallback = true;
			EditBoxFrameSetText(FrameAddr, text ? text : "", 1);
			if (SkipCallbackForInternalFunctions)
				SkipCallback = false;
			return;
		case CFrameType::FRAMETYPE_BUTTON: // Tip
			if (*(int*)(FrameAddr + 0x1E4))
			{
				Storm::MemFree(*(void**)(FrameAddr + 0x1E4));
			}
			if (!text)
				*(int*)(FrameAddr + 0x1E4) = 0;
			else
			{
				*(int*)(FrameAddr + 0x1E4) = (int)Storm::MemAlloc(len ? len : strlen(text) + 1);
				std::memcpy(*(void**)(FrameAddr + 0x1E4), text, len ? len : strlen(text) + 1);
			}
			return;
		case CFrameType::FRAMETYPE_TEXTBUTTON: // Tip
			if (*(int*)(FrameAddr + 0x1F8))
			{
				Storm::MemFree(*(void**)(FrameAddr + 0x1F8));
			}
			if (!text)
				*(int*)(FrameAddr + 0x1F8) = 0;
			else
			{
				*(int*)(FrameAddr + 0x1F8) = (int)Storm::MemAlloc(len ? len : strlen(text) + 1);
				std::memcpy(*(void**)(FrameAddr + 0x1F8), text, len ? len : strlen(text) + 1);
			}
			return;
		default:
			break;
		}
		if (SkipCallbackForInternalFunctions)
			SkipCallback = true;
		TextFrameSetText(TargetAddr, text ? text : "");
		if (SkipCallbackForInternalFunctions)
			SkipCallback = false;
	}

	unsigned int CWar3Frame::GetTextMaxLength()
	{
		if (!FrameOk)
			return 0;

		switch (FrameType)
		{
		case CFrameType::FRAMETYPE_FRAME:
			break;
		case CFrameType::FRAMETYPE_BACKDROP:
			break;
		case CFrameType::FRAMETYPE_ITEM:
			break;
		case CFrameType::FRAMETYPE_POPUPMENU:
			break;
		case CFrameType::FRAMETYPE_EDITBOX:
			return *(unsigned int*)(FrameAddr + 0x1E8);
			break;
		default:
			break;
		}

		return 0;
	}

	void CWar3Frame::Enable(bool enable)
	{
		//CONSOLE_Print( "Enable " );

		if (!FrameOk)
			return;
		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			if (enable)
			{
				if (!(*(unsigned int*)(FrameAddr + 0x1D4) & 1))
					*(unsigned int*)(FrameAddr + 0x1D4) += 1;
			}
			else
			{
				if ((*(unsigned int*)(FrameAddr + 0x1D4) & 1))
					*(unsigned int*)(FrameAddr + 0x1D4) -= 1;
			}
			UpdateFlagsV2();
			//UpdateFlags( FrameAddr, 0 );
		}
	}


	bool CWar3Frame::IsEnabled()
	{
		//CONSOLE_Print( "IsEnabled " );

		if (!FrameOk)
			return false;

		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			return (*(unsigned int*)(FrameAddr + 0x1D4) & 0x1);
		}
		return false;
	}


	bool CWar3Frame::IsFocused()
	{
		//CONSOLE_Print( "IsEnabled " );


		if (!FrameOk)
			return false;
		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			return (*(unsigned int*)(FrameAddr + 0x1D4) & 0x10);
		}
		return false;
	}

	void CWar3Frame::UpdateFlagsV2(unsigned int addflag)
	{
		if (SkipCallbackForInternalFunctions)
			SkipCallback = true;
		int framevtable = *(int*)(FrameAddr);
		if (framevtable)
		{
			int UpdateFlagsV2Addr = *(int*)(framevtable + 0xF0);
			if (UpdateFlagsV2Addr)
			{
				auto UpdateFlagsV2func = (int(__thiscall*)(unsigned char*, unsigned int))(UpdateFlagsV2Addr);
				UpdateFlagsV2func(FrameAddr, addflag);
			}
		}
		if (SkipCallbackForInternalFunctions)
			SkipCallback = false;
	}

	void CWar3Frame::UpdateFlagsV2Addr(unsigned char* FrameAddr, unsigned int addflag)
	{
		int framevtable = *(int*)(FrameAddr);
		if (framevtable)
		{
			int UpdateFlagsV2Addr = *(int*)(framevtable + 0xF0);
			if (UpdateFlagsV2Addr)
			{
				auto UpdateFlagsV2func = (int(__thiscall*)(unsigned char*, unsigned int))(UpdateFlagsV2Addr);
				UpdateFlagsV2func(FrameAddr, addflag);
			}
		}
	}

	void CWar3Frame::SetCursor(bool enabled)
	{

		if (!FrameOk)
			return;
		if (FrameType == CFrameType::FRAMETYPE_EDITBOX)
		{
			SetChecked(enabled);
			*(unsigned int*)(FrameAddr + 0x244) = 0;
			*(unsigned int*)(FrameAddr + 0x248) = enabled ? 1 : 0;
			UpdateFlagsV2();
		}
	}

	void CWar3Frame::SetFocus(bool focused)
	{

		if (!FrameOk)
			return;
		//sub_6F336C20
		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			Focused = focused;
			if (focused)
			{
				if (!(*(unsigned int*)(FrameAddr + 0x1D4) & 0x10))
					*(unsigned int*)(FrameAddr + 0x1D4) += 0x10;

			}
			else
			{
				if ((*(unsigned int*)(FrameAddr + 0x1D4) & 0x10))
					*(unsigned int*)(FrameAddr + 0x1D4) -= 0x10;
			}
			UpdateFlagsV2();
			//UpdateFlags( FrameAddr, 1 );
		}
		return;
	}

	bool CWar3Frame::IsPressed()
	{
		//CONSOLE_Print( "IsEnabled " );


		if (!FrameOk)
			return false;
		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			if (FrameType == CFrameType::FRAMETYPE_EDITBOX)
				return (*(unsigned int*)(FrameAddr + 0x1D4) & 0x8);
			return (*(unsigned int*)(FrameAddr + 0x1D4) & 0x4);
		}
		return false;
	}

	bool CWar3Frame::IsChecked()
	{
		//CONSOLE_Print( "IsChecked " );


		if (!FrameOk)
			return false;
		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			return (*(unsigned int*)(FrameAddr + 0x1D4) & 0x20);

		}
		return false;

	}

	int CWar3Frame::GetFrameCustomValue(int id)
	{

		if (!FrameOk)
			return 0;

		if (id > 10)
			id = 10;
		if (id < 1)
			id = 1;

		id--;


		return CustomValue[id];
	}

	void CWar3Frame::SetFrameCustomValue(int value, int id)
	{

		if (!FrameOk)
			return;

		if (id > 10)
			id = 10;
		if (id < 1)
			id = 1;

		id--;


		CustomValue[id] = value;
	}

	void CWar3Frame::SetChecked(bool checked)
	{
		//CONSOLE_Print( "SetChecked " );


		if (!FrameOk)
			return;
		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			if (checked)
			{
				if (!(*(unsigned int*)(FrameAddr + 0x1D4) & 0x20))
					*(unsigned int*)(FrameAddr + 0x1D4) += 0x20;
			}
			else
			{
				if ((*(unsigned int*)(FrameAddr + 0x1D4) & 0x20))
					*(unsigned int*)(FrameAddr + 0x1D4) -= 0x20;
			}
			UpdateFlagsV2();
			//UpdateFlags( FrameAddr, 0 );
		}
	}

	bool CWar3Frame::IsVisibled()
	{


		if (!FrameOk)
			return true;

		return Visibled;
	}

	void CWar3Frame::Show(bool enable)
	{
		//CONSOLE_Print( "Show " );

		if (!FrameOk)
			return;

		switch (FrameType)
		{
		case NWar3Frame::CFrameType::FRAMETYPE_FRAME:
		case NWar3Frame::CFrameType::FRAMETYPE_HIGHLIGHT:
			if (enable)
			{
				//*( int * )( FrameAddr + 0x168 ) = enable ? 2 : 1;
				gEvent.Event = CFrameEvent_ShowFrame;
				SimulateFrameEvent(FrameAddr, &gEvent);
				*(int*)(FrameAddr + 0xb0) = 44;
			}
			else
			{
				*(int*)(FrameAddr + 0xb0) = 0;
			}

			break;
		default:
			break;
		}

		Visibled = enable;

		/*	if ( enable )
		{
		if ( !( *( unsigned int* )( FrameAddr + 0x1D4 ) & 1 ) )
		*( unsigned int* )( FrameAddr + 0x1D4 ) += 1;
		}
		else
		{
		if ( ( *( unsigned int* )( FrameAddr + 0x1D4 ) & 1 ) )
		*( unsigned int* )( FrameAddr + 0x1D4 ) -= 1;
		}
		UpdateFlags( FrameAddr, 0 );*/
	}

	void CWar3Frame::SetCallbackFunc(int(*callback)(CWar3Frame* frame, unsigned char* FrameAddr, unsigned int EventId))
	{
		//CONSOLE_Print( "SetCallbackFunc " );

		if (!FrameOk)
			return;
		FrameCallback = callback;
	}

	void CWar3Frame::SetGlobalEventCallback(void(*globalEventCallback)())
	{
		GlobalEventCallback = globalEventCallback;
	}

	void CWar3Frame::SetChangeMenuEventCallback(void(*changeMenuEventCallback)())
	{
		ChangeMenuEventCallback = changeMenuEventCallback;
	}

	void CWar3Frame::Update(bool force)
	{
		//CONSOLE_Print( "Update " );

		if (!FrameOk)
			return;
		switch (FrameType)
		{
		case CFrameType::FRAMETYPE_FRAME:
			//UpdateFlags( FrameAddr + 180, force ? 1 : 0 );
			break;
		default:
			UpdateFlagsV2();
			//UpdateFlags( FrameAddr, force ? 1 : 0 );
			break;
		}
	}


	void CWar3Frame::AddItem(const char* name)
	{
		//CONSOLE_Print( "AddItem " );


		if (!FrameOk)
			return;
		if (FrameType == CFrameType::FRAMETYPE_POPUPMENU)
			PopupMenuAddItem(FrameAddr, name, -2);
		//int __cdecl sub_6F613260(CPopupMenu *a1, char *a2, int a3)
	}

	// EDITBOX 
	void CWar3Frame::SetMaxLen(unsigned int  maxlen)
	{
		//CONSOLE_Print( "SetMaxLen " );

		if (!FrameOk)
			return;
		if (FrameType != CFrameType::FRAMETYPE_FRAME)
		{
			EditboxSetMaxLen(FrameAddr, maxlen);
		}
	}

	std::string CWar3Frame::DumpAllFrames()
	{
		std::string FramesDmp = "Frames:\n";
		char dmpbuf[512];

		for (auto frame : FramesList)
		{
			if (frame && frame->FrameName.size() > 0)
			{
				sprintf_s(dmpbuf, "Name: %s (%i). Addr: %X. Callbacks:%u. Status:%s \n", frame->FrameName.c_str(), frame->FrameId, (unsigned long) frame->FrameAddr, frame->RegisteredEventId.size(), frame->CheckIsOk() ? "Okay" : "Error");
				FramesDmp += dmpbuf;
			}
		}

		return FramesDmp;
	}

	void CWar3Frame::SetFrameAbsolutePosition(CFramePosition orginPosition, float absoluteX, float absoluteY, unsigned int flag)
	{
		//CONSOLE_Print( "SetFrameAbsolutePosition " );

		if (!FrameOk)
			return;
		switch (FrameType)
		{
		case CFrameType::FRAMETYPE_FRAME:
			SetAbsolutePosition(FrameAddr + 180, orginPosition, absoluteX, absoluteY, flag);
			UpdatePosition(FrameAddr + 180, 1);
			break;
		default:
			SetAbsolutePosition(FrameAddr, orginPosition, absoluteX, absoluteY, flag);
			UpdatePosition(FrameAddr, 1);
			break;
		}

	}

	void CWar3Frame::SetFrameRelativePosition(CFramePosition orginPosition, int dstFrameAddr, CFramePosition toPosition, float relativeX, float relativeY, unsigned int flag)
	{

		if (!FrameOk)
			return;
		switch (FrameType)
		{
		case CFrameType::FRAMETYPE_FRAME:
			SetRelativePosition(FrameAddr + 180, orginPosition, dstFrameAddr + 180, toPosition, relativeX, relativeY, flag);
			UpdatePosition(FrameAddr + 180, 1);
			break;
		default:
			SetRelativePosition(FrameAddr, orginPosition, dstFrameAddr, toPosition, relativeX, relativeY, flag);
			UpdatePosition(FrameAddr, 1);
			break;
		}

	}

	void CWar3Frame::SetTexture(const char* path, const char* border, bool tiled, CFrameBackdropType backgtype)
	{
		//CONSOLE_Print( "SetTexture " );


		if (!FrameOk)
			return;

		unsigned char* BackdropAddr = FrameAddr;

		switch (backgtype)
		{
		case NWar3Frame::CFrameBackdropType::ControlBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1BC);
			break;
		case NWar3Frame::CFrameBackdropType::ControlPushedBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1C0);
			break;
		case NWar3Frame::CFrameBackdropType::ControlDisabledBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1C4);
			break;
		default:
			break;
		}



		switch (FrameType)
		{
		case CFrameType::FRAMETYPE_FRAME:
			break;
		case CFrameType::FRAMETYPE_BACKDROP:
			if (*(int*)(BackdropAddr + 0x44))
				SetFrameTexture(*(unsigned char**)(BackdropAddr + 0x44), path, 0, tiled ? 1 : 0, border, 1);
			else if (*(int*)(BackdropAddr + 0x68))
				SetFrameTexture(*(unsigned char**)(BackdropAddr + 0x68), path, 0, tiled ? 1 : 0, border, 1);
			break;
		case CFrameType::FRAMETYPE_ITEM:

			if (*(int*)(BackdropAddr + 0x20))
				SetFrameTexture(*(unsigned char**)(BackdropAddr + 0x20), path, 0, tiled ? 1 : 0, border, 1);
			break;
		default:
			break;
		}

	}

	unsigned char* CWar3Frame::GetFrameBackdropAddress(CFrameBackdropType backtype)
	{

		if (!FrameOk)
			return 0;

		unsigned char* BackdropAddr = FrameAddr;

		switch (backtype)
		{
		case NWar3Frame::CFrameBackdropType::ControlBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1BC);
			break;
		case NWar3Frame::CFrameBackdropType::ControlPushedBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1C0);
			break;
		case NWar3Frame::CFrameBackdropType::ControlDisabledBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1C4);
			break;
		default:
			break;
		}

		return BackdropAddr;
	}

	void CWar3Frame::FillToParentFrame(CFrameBackdropType backtype, bool fill)
	{

		if (!FrameOk)
			return;

		unsigned char* BackdropAddr = FrameAddr;

		switch (backtype)
		{
		case NWar3Frame::CFrameBackdropType::ControlBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1BC);
			break;
		case NWar3Frame::CFrameBackdropType::ControlPushedBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1C0);
			break;
		case NWar3Frame::CFrameBackdropType::ControlDisabledBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1C4);
			break;
		default:
			break;
		}

		if (BackdropAddr)
		{
			*(int*)(BackdropAddr + 0x108) = fill ? 1 : 0;
			UpdateFlagsV2();
		}
	}

	void CWar3Frame::UpdateScale()
	{
		if (!FrameOk)
			return;

		//int UpdateLayerAddr = FrameAddr + 0xB4;
		//UpdateFrameScale( UpdateLayerAddr );
	}

	void CWar3Frame::SetFrameScale(CFrameBackdropType backtype, float xscale, float yscale)
	{
		// FrameAddr + 0xB4 UpdateScale
		// 
		if (!FrameOk)
			return;

		unsigned char* BackdropAddr = FrameAddr;

		switch (backtype)
		{
		case NWar3Frame::CFrameBackdropType::ControlBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1BC);
			break;
		case NWar3Frame::CFrameBackdropType::ControlPushedBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1C0);
			break;
		case NWar3Frame::CFrameBackdropType::ControlDisabledBackdrop:
			BackdropAddr = *(unsigned char**)(BackdropAddr + 0x1C4);
			break;
		default:
			break;
		}

		if (BackdropAddr)
		{

			SetFrameWidth(BackdropAddr, xscale);
			SetFrameHeight(BackdropAddr, yscale);
			UpdateFlagsV2();

			//UpdateFrameScale( BackdropAddr + 0xB4);
			//UpdatePosition( FrameAddr, 0 );
			//UpdateFlags( BackdropAddr );
		}

	}

	void CWar3Frame::InitCallbackHook()
	{
		//CONSOLE_Print( "InitCallbackHook " );

		//	MessageBoxA( 0, "1", "1", 0 );
		if (CFrameCallbackInitialized)
		{
			//	MessageBoxA( 0, "InitCallbackHook Allready hooked", "Error", 0 );
			return;
		}

		if (!CFrameInitialized)
		{
			//MessageBoxA( 0, "InitCallbackHook", "Error", 0 );
			return;
		}

		//	char tmps[ 500 ];

		//MH_CreateHook( FrameEventHandler, &FrameEventCallback, reinterpret_cast< void** >( &FrameEventHandler_ptr ) );
		MH_CreateHook(FrameEventHandler, &FrameEventCallback, reinterpret_cast<void**>(&FrameEventHandler_ptr));
		MH_EnableHook(FrameEventHandler);
		MH_CreateHook(Wc3ChangeMenu, &Wc3ChangeMenu_my, reinterpret_cast<void**>(&Wc3ChangeMenu_ptr));
		MH_EnableHook(Wc3ChangeMenu);
		MH_CreateHook(Wc3GetSkinItemPath, &Wc3GetSkinItemPath_my, reinterpret_cast<void**>(&Wc3GetSkinItemPath_ptr));
		MH_EnableHook(Wc3GetSkinItemPath);

		CFrameCallbackInitialized = true;
	}
	void CWar3Frame::UninitializeCallbackHook()
	{
		//CONSOLE_Print( "UninitializeCallbackHook " );

		if (!CFrameCallbackInitialized)
		{
			return;
		}

		if (!CFrameInitialized)
		{
			//MessageBoxA( 0, "InitCallbackHook", "Error", 0 );
			return;
		}

		//	char tmps[ 50 ];
		//	sprintf_s( tmps, "Uninit Callback Func:%X ", &FrameEventCallback );
		//	MessageBoxA( 0, tmps, "1", 0 );
		if (FrameEventHandler)
			MH_DisableHook(FrameEventHandler);
		FrameEventHandler = NULL;

		//	MH_DisableHook( FixFrameHookError1 );
		if (Wc3ChangeMenu)
			MH_DisableHook(Wc3ChangeMenu);
		Wc3ChangeMenu = NULL;
		if (Wc3GetSkinItemPath)
			MH_DisableHook(Wc3GetSkinItemPath);
		Wc3GetSkinItemPath = NULL;

		CFrameCallbackInitialized = false;
	}

	void CWar3Frame::LoadFrameDefFiles(const char* filename, bool force)
	{
		//CONSOLE_Print( "LoadFrameDefFiles " );

		if (!force)
		{
			for (auto & s : LoadedFramedefFiles)
			{
				if (filename == s)
					return;
			}
		}

		if (!LoadFrameDefFile(filename, CStatusLoadFramesVar1, CStatusLoadFramesVar2, CStatusDefaultCStatus))
		{
			MessageBoxA(0, "Invalid toc file!", "Error", 0);
		}
		else
		{
			LoadedFramedefFiles.push_back(filename);
		}
	}

	bool CWar3Frame::CheckIsOk()
	{
		//CONSOLE_Print( "CheckIsOk " );


		if (!GetFrameItem(FrameName.c_str(), FrameId))
		{
			FrameOk = false;
			return false;
		}

		return true;
	}

	unsigned char* CWar3Frame::Load(const char* name, int id, bool showerror)
	{
		//CONSOLE_Print( "Load " );
		this->~CWar3Frame();

		memset(CustomValue, 0, 4 * 10);
		FrameOk = false;
		FramesCount++;

		FrameId = id;
		FrameAddr = GetFrameItem(name, id);
		FrameName = name ? name : "";
		if (!FrameAddr && showerror)
		{
			MessageBoxA(0, (FrameName + std::string(":") + std::to_string(FrameId)).c_str(), "Invalid frame name:", 0);
		}
		RegisteredEventId = std::vector<unsigned int>(100);
		RegisteredEventId.clear();
		FrameDestroyable = false;

		//FrameType = CFrameType::FRAMETYPE_ITEM;
		if (FrameAddr && FrameName.length() > 0)
		{
			FrameOk = true;
		}

		FramesList.push_back(this);

		return FrameAddr;
	}

	void CWar3Frame::SetFrameType(CFrameType newframetype)
	{
		//CONSOLE_Print( "SetFrameType " );

		FrameType = newframetype;
	}

	// Были внесены изменения!
	CWar3Frame::CWar3Frame(const char* name, int id, bool show, unsigned char* relativeframe, bool showerror)
	{
		//CONSOLE_Print( "Create Frame 3" );
		this->~CWar3Frame();

		//CONSOLE_Print( "CWar3Frame " );
		Focused = false;
		Pressed = false;
		memset(CustomValue, 0, 4 * 10);
		SkipOtherEvents = false;
		FrameOk = false;
		Visibled = true;
		FramesCount++;
		FrameId = id;
		//char debug[ 220 ];
		//sprintf_s( debug, "%s -> %X -> %X -> %X", name, relativeframe, GetCurrentFrameAddr( ),GetGlobalClassAddr( ) );
		//MessageBoxA( 0, debug, debug, 0 );
		//
		FrameName = name ? name : "";

		FrameAddr = CreateNewCFrame(name, relativeframe ? relativeframe : GetCurrentFrameAddr(), 0, 0, id);
		//MessageBoxA( 0, "Create OK", "Create OK", 0 );


		RegisteredEventId = std::vector<unsigned int>(100);
		RegisteredEventId.clear();
		if (FrameAddr && FrameName.length() > 0)
		{
			FrameOk = true;
		}
		if (show && FrameAddr)
		{
			Show(true);
		}
		else if (!FrameAddr && showerror)
		{
			MessageBoxA(0, (FrameName + std::string(":") + std::to_string(FrameId)).c_str(), "Invalid frame name:", 0);
		}

		FrameDestroyable = true;
		FrameType = CFrameType::FRAMETYPE_FRAME;
		FramesList.push_back(this);
	}


	unsigned char* CWar3Frame::CWar3FrameFromAddress(unsigned char* FrameAddr, bool show, bool showerror)
	{
		this->~CWar3Frame();

		//CONSOLE_Print( "CWar3Frame 5 " );
		Focused = false;
		Pressed = false;
		Visibled = true;
		memset(CustomValue, 0, 4 * 10);
		SkipOtherEvents = false;
		FrameOk = false;
		FramesCount++;
		FrameId = 0;
		this->FrameAddr = FrameAddr;
		FrameType = CFrameType::FRAMETYPE_ITEM;
		FrameName = "Frame_" + std::to_string(FrameId);
		RegisteredEventId = std::vector<unsigned int>(100);
		RegisteredEventId.clear();
		if (FrameAddr)
		{
			FrameOk = true;
		}


		if (show && FrameAddr)
		{
			Show(true);
		}
		else if (!FrameAddr && showerror)
		{
			MessageBoxA(0, (FrameName + std::string(":") + std::to_string(FrameId)).c_str(), "Invalid frame addr:", 0);
		}




		FrameDestroyable = false;
		FramesList.push_back(this);
		return FrameAddr;
	}

	void CWar3Frame::DestroyThisFrame()
	{
		SkipCallbackForInternalFunctions = false;
		//CONSOLE_Print( "CWar3Frame 2 " );
		Focused = false;
		Pressed = false;
		RegisteredEventId.clear();

		if (FrameOk)
		{
			FrameOk = false;
			if (FrameDestroyable)
			{
				DestroyCFrame(FrameAddr, 1);
				FrameDestroyable = false;
			}

		}
		this->~CWar3Frame();
	}

	CWar3Frame::CWar3Frame()
	{
		this->~CWar3Frame();
		SkipCallbackForInternalFunctions = false;
		SkipCallback = false;
		//CONSOLE_Print( "CWar3Frame 3 " );
		Focused = false;
		Visibled = false;
		Pressed = false;
		memset(CustomValue, 0, 4 * 10);
		FrameOk = false;
		SkipOtherEvents = false;

		FrameType = CFrameType::FRAMETYPE_ITEM;
	};

	void CWar3Frame::SetAnimOffset(float off)
	{
		if (!FrameOk)
			return;
		anim_offset = off;
	}

	void CWar3Frame::SetTextFrameFontHeight(float y)
	{
		if (!FrameOk)
			return;
		if (FrameType == CFrameType::FRAMETYPE_ITEM)
		{
			*(float*)(FrameAddr + 0x210) = y;
			UpdateFlagsV2();
		}

	}

	void CWar3Frame::SetTextFrameFontWidth(float x)
	{
		if (!FrameOk)
			return;
		if (FrameType == CFrameType::FRAMETYPE_ITEM)
		{
			*(float*)(FrameAddr + 0x214) = x;
			UpdateFlagsV2();
		}

	}

	CWar3Frame::~CWar3Frame()
	{
		if (CurrentFrameFocusedAddr && *(unsigned char**)CurrentFrameFocusedAddr == this->FrameAddr)
		{
			*(int*)CurrentFrameFocusedAddr = 0;
		}
		anim_offset = 0.0f;
		AnimateStarted = false;
		SkipCallbackForInternalFunctions = false;
		SkipCallback = false;
		//CONSOLE_Print( "~~~~CWar3Frame " );
		RegisteredEventId.clear();
		Focused = false;
		Visibled = false;
		Pressed = false;
		FrameOk = false;
		for (unsigned int i = 0; i < FramesList.size() && i >= 0; i++)
		{
			if (this == FramesList[i])
			{
				FramesList.erase(FramesList.begin() + i);
				i--;
			}
		}
	}


	//
	///* need research */
	//
	///* Example */
	//static void StormTest( )
	//{
	//	//Init after load default fdf file
	//
	//	/* Init all functions (address etc) */
	//	CWar3Frame::Init( 0, GameDll );
	//	/* Init callback feature */
	//	CWar3Frame::InitCallbackHook( );
	//	/* Load custom FrameDef.toc file(list of fdf files) */
	//	CWar3Frame::LoadFrameDefFiles( "CustomFrameDef.toc" );
	//	/* Load main frame from fdf*/
	//	auto mainframe = new CWar3Frame( "MainFrame", false );
	//	/* Set MainFrame position (from left bottom point) and show this frame */
	//	mainframe->SetFrameAbsolutePosition( CFramePosition::BOTTOM_LEFT, 0.0, 0.0 );
	//	/* Destroy frame */
	//	mainframe->DestroyThisFrame( ); // aviable only for Frame ? 
	//}
}