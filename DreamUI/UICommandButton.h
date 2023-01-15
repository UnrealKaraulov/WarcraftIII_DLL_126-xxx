#pragma once
#include "stdafx.h"
#ifndef UI_COMMANDBUTTON_H_INCLUDED
#define UI_COMMANDBUTTON_H_INCLUDED

#include "UIStructs.h"
#include "UISimpleButton.h"
#include "UISimpleFrame.h"
#include "UISimpleTexture.h"
#include "UISimpleFontString.h"
#include "EventDispatcher.h"
#include "Observer.h"
#include "Event.h"
#include "IUIObject.h"
#include <string>

#include "UISimpleRegion.h"

class UICommandButton : public UISimpleButton {
public:
	static UICommandButton* Create(UISimpleFrame* parent = NULL);
	static void Destroy(UICommandButton* t);
	UICommandButton(war3::CSimpleTexture* base = NULL) : UISimpleButton(base) {}
};

#endif