#include "stdafx.h"
#ifndef UI_SIMPLEBUTTON_H_INCLUDED
#define UI_SIMPLEBUTTON_H_INCLUDED
#include "UISimpleFrame.h"
class Observer;

class UISimpleButton : public UISimpleFrame {
public:
	enum MouseButton {
		MOUSEBUTTON_LEFT = 1 << 4,
		MOUSEBUTTON_MIDDLE = 1 << 5,
		MOUSEBUTTON_RIGHT = 1 << 6
	};

	enum State {
		STATE_DISABLED = 0,
		STATE_ENABLED = 1,
		STATE_PUSHED = 2
	};

	enum Event2 {
		EVENT_CLICK = 1u,
		EVENT_MOUSEOVER = 2u,
		EVENT_MOUSEOUT = 3u
	};

	static UISimpleButton* Create(UISimpleFrame* parent = NULL);
	static UISimpleButton* Create( //TODO
		UISimpleFrame* parent,
		float			width,
		float			height,
		const char* pathTextureDisabled,	//TODO 以后改为struct一次性获取一组，并支持例如getIcon('hfoo')来获得单位、技能、buff等各类型游戏内元素对应的纹理struct
		const char* pathTextureEnabled,
		const char* pathTexturePushed,
		unsigned int		mouseClickFlag,
		unsigned int		initState
		//TODO callback
	);
	static void Destroy(UISimpleButton* obj);

	UISimpleButton(void* base = NULL) : UISimpleFrame(base) {

	}
	void setStateTexture(unsigned int state, const char* path);
	void setState(unsigned int state);
	unsigned int getMouseButton();
	void setMouseButton(unsigned int flags);
	void setClickEventObserver(unsigned int eventId, Observer* ob);
	void setMouseEventObserver(unsigned int mouseOverEventId, unsigned int mouseOutEventId, Observer* ob);
private:
};

#endif