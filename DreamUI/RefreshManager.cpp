#include "stdafx.h"
#include "RefreshManager.h"
#include "GameStructs.h"
#include "UI.h"
#include "Tools.h"
#include "Benchmark.h"
#include "GameTime.h"

typedef std::list<RefreshObject*> RefreshObjectListType;
static RefreshObjectListType RefreshObjectList;

void RefreshManager_Init() {

}

void RefreshManager_AddObject(RefreshObject* object) {
	RefreshObjectList.push_back(object);
}

//退出时的清理
void RefreshManager_Cleanup() {
	for (RefreshObjectListType::iterator iter = RefreshObjectList.begin();
		iter != RefreshObjectList.end();
		++iter)
	{
		delete* iter;
	}
	RefreshObjectList.clear();
}

//游戏结束时的清理
void RefreshManager_CleanupGame() {
	for (RefreshObjectListType::iterator iter = RefreshObjectList.begin();
		iter != RefreshObjectList.end();)
	{
		if ((*iter)->isInGameObject()) {
			delete* iter;
			RefreshObjectList.erase(iter++);
			continue;
		}
		iter++;
	}
}

#ifdef _DEBUG
Benchmark b;
#endif
void RefreshManager_Update() {
#ifdef _DEBUG 
	b.mark("1");
#endif
	for (RefreshObjectListType::iterator iter = RefreshObjectList.begin();
		iter != RefreshObjectList.end();)
	{
		if ((*iter)->isCompleted()) {
			delete* iter;
			RefreshObjectList.erase(iter++);
			continue;
		}
		(*(iter++))->refresh();
	}
#ifdef _DEBUG 
	b.mark("2");
	//;//OutputDebug("Object count: %u, Refresh Time Cost: %.4f", RefreshObjectList.size(), b.elapsedTime("1", "2")); 
#endif
}