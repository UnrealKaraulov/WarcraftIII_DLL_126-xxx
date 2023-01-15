#include "stdafx.h"
#include "SimpleFontString.h"

#include "FunctionTemplate.h"
#include "Offsets.h"
#include "Tools.h"
#include "Storm.h"

namespace SimpleFontString {
	war3::CSimpleFontString* create(void* parent, unsigned int arg, unsigned int flag) {
		war3::CSimpleFontString* t = DreamStorm::MemAllocStruct<war3::CSimpleFontString>();
		dreamaero::generic_this_call<void>(
			Offset(SIMPLEFONTSTRING_CONSTRUCT),
			t,
			parent,
			arg,
			flag
			);
		return t;
	}

	void destroy(war3::CSimpleFontString* t) {
		if (GameUIObjectGet()) {
			dreamaero::generic_this_call<void>(
				VTBL(t)[2],
				t,
				1
				);
		}
	}

	unsigned int initFont(war3::CSimpleFontString* t, const char* fontPath, float fontSize, unsigned int useShadow) {
		return dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLEFONTSTRING_INITFONT),
			t,
			fontPath,
			fontSize,
			useShadow
			);
	}

	unsigned int initText(war3::CSimpleFontString* t, uint8_t flag) {
		return dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLEFONTSTRING_INITTEXT),
			t,
			flag
			);
	}

	unsigned int setText(war3::CSimpleFontString* t, const char* text) {
		return dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLEFONTSTRING_SETTEXT),
			t,
			text
			);
	}

	float getTextWidth(war3::CSimpleFontString* t) {
		return dreamaero::generic_this_call<float>(
			Offset(SIMPLEFONTSTRING_GETTEXTWIDTH),
			t
			);
	}
}