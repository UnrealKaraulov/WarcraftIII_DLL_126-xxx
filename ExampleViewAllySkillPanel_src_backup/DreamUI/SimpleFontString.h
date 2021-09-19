#include "stdafx.h"
#ifndef SIMPLE_FONT_STRING_H_INCLUDED_
#define SIMPLE_FONT_STRING_H_INCLUDED_

#include "UIStructs.h"

namespace SimpleFontString {
	war3::CSimpleFontString* create(void* parent = NULL, uint32_t arg = 2, uint32_t flag = 1);

	template <typename FrameType>
	war3::CSimpleFontString* create(FrameType* parent = NULL, uint32_t arg = 2, uint32_t flag = 1) {
		return create(reinterpret_cast<void*>(parent), arg, flag);
	}
	
	void destroy(war3::CSimpleFontString* t);
	uint32_t initFont(war3::CSimpleFontString* t, const char* fontPath, float fontSize, uint32_t useShadow);
	uint32_t initText(war3::CSimpleFontString* t, uint8_t flag = 1);
	uint32_t setText(war3::CSimpleFontString* t, const char* text);
	float getTextWidth(war3::CSimpleFontString* t);
}

#endif