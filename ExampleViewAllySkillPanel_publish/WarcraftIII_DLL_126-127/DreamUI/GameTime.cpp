#include "stdafx.h"
#include "GameTime.h"

static uint32_t TimeUint = 0;
static float TimeFloat = 0;

void GameTime_Reset() {
	TimeUint = 0;
	TimeFloat = 0;
}

void GameTime_Update(uint32_t time) {
	TimeUint = time;
	TimeFloat = static_cast<float>(time) / 1000.f;
}

float Time() {
	return TimeFloat;
}

uint32_t TimeRaw() {
	return TimeUint;
}