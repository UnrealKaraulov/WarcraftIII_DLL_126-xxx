#include "stdafx.h"
#include "TextFrame.h"
#include "FunctionTemplate.h"
#include "Offsets.h"
#include "Storm.h"
#include "Tools.h"

namespace TextFrame {
	war3::CTextFrame* create(void* parent, unsigned int a2, unsigned int a3) {
		war3::CTextFrame* rv = DreamStorm::MemAllocStruct<war3::CTextFrame>();
		return dreamaero::generic_this_call<war3::CTextFrame*>(
			Offset(TEXTFRAME_CONSTRUCT),
			rv,
			parent,
			a2,
			a3
			);
	}

	void initFont(war3::CTextFrame* t, const char* fontPath, float fontHeight, unsigned int arg3) {
		dreamaero::generic_this_call<void>(
			Offset(TEXTFRAME_INITFONT),
			t,
			fontPath,
			fontHeight,
			arg3
			);
	}

	void setFont(war3::CTextFrame* t, unsigned int arg1, float a2, float a3) {
		war3::TextFrameFontSettings settings;
		settings.unk_0 = a2;
		settings.unk_4 = a3;
		dreamaero::generic_this_call<void>(
			Offset(TEXTFRAME_SETFONT),
			t,
			arg1,
			&settings
			);
	}

	void setShadow(war3::CTextFrame* t, unsigned int arg1) {
		dreamaero::generic_this_call<void>(
			Offset(TEXTFRAME_SETSHADOW),
			t,
			arg1
			);
	}

	void setColor(war3::CTextFrame* t, unsigned int color) {
		dreamaero::generic_this_call<void>(
			Offset(TEXTFRAME_SETCOLOR),
			t,
			&color
			);
	}

	void setText(war3::CTextFrame* t, const char* text) {
		dreamaero::generic_this_call<void>(
			Offset(TEXTFRAME_SETTEXT),
			t,
			text
			);
	}

	void destroy(war3::CTextFrame* t) {
		if (GlueMgrObjectGet()) {
			dreamaero::generic_this_call<void>(
				VTBL(t)[1],
				t,
				1
				);
		}
	}
}