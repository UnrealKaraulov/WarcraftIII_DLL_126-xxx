#ifndef EDITBOX_BUTTON_H_
#define EDITBOX_BUTTON_H_

#include "Button.h"

#include "Timer.h"


class EditBoxButton : public Button {

public:
	std::string* _pVarText; //绑定的热键变量(整数键值)
	bool	_enteringText = false;	//表示是否进入设置热键状态
	int		_timer = 0;
	std::wstring _org_text;
	int _offset;

	ButtonCallback	editBoxButtonCallback = NULL;

	EditBoxButton() : _pVarText(NULL), _enteringText(false) , _offset(0){}

	EditBoxButton(UISimpleFrame* parent, float width, float height, std::string* text, std::string defaulttext,
		ButtonCallback			callback = NULL,
		bool					noSurface = false);

	~EditBoxButton();

	void UpdateTextAndCallback();

	void DisplayCurrentText(bool withcursor = false);

	void toggleEnteringText();

	bool isEnteringText();

	int getTimer();

	void setTimer(int times);

	static void Init();

	static void Cleanup();

	static bool IsAnyEditBoxActive();

	void EnterText(std::wstring text);

	static void EnterTextToCurrentEditBox(std::wstring text);

	void RemoveCharacter();

	void RemoveCharacterRight();

	static void RemoveCharacterFromCurrentEditBox(bool right = false);

	void MoveCursorLeft();

	static void CurrentEditBoxMoveCursorLeft();

	void MoveCursorRight();

	static void CurrentEditBoxMoveCursorRight();

	static void StopInput();

};

#endif