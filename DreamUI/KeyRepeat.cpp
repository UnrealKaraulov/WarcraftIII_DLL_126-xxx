#include "stdafx.h"
#include "Timer.h"
#include "KeyCode.h"
#include "Input.h"
#include "EventData.h"
#include "Event.h"
#include "Game.h"

namespace KeyRepeat {

	//static bool RepeatEnable;
	static float RepeatDelay;	//��ʼ�ӳ�ʱ��
	static float RepeatRate;	//��������ÿ��

	static Timer* RepeatTimer[0x30C];
	static Timer* RepeatDelayTimer[0x30C];

	bool KeyAllowedToRepeat(int keyCode) {
		return
			keyCode != KEYCODE::KEY_CONTROL
			&& keyCode != KEYCODE::KEY_ALT
			&& keyCode != KEYCODE::KEY_SHIFT
			&& keyCode != KEYCODE::KEY_ENTER
			&& keyCode >= 0
			&& keyCode < 0x30C;
		//���Ӹ�������
	}

	void onRepeatExpired(Timer* tm) {
		int* timerRepeatKeyCode = tm->data<int>();
		GameUIKeyPress(*timerRepeatKeyCode, true, false);
		//OutputScreen(10, "key %c repeat.", *timerRepeatKeyCode);
	}

	void onRepeatDelayExpired(Timer* tm) {
		int* timerKeyCode = tm->data<int>();
		if (RepeatDelayTimer[*timerKeyCode]) {
			RepeatDelayTimer[*timerKeyCode]->destroy();
			RepeatDelayTimer[*timerKeyCode] = NULL;
		}
		if (KeyIsDown(*timerKeyCode)) {//�����Ȼ�ڰ���״̬
			//��Ҫ�ж��Ƿ����timer!
			if (!RepeatTimer[*timerKeyCode]) {
				//ÿ�� 1/RepeatRate ʱ�䣬��һ�ΰ��²�ģ����Ҳ���
				RepeatTimer[*timerKeyCode]
					= GetTimer(1.f / RepeatRate, onRepeatExpired, true, TimeLocal);

				RepeatTimer[*timerKeyCode]->setData<int>(timerKeyCode);
				RepeatTimer[*timerKeyCode]->start();
			}
		}
		else {
			if (RepeatTimer[*timerKeyCode]) {
				RepeatTimer[*timerKeyCode]->destroy();
				RepeatTimer[*timerKeyCode] = NULL;
			}
		}
	}

	void onKeyDown(const Event* evt) {
		KeyboardEventData* data = evt->data<KeyboardEventData>();
		int keyCode = data->code;


		if (KeyAllowedToRepeat(keyCode)) {
			if (!RepeatTimer[keyCode]) {
				if (RepeatDelayTimer[keyCode]) {
					RepeatDelayTimer[keyCode]->destroy();
					RepeatDelayTimer[keyCode] = NULL;
				}
				RepeatDelayTimer[keyCode] = GetTimer(RepeatDelay, onRepeatDelayExpired, false, TimeLocal);
				RepeatDelayTimer[keyCode]->setData<int>(&keyCode);
				RepeatDelayTimer[keyCode]->start();
			}
		}
	}

	void onKeyUp(const Event* evt) {
		KeyboardEventData* data = evt->data<KeyboardEventData>();
		int keyCode = data->code;
		if (KeyAllowedToRepeat(keyCode)) {
			if (RepeatDelayTimer[keyCode]) {
				RepeatDelayTimer[keyCode]->destroy();
				RepeatDelayTimer[keyCode] = NULL;
			}
			if (RepeatTimer[keyCode]) {
				RepeatTimer[keyCode]->destroy();
				RepeatTimer[keyCode] = NULL;
				//OutputScreen(10, "key %c cleared.", keyCode);
			}
		}
	}

	void onFocus(const Event* evt) {
		unsigned int size = sizeof(RepeatTimer) / sizeof(Timer*);
		for (unsigned int keyCode = 0; keyCode < size; ++keyCode) {
			if (RepeatDelayTimer[keyCode]) {
				RepeatDelayTimer[keyCode]->destroy();
				RepeatDelayTimer[keyCode] = NULL;
			}
			if (RepeatTimer[keyCode]) {
				RepeatTimer[keyCode]->destroy();
				RepeatTimer[keyCode] = NULL;
			}
		}
	}

	void Init() {
		memset(RepeatTimer, NULL, sizeof(RepeatTimer));
		memset(RepeatDelayTimer, NULL, sizeof(RepeatDelayTimer));
		//RepeatDelay = 0.3f;
		//RepeatRate = 30.f;
		RepeatDelay = 0.3f;
		RepeatRate = 30.f;
		MainDispatcher()->listen(EVENT_KEY_DOWN, onKeyDown);
		MainDispatcher()->listen(EVENT_KEY_UP, onKeyUp);
		MainDispatcher()->listen(EVENT_FOCUS, onFocus);
	}
}