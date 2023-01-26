#include "stdafx.h"
#ifndef REFRESH_MANAGER_H_INCLUDED
#define REFRESH_MANAGER_H_INCLUDED

class RefreshObject {
public:
	RefreshObject(bool inGame = true) {
		this->isInGameObject_ = inGame;
	}
	bool isInGameObject() { return this->isInGameObject_; }
	virtual void refresh() = 0;
	virtual bool isCompleted() = 0;
	virtual ~RefreshObject() {

	};
private:
	bool isInGameObject_;
};

void RefreshManager_Init();
void RefreshManager_Cleanup();
void RefreshManager_CleanupGame();
void RefreshManager_Update();
void RefreshManager_AddObject(RefreshObject* object);

#endif