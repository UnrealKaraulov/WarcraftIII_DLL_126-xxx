#include "stdafx.h"
#include "UISimpleButton.h"
#include "Offsets.h"
#include "Tools.h"
#include "Storm.h"
#include "ObjectPool.h"
#include "Observer.h"

//ObjectPool<UISimpleButton> SimpleButtonPool(10);

OBSERVER_EVENT_HANDLER(Handler) {

	return 1;
}

UISimpleButton* UISimpleButton::Create(UISimpleFrame* parent) {
	UISimpleButton* rv = new UISimpleButton;//SimpleButtonPool.get();
	war3::CSimpleButton* baseObj = DreamStorm::MemAllocStruct<war3::CSimpleButton>();
	dreamaero::generic_this_call<void>(
		Offset(SIMPLEBUTTON_CONSTRUCT), 
		baseObj,
		parent ? parent->base() : NULL
	);

	rv->setBase(baseObj);
	return rv;
}

void UISimpleButton::Destroy(UISimpleButton* obj) {
	war3::CSimpleButton* baseObj = obj->base<war3::CSimpleButton*>();
	if (baseObj) {
		if (GameUIObjectGet()) {
			dreamaero::generic_this_call<void>(
				VTBL(baseObj)[2],
				baseObj,
				1
			);
		}
	}
	//SimpleButtonPool.ret(obj);
	delete obj;
}

void UISimpleButton::setStateTexture(uint32_t state, const char* path) {
	war3::CSimpleButton* baseObj = this->base<war3::CSimpleButton*>();
	if (baseObj) {
		dreamaero::generic_this_call<void>(
			Offset(SIMPLEBUTTON_SETSTATETEXTURE), 
			baseObj,
			state,
			path
		);
	}
}

void UISimpleButton::setState(uint32_t state) {
	war3::CSimpleButton* baseObj = this->base<war3::CSimpleButton*>();
	if (baseObj) {
		dreamaero::generic_this_call<void>(
			Offset(SIMPLEBUTTON_SETSTATE), 
			baseObj,
			state
		);
	}
}

uint32_t UISimpleButton::getMouseButton() {
	war3::CSimpleButton* baseObj = this->base<war3::CSimpleButton*>();
	if (baseObj) {
		return baseObj->mouseButtonFlags;
	}
	return 0;
}

void UISimpleButton::setMouseButton(uint32_t flags) { 
	war3::CSimpleButton* baseObj = this->base<war3::CSimpleButton*>();
	if (baseObj) {
		baseObj->mouseButtonFlags = flags;
	}	
}

void UISimpleButton::setClickEventObserver(uint32_t eventId, Observer* ob) {
	war3::CSimpleButton* baseObj = this->base<war3::CSimpleButton*>();
	if (baseObj) {
		baseObj->clickEventId = eventId;
		baseObj->clickEventObserver = ob->base<war3::CObserver*>();
	}
}

void UISimpleButton::setMouseEventObserver(uint32_t mouseOverEventId, uint32_t mouseOutEventId, Observer* ob) {
	war3::CSimpleButton* baseObj = this->base<war3::CSimpleButton*>();
	if (baseObj) {
		baseObj->mouseOverEventId = mouseOverEventId;
		baseObj->mouseOutEventId = mouseOutEventId;
		baseObj->mouseEventObserver = ob->base<war3::CObserver*>();
	}
}

UISimpleButton* UISimpleButton::Create(
	UISimpleFrame	*parent,
	float			width,
	float			height,
	const char		*pathTextureDisabled,
	const char		*pathTextureEnabled,
	const char		*pathTexturePushed,
	uint32_t		mouseClickFlag,
	uint32_t		initState
){
	UISimpleButton* button = Create(parent);
	if (button) {
		button->setWidth(width);
		button->setHeight(height);
		button->setStateTexture(UISimpleButton::STATE_DISABLED, pathTextureDisabled);
		button->setStateTexture(UISimpleButton::STATE_ENABLED, pathTextureEnabled);
		button->setStateTexture(UISimpleButton::STATE_PUSHED, pathTexturePushed);
		button->setMouseButton(mouseClickFlag);
		button->setState(initState);
	}
	return button;
}