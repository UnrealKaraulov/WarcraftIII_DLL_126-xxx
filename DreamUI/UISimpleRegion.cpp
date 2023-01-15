#include "stdafx.h"
#include "UISimpleRegion.h"
#include "UISimpleFrame.h"

UISimpleRegion::UISimpleRegion(void* base) : UILayoutFrame(base) {

}

void UISimpleRegion::setColor(unsigned int color) {
	war3::CSimpleRegion* baseObj = this->base<war3::CSimpleRegion*>();
	if (baseObj) {
		dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLEREGION_SETCOLOR),
			baseObj,
			&color
			);
	}
}

void UISimpleRegion::setColorFloat(float r, float g, float b, float alpha) {
	setColor(
		((unsigned int)(alpha * 255) << 0x18) +
		((unsigned int)(r * 255) << 0x10) +
		((unsigned int)(g * 255) << 0x8) +
		((unsigned int)(b * 255))
	);
}

void UISimpleRegion::setColorFloatAlpha(float alpha) {
	war3::CSimpleRegion* baseObj = this->base<war3::CSimpleRegion*>();
	unsigned int currentR = baseObj->colorRed;
	unsigned int currentG = baseObj->colorGreen;
	unsigned int currentB = baseObj->colorBlue;
	setColor(
		((unsigned int)(alpha * 255) << 0x18) +
		(currentR << 0x10) +
		(currentG << 0x8) +
		(currentB)
	);
}

void UISimpleRegion::setParent(UIObject* parent) {
	war3::CSimpleRegion* baseObj = this->base<war3::CSimpleRegion*>();
	if (baseObj) {
		dreamaero::generic_this_call<void>(
			Offset(SIMPLEREGION_SETPARENT),
			baseObj,
			parent ? parent->base() : NULL,
			2,	//TODO ??
			1	//TODO ??
			);
	}
}