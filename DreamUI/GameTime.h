#ifndef GAME_TIME_MANAGER_H_INCLUDED_
#define GAME_TIME_MANAGER_H_INCLUDED_

#include <cstdint>

void GameTime_Reset();
void GameTime_Update(unsigned int time);

float Time();
unsigned int TimeRaw();

#endif