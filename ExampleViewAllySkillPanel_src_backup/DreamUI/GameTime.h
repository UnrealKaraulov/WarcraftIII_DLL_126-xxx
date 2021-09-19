#ifndef GAME_TIME_MANAGER_H_INCLUDED_
#define GAME_TIME_MANAGER_H_INCLUDED_

#include <cstdint>

void GameTime_Reset();
void GameTime_Update(uint32_t time);

float Time();
uint32_t TimeRaw();

#endif