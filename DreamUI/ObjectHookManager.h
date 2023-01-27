#pragma once

#include "stdafx.h"

struct ObjectHookInfo {
	unsigned int methodCount;
	void* mem;
	void** realVtable;
	void** fakeVtable;
	bool isDynamic;
	bool autoCleanup;
};

typedef std::map<void*, ObjectHookInfo> ObjectHookInfoMapType;

class ObjectHookManager {
public:
	ObjectHookManager();
	~ObjectHookManager();

	void** replace(void* object, unsigned int vtableLength, bool dynamic = false);
	void** replace(void* object, void** vtable, unsigned int vtableLength);
	void* apply(void* object, unsigned int offset, void* detour);
	void cancel(void* object, unsigned int offset);
	void restore(void* object);
	void clearAllDynamic();
	void* getOrignal(void* object, unsigned int offset);
	ObjectHookInfo* getObjectHookInfo(void* object);
	void** analysis(void* object, unsigned int vtableLength);
private:
	ObjectHookInfoMapType map_;
};

ObjectHookManager* GetObjectHookManager();
void ObjectHookManager_Init();
void ObjectHookManager_Cleanup();

