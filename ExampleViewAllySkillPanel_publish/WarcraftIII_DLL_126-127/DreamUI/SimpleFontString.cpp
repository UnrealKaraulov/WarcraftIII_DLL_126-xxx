#include "stdafx.h"
#include "SimpleFontString.h"

#include "FunctionTemplate.h"
#include "Offsets.h"
#include "Tools.h"
#include "Storm.h"

namespace SimpleFontString {
	war3::CSimpleFontString* create(void* parent, uint32_t arg, uint32_t flag) {
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

	uint32_t initFont(war3::CSimpleFontString* t, const char* fontPath, float fontSize, uint32_t useShadow) {
		return dreamaero::generic_this_call<uint32_t>(
			Offset(SIMPLEFONTSTRING_INITFONT), 
			t,
			fontPath,
			fontSize,
			useShadow
		);
	}
	
	uint32_t initText(war3::CSimpleFontString* t, uint8_t flag) {
		return dreamaero::generic_this_call<uint32_t>(
			Offset(SIMPLEFONTSTRING_INITTEXT), 
			t,
			flag
		);
	}

	uint32_t setText(war3::CSimpleFontString* t, const char* text) {
		return dreamaero::generic_this_call<uint32_t>(
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