#include "stdafx.h"
#include "GameTime.h"

static unsigned int TimeUint = 0;
static float TimeFloat = 0;

void GameTime_Reset() {
	TimeUint = 0;
	TimeFloat = 0;
}

void GameTime_Update(unsigned int time) {
	TimeUint = time;
	TimeFloat = static_cast<float>(time) / 1000.f;
}

float Time() {
	return TimeFloat;
}

unsigned int TimeRaw() {
	return TimeUint;
}