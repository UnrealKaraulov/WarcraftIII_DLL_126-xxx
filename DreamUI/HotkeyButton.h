//�������Ͱ�ť����������һ���ȼ�����ť��ʾ����Ϊ�ȼ����ı��������ť�󰴼���������

#ifndef HOTKEY_BUTTON_H_
#define HOTKEY_BUTTON_H_

#include "Button.h"

class HotkeyButton : public Button {
	int		_hotkey;
	int* _pVarHotkey; //�󶨵��ȼ�����(������ֵ)
	char* _profileApp; //�󶨵�profile������
	char* _profileKey; //�󶨵�profileС����
	bool	_enteringHotkey;	//��ʾ�Ƿ���������ȼ�״̬
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