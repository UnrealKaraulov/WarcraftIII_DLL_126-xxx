#include "stdafx.h"
#ifndef UI_SIMPLESTATUSBAR_H_INCLUDED
#define UI_SIMPLESTATUSBAR_H_INCLUDED
#include "UISimpleFrame.h"

class UISimpleStatusBar : public UISimpleFrame {
public:
	static UISimpleStatusBar* Create(UISimpleFrame* parent = NULL);
	static void Destroy(UISimpleStatusBar* obj);

	UISimpleStatusBar(void* base = NULL) : UISimpleFrame(base) {

	}

	void setColor(float r, float g, float b, float a);
	void setTexture(const char* path);
	void setValueRange(float min, float max);
	void setValue(float value);
	void setValueUpdated();
};

#endif