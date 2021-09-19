#include "stdafx.h"
#include "UISimpleFrame.h"
#include "Storm.h"
#include "ObjectPool.h"
#include "Tools.h"
#include "RCString.h"

ObjectPool<UISimpleFrame> SimpleFramePool(150);

UISimpleFrame* UISimpleFrame::Create(UISimpleFrame* UIParent) {
	UISimpleFrame* rv = SimpleFramePool.get();
	war3::CSimpleFrame* t = DreamStorm::MemAllocStruct<war3::CSimpleFrame>();
	dreamaero::generic_this_call<war3::CSimpleFrame*>(
		Offset(SIMPLEFRAME_CONSTRUCT),
		t,
		UIParent ? UIParent->base() : NULL
		);
	rv->setBase(t);
	return rv;
}

void UISimpleFrame::Destroy(UISimpleFrame* obj) {
	if (obj) {
		if (GameUIObjectGet()) {
			war3::CSimpleFrame* t = obj->base<war3::CSimpleFrame*>();
			dreamaero::generic_this_call<void>(
				VTBL(t)[2],
				t,
				1
				);
		}
		SimpleFramePool.ret(obj);
		obj->setBase(NULL);
	}
}

UISimpleFrame::UISimpleFrame(void* base) : UILayoutFrame(base) {

}

void UISimpleFrame::applyTextureSettings() {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj->textureSettings) {
		dreamaero::generic_this_call<uint32_t>(
			Offset(SIMPLEFRAME_APPLYTEXTURESETTINGS),
			baseObj,
			baseObj->textureSettings
			);
	}
}

void UISimpleFrame::setBackground(const char* path) {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj && path ) {
		this->initTextureSettings();
		RCString::setString(&(baseObj->textureSettings->RCS_bgTexturePath), path);
	}
}

void UISimpleFrame::setBorder(const char* path) {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj && path ) {
		this->initTextureSettings();
		RCString::setString(&(baseObj->textureSettings->RCS_borderTexturePath), path);
	}
}

void UISimpleFrame::setPadding(float top, float bottom, float left, float right) {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj) {
		this->initTextureSettings();
		baseObj->textureSettings->paddingTop = top;
		baseObj->textureSettings->paddingBottom = bottom;
		baseObj->textureSettings->paddingLeft = left;
		baseObj->textureSettings->paddingRight = right;
	}
}

void UISimpleFrame::setPadding(float padding) {
	setPadding(padding, padding, padding, padding);
}

void UISimpleFrame::setBorderWidth(float width) {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj) {
		this->initTextureSettings();
		baseObj->textureSettings->borderWidth = width;
	}
}

void UISimpleFrame::show() {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj) {
		baseObj->visible = 1;
		void *func = vtableFpAtOffset(VTBL(baseObj), 0x68);
		if (func)
		{
			dreamaero::generic_this_call<void>(func, baseObj);
		}
	}
}

void UISimpleFrame::hide() {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj) {
		baseObj->visible = 0;
		void *func = vtableFpAtOffset(VTBL(baseObj), 0x64);
		if (func)
		{
			dreamaero::generic_this_call<void>(func, baseObj);
		}
	}
}

bool UISimpleFrame::visible() {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj) {
		return (baseObj->visible != 0);
	}
	return false;
}

void UISimpleFrame::setParent(UISimpleFrame* parent) {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	war3::CSimpleFrame* parentBaseObj;
	if ((parentBaseObj = parent->base<war3::CSimpleFrame*>()) && parent != this) {
		dreamaero::generic_this_call<uint32_t>(Offset(SIMPLEFRAME_SETPARENT), baseObj, parentBaseObj);
	}
}


void UISimpleFrame::setLevel(int level) {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	dreamaero::generic_this_call<uint32_t>(Offset(SIMPLEFRAME_SETLEVEL), baseObj, level);

	this->applyPosition( );
}

UILayoutFrame *UISimpleFrame::parent()
{
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj)
	{
		return &UILayoutFrame(baseObj->parent);
	}
	return NULL;
}

void UISimpleFrame::initTextureSettings() {
	war3::CSimpleFrame* baseObj = this->base<war3::CSimpleFrame*>();
	if (baseObj) {
		if (!baseObj->textureSettings) {
			baseObj->textureSettings = DreamStorm::MemAllocStruct<war3::SimpleFrameTextureSettings>();
			baseObj->textureSettings = dreamaero::generic_fast_call<war3::SimpleFrameTextureSettings*>(
				Offset(SIMPLEFRAME_INITTEXTURESETTINGS),
				baseObj->textureSettings
				);
			baseObj->textureSettings->borderFlags = 0xFF;
			baseObj->textureSettings->unk_30 = 1;
		}
	}
}