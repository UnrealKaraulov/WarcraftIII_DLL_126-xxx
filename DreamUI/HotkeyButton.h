//特殊类型按钮，用于设置一个热键，按钮显示文字为热键的文本，点击按钮后按键可以设置

#ifndef HOTKEY_BUTTON_H_
#define HOTKEY_BUTTON_H_

#include "Button.h"

class HotkeyButton : public Button {
	int		_hotkey;
	int* _pVarHotkey; //绑定的热键变量(整数键值)
	char* _profileApp; //绑定的profile大项名
	char* _profileKey; //绑定的profile小项名
	bool	_enteringHotkey;	//表示是否进入设置热键状态
	int		_timer = 0;
public:
	ButtonCallback	hotkeyButtonCallback = NULL;

	HotkeyButton() : _hotkey(NULL), _pVarHotkey(NULL), _profileApp(NULL), _profileKey(NULL), _enteringHotkey(false) {}

	HotkeyButton(
		UISimpleFrame* parent,
		float					width,
		float					height,
		int* hotkeyVar,
		int						defaultHotkey,
		char* profileApp = NULL,
		char* profileKey = NULL,
		ButtonCallback			callback = NULL,
		bool					noSurface = false
	);

	~HotkeyButton();

	void setKey(int keyCode);
	void setKeyEx(unsigned int keyCode);
	int getKey() { return _hotkey; }
	void toggleEnteringHotkey() { _enteringHotkey = !_enteringHotkey; }
	bool isEnteringHotkey() { return _enteringHotkey; }
	int getTimer() { return _timer; }
	void setTimer(int times) { _timer = times; }

	static void Init();
	static void Cleanup();
	static void StopInput();
	static bool IsAnyHotkeyActive();
};

#endif