#include "stdafx.h"
#include "UISimpleRegion.h"
#include "UISimpleFrame.h"

UISimpleRegion::UISimpleRegion(void* base) : UILayoutFrame(base) {

}

void UISimpleRegion::setColor(uint32_t color) {
	war3::CSimpleRegion* baseObj = this->base<war3::CSimpleRegion*>();
	if (baseObj) {
		dreamaero::generic_this_call<uint32_t>(
			Offset(SIMPLEREGION_SETCOLOR), 
			baseObj,
			&color
		);
	}
}

void UISimpleRegion::setColorFloat(float r, float g, float b, float alpha) {
	setColor(
		((uint32_t)(alpha*255)<<0x18) +
		((uint32_t)(r*255)<<0x10) +
		((uint32_t)(g*255)<<0x8) +
		((uint32_t)(b*255))
	);
}

void UISimpleRegion::setColorFloatAlpha(float alpha) {
	war3::CSimpleRegion* baseObj = this->base<war3::CSimpleRegion*>();
	uint32_t currentR = baseObj->colorRed;
	uint32_t currentG = baseObj->colorGreen;
	uint32_t currentB = baseObj->colorBlue;
	setColor(
		((uint32_t)(alpha*255)<<0x18) +
		(currentR<<0x10) +
		(currentG<<0x8) +
		(currentB)
	);
}

void UISimpleRegion::setParent(UIObject *parent) {
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