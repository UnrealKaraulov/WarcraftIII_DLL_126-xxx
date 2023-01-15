#include "stdafx.h"
#ifndef UI_SIMPLEREGION_H_INCLUDED
#define UI_SIMPLEREGION_H_INCLUDED

#include "UILayoutFrame.h"

class UISimpleRegion : public UILayoutFrame {
public:
	UISimpleRegion(void* base = NULL);
	void setParent(UIObject* parent);
	void setColor(unsigned int);
	void setColorFloat(float r, float g, float b, float alpha);
	void setColorFloatAlpha(float alpha);
private:
};

#endif