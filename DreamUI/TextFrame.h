#include "stdafx.h"
#ifndef TEXTFRAME_H_INCLUDED
#define TEXTFRAME_H_INCLUDED

#include "UIStructs.h"

namespace TextFrame {
	war3::CTextFrame* create(void* parent = NULL, unsigned int a2 = 0, unsigned int a3 = 0);
	template <typename FrameType>
	war3::CTextFrame* create(FrameType* parent = NULL, unsigned int a2 = 0, unsigned int a3 = 0) {
		return create(reinterpret_cast<void*>(parent, a2, a3));
	}

	void destroy(war3::CTextFrame* t);

	void initFont(war3::CTextFrame* t, const char* fontPath, float fontHeight, unsigned int arg3 = 0);
	void setFont(war3::CTextFrame* t, unsigned int arg1, float a2, float a3);
	void setShadow(war3::CTextFrame* t, unsigned int arg1 = 8);
	void setColor(war3::CTextFrame* t, unsigned int color);
	void setText(war3::CTextFrame* t, const char* text);
}

#endif