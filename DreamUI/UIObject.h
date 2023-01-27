#include "stdafx.h"
#ifndef UI_OBJECT_H_INCLUDED
#define UI_OBJECT_H_INCLUDED

#include "War3ClassWrapper.h"
#include "UIStructs.h"
class UISimpleFrame;

class UIObject : public War3ClassWrapper {
public:
	static const char* GetPathByName(const char* name, const char* type = NULL);
	static UISimpleFrame* GetGameSimpleConsole();
	UISimpleFrame* CreateGameSimpleConsole();
	static UISimpleFrame* GetMinimapFrame();

	UIObject() = default;

};

#endif