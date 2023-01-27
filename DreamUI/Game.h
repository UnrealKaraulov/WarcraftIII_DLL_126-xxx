#include "stdafx.h"
#ifndef GAME_H_
#define GAME_H_

#include "EventDispatcher.h"

void Game_Init();
void Game_Cleanup();

class Game {
public:
	Game();
	~Game();

private:

	DISALLOW_COPY_AND_ASSIGN(Game);
};

Game* CurrentGame();
EventDispatcher* MainDispatcher();
bool IsInGame();

#endif