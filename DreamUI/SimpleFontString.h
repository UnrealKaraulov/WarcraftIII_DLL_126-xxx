#include "stdafx.h"
#ifndef SIMPLE_FONT_STRING_H_INCLUDED_
#define SIMPLE_FONT_STRING_H_INCLUDED_

#include "UIStructs.h"

namespace SimpleFontString {
	war3::CSimpleFontString* create(void* parent = NULL, unsigned int arg = 2, unsigned int flag = 1);

	template <typename FrameType>
	war3::CSimpleFontString* create(FrameType* parent = NULL, unsigned int arg = 2, unsigned int flag = 1) {
		return create(reinterpret_cast<void*>(parent), arg, flag);
	}

	void destroy(war3::CSimpleFontString* t);
	unsigned int initFont(war3::CSimpleFontString* t, const char* fontPath, float fontSize, unsigned int useShadow);
	unsigned int initText(war3::CSimpleFontString* t, uint8_t flag = 1);
	unsigned int setText(war3::CSimpleFontString* t, const char* text);
	float getTextWidth(war3::CSimpleFontString* t);
}

#endif