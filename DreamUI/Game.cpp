#include "stdafx.h"
#include "Game.h"
#include "EventDispatcher.h"
#include "Tools.h"
#include "Timer.h"
#include "Hook.h"
#include "RefreshManager.h"
#include "GameEventObserver.h"
#include "ToolTip.h"
#include "HotkeyButton.h"
#include "EditBoxButton.h"
#include "Button.h"
#include "InfoPanelRow.h"//MUSTDO
#include "IUIObject.h"

static EventDispatcher* Dispatcher = new EventDispatcher();
static Event GameStateStart(EVENT_GAME_START);
static Event  GameStateEnd(EVENT_GAME_END);

void onGlobalTimerCheck(Timer* tm) {
#ifndef _FREEPLUGIN
	//VMProtectBeginVirtualization("GlobalOverdateTimer");
	//if (isOverdate()){
	//	if (RandomFloat(0, 1.f) <= 0.05f){
	//		Jass::CreateTimer();
	//		tm->destroy();
	//	}
	//}
	//VMProtectEnd();
#endif
}

typedef void(*initfunctype) ();

static initfunctype GameInitFuncArr[8] =
{
	Timer_Init
};

Game::Game() {
	//基础组件初始化
	if (!Dispatcher)
		Dispatcher = new EventDispatcher();
	Timer_Init();
	UI_Init();
	Button::Init();
	ToolTip_Init();//TODO
	HotkeyButton::Init();
	EditBoxButton::Init();

	GetTimer(RandomFloat(15.f, 30.f), onGlobalTimerCheck, true)->start();


	MainDispatcher()->dispatch(&GameStateStart);
}

Game::~Game() {
	//功能模块清理

	MainDispatcher()->dispatch(&GameStateEnd);
	//基础组件清理
	Button::Cleanup();
	ToolTip_Cleanup();//TODO
	HotkeyButton::Cleanup();
	EditBoxButton::Cleanup();

	UI_Cleanup();

	//RefreshManager_CleanupGame();
	Timer_Cleanup();
	delete Dispatcher;
	Dispatcher = NULL;
}


static Game* CurrentGameObject = NULL;
static unsigned int LastRecordedTime = 0;
static bool GameStarted = false;

Game* CurrentGame() {
	return CurrentGameObject;
}

void Game_Init() {
	CurrentGameObject = new Game();
	GameStarted = true;
	GameEventObserver_ProcessPreGameEvents();

}

void Game_Cleanup() {
	//因为GameLeave事件从玩家列表退出时也会触发
	if (GameStarted) {
		GameStarted = false;
		delete CurrentGameObject;
	}
}

bool IsInGame() {
	return GameStarted;
}

EventDispatcher* MainDispatcher() {
	if (Dispatcher == NULL)
		Dispatcher = new EventDispatcher();
	return Dispatcher;
}
