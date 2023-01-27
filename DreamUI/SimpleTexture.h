#include "stdafx.h"
#ifndef SIMPLETEXTURE_H_INCLUDED
#define SIMPLETEXTURE_H_INCLUDED

#include "UIStructs.h"

namespace SimpleTexture {
	war3::CSimpleTexture* create(void* parent = NULL, unsigned int arg = 2, unsigned int flag = 1);
	void destroy(war3::CSimpleTexture* t);

	unsigned int setTexturePath(war3::CSimpleTexture* t, const char* path, unsigned int flag = 0);
}

#endif