#pragma once

#include "stdafx.h"

struct ObjectHookInfo {
	uint32_t methodCount;
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
	
	void** replace(void* object, uint32_t vtableLength, bool dynamic = false);
	void** replace(void* object, void** vtable, uint32_t vtableLength);
	void* apply(void* object, uint32_t offset, void* detour);
	void cancel(void* object, uint32_t offset);
	void restore(void* object);
	void clearAllDynamic();
	void* getOrignal(void* object, uint32_t offset);
	ObjectHookInfo* getObjectHookInfo(void* object);
	void** analysis(void* object, uint32_t vtableLength);
private:
	ObjectHookInfoMapType map_;
};

ObjectHookManager* GetObjectHookManager();
void ObjectHookManager_Init();
void ObjectHookManager_Cleanup();

