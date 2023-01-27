#include "stdafx.h"
#ifndef SIMPLEFRAME_H_INCLUDED
#define SIMPLEFRAME_H_INCLUDED

#include "UIStructs.h"
#include "FunctionTemplate.h"
#include "Offsets.h"

namespace SimpleFrame {
	war3::CSimpleFrame* create(void* parent = NULL);

	template <typename FrameType>
	war3::CSimpleFrame* create(FrameType* parent = NULL) {
		return create(reinterpret_cast<void*>(parent));
	}

	void destroy(war3::CSimpleFrame* t);

	war3::SimpleFrameTextureSettings* initTextureSettings(war3::CSimpleFrame* t);

	unsigned int applyTextureSetting(war3::CSimpleFrame* t);

	void show(war3::CSimpleFrame* t);

	void hide(war3::CSimpleFrame* t);
}

#endif