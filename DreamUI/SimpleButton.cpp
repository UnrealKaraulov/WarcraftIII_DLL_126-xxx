#include "stdafx.h"
#include "SimpleButton.h"
#include "Offsets.h"
#include "FunctionTemplate.h"
#include "Storm.h"
#include "Tools.h"

namespace SimpleButton {
	war3::CSimpleButton* create(void* parent) {
		war3::CSimpleButton* t = DreamStorm::MemAllocStruct<war3::CSimpleButton>();
		dreamaero::generic_this_call<void>(
			Offset(SIMPLEBUTTON_CONSTRUCT),
			t,
			parent ? parent : GameUIObjectGet()->simpleConsole
			);
		return t;
	}

	war3::CSimpleButton* createeditbox(void* parent) {
		war3::CSimpleButton* t = DreamStorm::MemAllocStruct<war3::CSimpleButton>();
		dreamaero::generic_this_call<void>(
			Offset(EDITBOX_CONSTRUCT),
			t,
			parent ? parent : GameUIObjectGet()->simpleConsole, 0, 0, 0
			);
		return t;
	}

	void destroy(war3::CSimpleButton* t) {
		if (GameUIObjectGet()) {
			dreamaero::generic_this_call<void>(
				VTBL(t)[2],
				t,
				1
				);
		}
	}

	void setClickEventObserverData(war3::CSimpleButton* t, unsigned int eventId, war3::CObserver* ob) {
		t->clickEventId = eventId;
		t->clickEventObserver = ob;
	}

	void setMouseEventObserverData(war3::CSimpleButton* t, unsigned int mouseOverEventId, unsigned int mouseOutEventId, war3::CObserver* ob) {
		t->mouseOverEventId = mouseOverEventId;
		t->mouseOutEventId = mouseOutEventId;
		t->mouseEventObserver = ob;
	}

	void setMouseButtonFlags(war3::CSimpleButton* t, unsigned int flags) {
		t->mouseButtonFlags = flags;
	}

	void setStateTexture(war3::CSimpleButton* t, unsigned int state, const char* path) {
		dreamaero::generic_this_call<void>(
			Offset(SIMPLEBUTTON_SETSTATETEXTURE),
			t,
			state,
			path
			);
	}

	void setState(war3::CSimpleButton* t, unsigned int state) {
		dreamaero::generic_this_call<void>(
			Offset(SIMPLEBUTTON_SETSTATE),
			t,
			state
			);
	}
}