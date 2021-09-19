#include "stdafx.h"
#ifndef UISTATBAR_H_INCLUDED
#define UISTATBAR_H_INCLUDED
#include "UISimpleStatusBar.h"

class UIStatBar : public UISimpleStatusBar {
public:
	static UIStatBar* Create(UISimpleFrame* parent = NULL);
	static void Destroy(UIStatBar* obj);
private:
	uint32_t setOwner(war3::CUnit* unit);
	uint32_t updateStat();
};

#endif