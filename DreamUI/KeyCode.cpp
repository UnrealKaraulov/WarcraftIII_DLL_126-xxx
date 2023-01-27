#include "stdafx.h"
#include "KeyCode.h"

namespace KEYCODE {
	const char* getStr(unsigned int keyCode)
	{
		int locale = (int)LOWORD((unsigned long)GetKeyboardLayout(0));

		switch (keyCode) {
		case KEY_SHIFT:
			return "Shift";
		case KEY_CONTROL:
			return "Ctrl";
		case KEY_ALT:
			return "Alt";
		case KEY_SPACE:
			return "Space";

		case KEY_0:
			return "0";
		case KEY_1:
			return "1";
		case KEY_2:
			return "2";
		case KEY_3:
			return "3";
		case KEY_4:
			return "4";
		case KEY_5:
			return "5";
		case KEY_6:
			return "6";
		case KEY_7:
			return "7";
		case KEY_8:
			return "8";
		case KEY_9:
			return "9";

		case KEY_A:
			return "A";
		case KEY_B:
			return "B";
		case KEY_C:
			return "C";
		case KEY_D:
			return "D";
		case KEY_E:
			return "E";
		case KEY_F:
			return "F";
		case KEY_G:
			return "G";
		case KEY_H:
			return "H";
		case KEY_I:
			return "I";
		case KEY_J:
			return "J";
		case KEY_K:
			return "K";
		case KEY_L:
			return "L";
		case KEY_M:
			return "M";
		case KEY_N:
			return "N";
		case KEY_O:
			return "O";
		case KEY_P:
			return "P";
		case KEY_Q:
			return "Q";
		case KEY_R:
			return "R";
		case KEY_S:
			return "S";
		case KEY_T:
			return "T";
		case KEY_U:
			return "U";
		case KEY_V:
			return "V";
		case KEY_W:
			return "W";
		case KEY_X:
			return "X";
		case KEY_Y:
			return "Y";
		case KEY_Z:
			return "Z";


		case KEY_TILDE:
			if (locale == 1031) return "\xC3\x96";//"Ö";
			return "`";
		case KEY_NUMPAD0:
			return "Num 0";
		case KEY_NUMPAD1:
			return "Num 1";
		case KEY_NUMPAD2:
			return "Num 2";
		case KEY_NUMPAD3:
			return "Num 3";
		case KEY_NUMPAD4:
			return "Num 4";
		case KEY_NUMPAD5:
			return "Num 5";
		case KEY_NUMPAD6:
			return "Num 6";
		case KEY_NUMPAD7:
			return "Num 7";
		case KEY_NUMPAD8:
			return "Num 8";
		case KEY_NUMPAD9:
			return "Num 9";
		case KEY_ADD:
			return "Num +";
		case KEY_SUBTRACT:
			return "Num -";
		case KEY_MULTIPLY:
			return "Num *";
		case KEY_DIVIDE:
			return "Num /";
		case KEY_DECIMAL:
			return "Num Del";

		case KEY_OEM_PLUS:
			if (locale == 1031) return "+";
			return "=";
		case KEY_OEM_MINUS:
			return "-";
		case KEY_OEM_OBRACKE:
			if (locale == 1031) return "\xC3\x9F";//"ß";
			return "[";
		case KEY_OEM_CBRACKE:
			return "]";
		case KEY_OEM_BACKSLASH:
			if (locale == 1031) return "^";
			return "\\";
		case KEY_OEM_SEMICOLON:
			if (locale == 1031) return "\xC3\x9C";//"Ü";
			return ";";
		case KEY_OEM_SQUOTMARKS:
			if (locale == 1031) return "\xC3\x84";//"Ä";
			return "'";
		case KEY_OEM_COMMA:
			return ",";
		case KEY_OEM_PERIOD:
			return ".";
		case KEY_OEM_SLASH:
			if (locale == 1031) return "#";
			return "/";
		case KEY_ESC:
			return "Esc";
		case KEY_ENTER:
			return "Enter";
		case KEY_BACKSPACE:
			return "Backspace";
		case KEY_TAB:
			return "Tab";
		case KEY_LEFT:
			return "Left";
		case KEY_UP:
			return "Up";
		case KEY_RIGHT:
			return "Right";
		case KEY_DOWN:
			return "Down";
		case KEY_INSERT:
			return "Insert";
		case KEY_DELETE:
			return "Delete";
		case KEY_HOME:
			return "Home";
		case KEY_END:
			return "End";
		case KEY_PAGEUP:
			return "Page Up";
		case KEY_PAGEDOWN:
			return "Page Down";
		case KEY_CAPSLOCK:
			return "Caps Lock";
		case KEY_NUMLOCK:
			return "Num Lock";
		case KEY_SCROLLLOCK:
			return "Scroll Lock";
		case KEY_PAUSE:
			return "Pause";
		case KEY_PRINTSCREEN:
			return "Print Screen";

		case KEY_F1:
			return "F1";
		case KEY_F2:
			return "F2";
		case KEY_F3:
			return "F3";
		case KEY_F4:
			return "F4";
		case KEY_F5:
			return "F5";
		case KEY_F6:
			return "F6";
		case KEY_F7:
			return "F7";
		case KEY_F8:
			return "F8";
		case KEY_F9:
			return "F9";
		case KEY_F10:
			return "F10";
		case KEY_F11:
			return "F11";
		case KEY_F12:
			return "F12";
			//custom
		case KEY_MOUSE_LEFT:
			return (const char*)"LMB";
		case KEY_MOUSE_RIGHT:
			return (const char*)"RMB";
		case KEY_MOUSE_MIDDLE:
			return (const char*)"MMB";
		case KEY_MOUSE_X1:
			return (const char*)"X1";
		case KEY_MOUSE_X2:
			return (const char*)"X2";
		case KEY_MOUSE_SCROLL_DOWN:
			return (const char*)"SDOWN";
		case KEY_MOUSE_SCROLL_UP:
			return (const char*)"SUP";

		default:
			return "N/A";
		}
	}

	int MouseCodeToKeyCode(int mouseCode)
	{
		//左键=1 中键=2 右键=4 左边靠后的侧键=8 左边靠前的侧键=0x10
		switch (mouseCode)
		{
		case 1:
			return KEY_MOUSE_LEFT;
		case 2:
			return KEY_MOUSE_MIDDLE;
		case 4:
			return KEY_MOUSE_RIGHT;
		case 8:
			return KEY_MOUSE_X1;
		case 0x10:
			return KEY_MOUSE_X2;
		default:
			break;
		}
		return -1;
	}

}