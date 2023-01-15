#include "stdafx.h"
#include "SimpleTexture.h"

#include "FunctionTemplate.h"
#include "Offsets.h"
#include "Tools.h"
#include "Storm.h"

namespace SimpleTexture {
	war3::CSimpleTexture* create(void* parent, unsigned int arg, unsigned int flag) {
		war3::CSimpleTexture* t = DreamStorm::MemAllocStruct<war3::CSimpleTexture>();
		dreamaero::generic_this_call<void>(
			Offset(SIMPLETEXTURE_CONSTRUCT),
			t,
			parent,
			arg,
			flag
			);
		return t;
	}

	void destroy(war3::CSimpleTexture* t) {
		if (GameUIObjectGet()) {
			dreamaero::generic_this_call<void>(
				VTBL(t)[2],
				t,
				1
				);
		}
	}

	unsigned int setTexturePath(war3::CSimpleTexture* t, const char* path, unsigned int flag) {
		return dreamaero::generic_this_call<unsigned int>(
			Offset(SIMPLETEXTURE_FILLBITMAP),
			t,
			path,
			flag
			);
	}
}