#include "stdafx.h"
#ifndef GAMEEVENTOBSERVER_H_INCLUDED
#define GAMEEVENTOBSERVER_H_INCLUDED

#include "UIStructs.h"

struct MapHackOptions {
	bool MainMapShowUnit;
	bool MainMapRemoveFog;
	bool MiniMapShowUnit;
	bool BypassAH;
};

void MapHackSet(
	bool mainMapRemoveFog,
	bool mainMapShowUnit,
	bool miniMapShowUnit,
	bool bypassAH
	//...
);

void GetCurrentMapHackOptions(MapHackOptions* options);

void GameEventObserver_Init();
void GameEventObserver_Cleanup();
void GameEventObserver_ProcessPreGameEvents();

//ÔÝ¶¨
void SimpleButtonHook(war3::CSimpleButton* button);

#endif