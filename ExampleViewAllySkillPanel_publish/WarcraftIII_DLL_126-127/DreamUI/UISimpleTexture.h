#include "stdafx.h"
#ifndef UI_SIMPLETEXTURE_H
#define UI_SIMPLETEXTURE_H

#include "UISimpleRegion.h"

class UISimpleTexture : public UISimpleRegion {
public:
	static UISimpleTexture* Create(UISimpleFrame* parent = NULL);
	static void Destroy(UISimpleTexture* t);

	UISimpleTexture(war3::CSimpleTexture* base = NULL) : UISimpleRegion(base) {}

	uint32_t fillBitmap(const char* path);
	uint32_t fillColor(uint32_t color);
};

#endif