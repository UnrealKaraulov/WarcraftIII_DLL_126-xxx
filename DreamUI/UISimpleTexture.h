#include "stdafx.h"
#ifndef UI_SIMPLETEXTURE_H
#define UI_SIMPLETEXTURE_H

#include "UISimpleRegion.h"

class UISimpleTexture : public UISimpleRegion {
public:
	static UISimpleTexture* Create(UISimpleFrame* parent = NULL);
	static void Destroy(UISimpleTexture* t);

	UISimpleTexture(war3::CSimpleTexture* base = NULL) : UISimpleRegion(base) {}

	unsigned int fillBitmap(const char* path);
	unsigned int fillColor(unsigned int color);
};

#endif