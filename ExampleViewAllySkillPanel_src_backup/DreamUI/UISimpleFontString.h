#include "stdafx.h"
#ifndef UI_SIMPLEFONTSTRING_H_INCLUDED
#define UI_SIMPLEFONTSTRING_H_INCLUDED

#include "UISimpleRegion.h"
#include "UISimpleFrame.h"
#include <string>

class UISimpleFontString : public UISimpleRegion {
public:
	static UISimpleFontString* Create(UILayoutFrame* parent = 0);
	static UISimpleFontString* Create(
		UILayoutFrame* parent,
		const char* text,
		float size
	);
	static void Destroy(UISimpleFontString* t);

	UISimpleFontString(void* base = 0) : UISimpleRegion(base) {}

	void initFont(const char* path, float size, uint32_t useShadow);
	void setText(const char* text);
	float getTextWidth();
	float getTextHeight( );
private:
	std::string _org_text;
	float fontsize;
};

#endif