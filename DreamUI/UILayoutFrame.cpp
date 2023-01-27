#include "stdafx.h"
#include "UILayoutFrame.h"

#include "Offsets.h"
void UILayoutFrame::setAbsolutePosition(unsigned int originPos, float absoluteX, float absoluteY) {
	dreamaero::generic_this_call<war3::CFramePointAbsolute*>(
		Offset(LAYOUTFRAME_SETABSPOS),
		this->base<war3::CLayoutFrame*>(this->baseLayoutFrameOffset_),
		originPos,
		absoluteX,
		absoluteY,
		1			//TODO 搞清楚这个是什么
		);
}

void UILayoutFrame::setRelativePosition(unsigned int originPos, UILayoutFrame* target, unsigned int toPos, float relativeX, float relativeY) {
	float x = relativeX / War3WindowRatioX();
	dreamaero::generic_this_call<war3::FRAMENODE*>(
		Offset(LAYOUTFRAME_SETRELPOS),
		this->base<war3::CLayoutFrame*>(this->baseLayoutFrameOffset_),
		originPos,
		target->baseLayoutFrame(),
		toPos,
		x,//relativeX,
		relativeY,
		1			//TODO 搞清楚这个是什么
		);
}

void UILayoutFrame::setRelativeObject(UILayoutFrame* target) {
	dreamaero::generic_this_call<war3::FRAMENODE*>(
		Offset(LAYOUTFRAME_SETRELFRAME),
		this->base<war3::CLayoutFrame*>(this->baseLayoutFrameOffset_),
		target->baseLayoutFrame(),
		1			//TODO 搞清楚这个是什么
		);
}

void UILayoutFrame::applyPosition() {
	dreamaero::generic_this_call<unsigned int>(
		Offset(LAYOUTFRAME_UPDATEPOSITION),
		this->base<war3::CLayoutFrame*>(this->baseLayoutFrameOffset_),
		1			//TODO 搞清楚这个是什么
		);
}

void UILayoutFrame::Focus()
{
	*(unsigned int*)Offset(FOCUSED_FRAME) = this->baseLayoutFrameOffset_;
}


void UILayoutFrame::ClearFocus()
{
	*(unsigned int*)Offset(FOCUSED_FRAME) = 0;
}