#include "stdafx.h"
#ifndef FOUNDATION_H_INCLUDED
#define FOUNDATION_H_INCLUDED

//#ifdef _VMP
//#pragma comment(lib, "../Release/DreamProtect.lib")
//#else
//#pragma comment(lib, "../Debug/DreamProtect.lib")
//#endif


extern char RootPath[MAX_PATH];

void Init(void* _GameDll);
void Cleanup();

int GetGameVersion();

#endif