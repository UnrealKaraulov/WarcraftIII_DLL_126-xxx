#include "stdafx.h"
#include "UISimpleFontString.h"
#include "UILayoutFrame.h"
#include "ObjectPool.h"
#include "Storm.h"
#include "Tools.h"

ObjectPool<UISimpleFontString> SimpleFontStringPool(150);

UISimpleFontString* UISimpleFontString::Create(UILayoutFrame* parent) {
	UISimpleFontString* rv = SimpleFontStringPool.get();
	war3::CSimpleFontString* baseObj = DreamStorm::MemAllocStruct<war3::CSimpleFontString>();
	dreamaero::generic_this_call<void>(
		Offset(SIMPLEFONTSTRING_CONSTRUCT),
		baseObj,
		parent ? parent->base() : NULL,
		2,				//TODO 这是什么
		1				//TODO 这是什么
		);
	rv->setBase(baseObj);
	return rv;
}

UISimpleFontString* UISimpleFontString::Create(
	UILayoutFrame* parent,
	const char* text,
	float size
) {
	UISimpleFontString* sfs = UISimpleFontString::Create(parent);

	sfs->fontsize = size;
	sfs->initFont(Skin::getPathByName("MasterFont"), size, 0);
	sfs->setText(text);
	sfs->setColorFloat(1, 0.8f, 0, 1);
	return sfs;
}

void UISimpleFontString::Destroy(UISimpleFontString* obj) {
	war3::CSimpleFontString* baseObj = obj->base<war3::CSimpleFontString*>();
	if (baseObj) {
		if (GameUIObjectGet()) {
			dreamaero::generic_this_call<void>(
				VTBL(baseObj)[2],
				baseObj,
				1
				);
		}
	}
	SimpleFontStringPool.ret(obj);
}

void UISimpleFontString::setText(const char* text) {
	war3::CSimpleFontString* baseObj = this->base<war3::CSimpleFontString*>();

	if (text != NULL)
		_org_text = text;

	if (baseObj) {
		dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLEFONTSTRING_SETTEXT),
			baseObj,
			text
			);
	}
}

void UISimpleFontString::initFont(const char* path, float size, unsigned int useShadow) {
	war3::CSimpleFontString* baseObj = this->base<war3::CSimpleFontString*>();
	fontsize = size;

	if (baseObj) {
		dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLEFONTSTRING_INITFONT),
			baseObj,
			path,
			size,
			useShadow
			);

		dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLEFONTSTRING_INITTEXT),
			baseObj,
			1		//TODO 这是什么
			);
	}
}

float UISimpleFontString::getTextWidth() {
	float rv = -1.0f;
	war3::CSimpleFontString* baseObj = this->base<war3::CSimpleFontString*>();
	if (baseObj) {
		rv = dreamaero::generic_this_call<float>(
			Offset(SIMPLEFONTSTRING_GETTEXTWIDTH),
			baseObj
			);
	}
	return rv;
}

float UISimpleFontString::getTextHeight()
{
	int count = 2;
	size_t nPos = _org_text.find("|n", 0); // first occurrence
	while (nPos != std::string::npos && nPos < _org_text.length())
	{
		count++;
		nPos = _org_text.find("|n", nPos + 1);
	}

	return count * fontsize;
}