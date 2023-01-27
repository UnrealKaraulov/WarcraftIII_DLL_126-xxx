#include "stdafx.h"
#include "UISimpleStatusBar.h"
#include "Offsets.h"
#include "Tools.h"
#include "Storm.h"
#include "ObjectPool.h"
#include "UISimpleTexture.h"

ObjectPool<UISimpleStatusBar> SimpleStatusBarPool(10);

UISimpleStatusBar* UISimpleStatusBar::Create(UISimpleFrame* parent) {
	UISimpleStatusBar* rv = SimpleStatusBarPool.get();
	war3::CSimpleButton* baseObj = DreamStorm::MemAllocStruct<war3::CSimpleButton>();
	dreamaero::generic_this_call<void>(
		Offset(SIMPLESTATUSBAR_CONSTRUCT),
		baseObj,
		parent ? parent->base() : NULL
		);
	rv->setBase(baseObj);
	return rv;
}

void UISimpleStatusBar::Destroy(UISimpleStatusBar* obj) {
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
	SimpleStatusBarPool.ret(obj);
}

void UISimpleStatusBar::setValueRange(float min, float max) {
	war3::CSimpleStatusBar* baseObj = this->base<war3::CSimpleStatusBar*>();
	if (baseObj) {
		//6F60E3B0
		float v3, v4, v5, v6;
		v3 = min;
		baseObj->valueMin = min;
		baseObj->valueMax = max;
		if (baseObj->value >= min) {
			min = baseObj->value;
			v4 = v3;
			v5 = max;
		}
		else {
			v4 = min;
			v5 = max;
		}
		if (min <= v5) {
			v5 = v4;
			if (baseObj->value >= v4)
				v5 = baseObj->value;
		}
		v6 = v5;
		baseObj->valueChanged = 1;
		baseObj->value = v6;
	}
}

void UISimpleStatusBar::setValue(float value) {
	war3::CSimpleStatusBar* baseObj = this->base<war3::CSimpleStatusBar*>();
	if (baseObj) {
		dreamaero::generic_this_call<void>(
			dreamaero::offset_element_get<void*>(VTBL(baseObj), 0x6C),
			baseObj,
			value
			);
	}
}

void UISimpleStatusBar::setTexture(const char* path) {
	//6F60E610
	unsigned int rv;
	war3::CSimpleStatusBar* baseObj = this->base<war3::CSimpleStatusBar*>();
	if (baseObj) {
		war3::CSimpleTexture* texture;
		if (NULL != (texture = baseObj->texture)) {
			UISimpleTexture(texture).fillBitmap(path);
			rv = 1;
		}
		else {
			texture = baseObj->texture = DreamStorm::MemAllocStruct<war3::CSimpleTexture>();
			if (texture) {
				dreamaero::generic_this_call<war3::CSimpleTexture*>(
					Offset(SIMPLETEXTURE_CONSTRUCT),
					texture,
					0,
					2,
					1
					);
				UISimpleTexture t(texture);
				UILayoutFrame tthis(baseObj);
				t.fillBitmap(path);
				t.setRelativeObject(&tthis);
				t.setParent(this);
				rv = 1;
			}
			else
				rv = 0;
		}
	}
}

void UISimpleStatusBar::setColor(float r, float g, float b, float a)
{
	war3::CSimpleStatusBar* baseObj = this->base<war3::CSimpleStatusBar*>();
	if (baseObj)
	{
		war3::CSimpleTexture* texture;
		if (NULL != (texture = baseObj->texture)) {
			UISimpleTexture t(texture);
			t.setColorFloat(r, g, b, a);
		}
	}
}

void UISimpleStatusBar::setValueUpdated() {
	war3::CSimpleStatusBar* baseObj = this->base<war3::CSimpleStatusBar*>();
	if (baseObj) {
		baseObj->valueChanged = 1;
	}
}