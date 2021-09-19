#include "stdafx.h"
#include "UISimpleTexture.h"
#include "UISimpleFrame.h"
#include "Offsets.h"
#include "Tools.h"
#include "Storm.h"
#include "ObjectPool.h"

ObjectPool<UISimpleTexture> SimpleTexturePool(150);

UISimpleTexture* UISimpleTexture::Create(UISimpleFrame* parent) {
	UISimpleTexture* rv = SimpleTexturePool.get();
	war3::CSimpleTexture* baseObj = DreamStorm::MemAllocStruct<war3::CSimpleTexture>();
	dreamaero::generic_this_call<void>(
		Offset(SIMPLETEXTURE_CONSTRUCT), 
		baseObj,
		parent ? parent->base(): NULL,
		2,				//TODO 这是什么
		1				//TODO 这是什么
	);
	rv->setBase(baseObj);
	return rv;
}

void UISimpleTexture::Destroy(UISimpleTexture* obj) {
	if (!obj) return;
	war3::CSimpleTexture* baseObj = obj->base<war3::CSimpleTexture*>();
	if (baseObj) {
		if (GameUIObjectGet()) {
			dreamaero::generic_this_call<void>(
				VTBL(baseObj)[2],
				baseObj,
				1
			);
		}
	}
	SimpleTexturePool.ret(obj);
}

uint32_t UISimpleTexture::fillBitmap(const char* path) {
	uint32_t rv = 0;
	war3::CSimpleTexture* baseObj = this->base<war3::CSimpleTexture*>();
	if (baseObj) {
		rv = dreamaero::generic_this_call<uint32_t>(
			Offset(SIMPLETEXTURE_FILLBITMAP),
			baseObj,
			path,
			0				//这是什么？
		);
	}
	return rv;
}

uint32_t UISimpleTexture::fillColor(uint32_t color) {
	uint32_t rv = 0;
	war3::CSimpleTexture* baseObj = this->base<war3::CSimpleTexture*>();
	if (baseObj) {
		rv = dreamaero::generic_this_call<uint32_t>(
			Offset(SIMPLETEXTURE_FILLCOLOR),
			baseObj,
			&color
		);
	}
	return rv;
}