#include "stdafx.h"
#include "UICommandButton.h"
#include "UISimpleFrame.h"
#include "Offsets.h"
#include "Tools.h"
#include "Storm.h"
#include "ObjectPool.h"

ObjectPool<UICommandButton> CommandButtonPool(150);

UICommandButton* UICommandButton::Create(UISimpleFrame* parent) {
	UICommandButton* rv = CommandButtonPool.get();

	war3::CCommandButton* baseObj = DreamStorm::MemAllocStruct<war3::CCommandButton>();
	dreamaero::generic_this_call<void>(
		Offset(COMMANDBUTTON_CONSTRUCT),
		baseObj,
		parent ? parent->base() : NULL,
		1				//TODO 
		);
	rv->setBase(baseObj);

	dreamaero::generic_this_call<void>(
		Offset(COMMANDBUTTON_SETSOMEFLOAT),
		baseObj,
		0.81999999f
		);


	rv->setWidth(0.03f);
	rv->setHeight(0.03f);
	rv->setMouseButton(16);


	return rv;
}

void UICommandButton::Destroy(UICommandButton* obj) {
	if (!obj) return;
	war3::CCommandButton* baseObj = obj->base<war3::CCommandButton*>();
	if (baseObj) {
		if (GameUIObjectGet()) {
			dreamaero::generic_this_call<void>(
				VTBL(baseObj)[2],
				baseObj,
				1
				);
		}
	}
	CommandButtonPool.ret(obj);
}