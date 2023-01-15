#include "stdafx.h"
#ifndef SIMPLEBUTTON_H_INCLUDED
#define SIMPLEBUTTON_H_INCLUDED

#include "UIStructs.h"
#include "FunctionTemplate.h"
#include "Offsets.h"

namespace SimpleButton {
	namespace MouseButtonFlag {
		const unsigned int LEFT = 1 << 4;
		const unsigned int CENTER = 1 << 5;
		const unsigned int RIGHT = 1 << 6;
	}

	namespace State {
		const unsigned int DISABLED = 0;
		const unsigned int ENABLED = 1;
		const unsigned int PUSHED = 2;
	}

	war3::CSimpleButton* create(void* parent = NULL);
	template <typename FrameType>
	war3::CSimpleButton* create(FrameType* parent = NULL) {
		return create(reinterpret_cast<void*>(parent));
	}

	void destroy(war3::CSimpleButton* t);

	void setClickEventObserverData(war3::CSimpleButton* t, unsigned int eventId, war3::CObserver* ob);
	void setMouseEventObserverData(war3::CSimpleButton* t, unsigned int mouseOverEventId, unsigned int mouseOutEventId, war3::CObserver* ob);
	void setMouseButtonFlags(war3::CSimpleButton* t, unsigned int flags);
	void setStateTexture(war3::CSimpleButton* t, unsigned int state, const char* path);
	void setState(war3::CSimpleButton* t, unsigned int state);
}

#endif