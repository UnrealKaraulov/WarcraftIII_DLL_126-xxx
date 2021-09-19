#include "stdafx.h"
#ifndef SIMPLEBUTTON_H_INCLUDED
#define SIMPLEBUTTON_H_INCLUDED

#include "UIStructs.h"
#include "FunctionTemplate.h"
#include "Offsets.h"

namespace SimpleButton {
	namespace MouseButtonFlag {
		const uint32_t LEFT = 1 << 4;
		const uint32_t CENTER = 1 << 5;
		const uint32_t RIGHT = 1 << 6;
	}

	namespace State {
		const uint32_t DISABLED = 0;
		const uint32_t ENABLED	= 1;
		const uint32_t PUSHED	= 2;
	}

	war3::CSimpleButton* create(void* parent = NULL);
	template <typename FrameType>
	war3::CSimpleButton* create(FrameType* parent = NULL) {
		return create(reinterpret_cast<void*>(parent));
	}

	void destroy(war3::CSimpleButton* t);
	
	void setClickEventObserverData(war3::CSimpleButton* t, uint32_t eventId, war3::CObserver* ob);
	void setMouseEventObserverData(war3::CSimpleButton* t, uint32_t mouseOverEventId, uint32_t mouseOutEventId, war3::CObserver* ob);
	void setMouseButtonFlags(war3::CSimpleButton* t, uint32_t flags);
	void setStateTexture(war3::CSimpleButton* t, uint32_t state, const char* path);
	void setState(war3::CSimpleButton* t, uint32_t state);
}

#endif