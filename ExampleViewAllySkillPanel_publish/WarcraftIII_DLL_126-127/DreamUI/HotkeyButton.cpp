#include "stdafx.h"
#include "HotkeyButton.h"
#include "Game.h"
#include "Tools.h"
#include "Timer.h"


namespace keyconverter
{
#define IsKeyPressed(CODE) ((GetAsyncKeyState(CODE) & 0x8000) > 0)



	// построение текущих нажатых кнопок в код клавиш
	unsigned int BuildKeyCode()
	{
		unsigned int code = 0;

		if (IsKeyPressed(VK_LSHIFT) ||
			IsKeyPressed(VK_RSHIFT) ||
			IsKeyPressed(VK_SHIFT))
		{
			code += 0x40000;
		}
		else if (IsKeyPressed(VK_MENU) ||
			IsKeyPressed(VK_RMENU) ||
			IsKeyPressed(VK_LMENU))
		{
			code += 0x10000;
		}
		else if (IsKeyPressed(VK_LCONTROL) ||
			IsKeyPressed(VK_RCONTROL) ||
			IsKeyPressed(VK_CONTROL))
		{
			code += 0x20000;
		}

		for (int i = 0; i < 255; i++)
		{
			if (i == (int)VK_LSHIFT ||
				i == (int)VK_RSHIFT ||
				i == (int)VK_SHIFT ||
				i == (int)VK_MENU ||
				i == (int)VK_RMENU ||
				i == (int)VK_LMENU ||
				i == (int)VK_LCONTROL ||
				i == (int)VK_RCONTROL ||
				i == (int)VK_CONTROL)
				continue;
			if (i == (int)VK_MENU)
				continue;
			if (i == (int)VK_CONTROL)
				continue;

			short x = GetAsyncKeyState(i);
			if ((x & 0x8000) > 0)
			{
				code += i;
				break;
			}

		}

		return code;

	}


	// конвертировать строку в код клавиши
	static int _StrToVKey(const std::string  & skey) {
		if (skey == "LBTN") return VK_LBUTTON; // Left mouse button
		if (skey == "RBTN") return VK_RBUTTON; // Right mouse button
		if (skey == "CANCEL") return VK_CANCEL; // Control-break processing
		if (skey == "MBTN") return VK_MBUTTON; // Middle mouse button (three-button mouse)
		if (skey == "XBTN1") return VK_XBUTTON1; // X1 mouse button
		if (skey == "XBTN2") return VK_XBUTTON2; // X2 mouse button
		if (skey == "BACK") return VK_BACK; // BACKSPACE key
		if (skey == "TAB") return VK_TAB; // TAB key
		if (skey == "CLEAR") return VK_CLEAR; // CLEAR key
		if (skey == "RETURN") return VK_RETURN; // ENTER key
		if (skey == "SHIFT") return VK_SHIFT; // SHIFT key
		if (skey == "CTRL") return VK_CONTROL; // CTRL key
		if (skey == "ALT") return VK_MENU; // ALT key
		if (skey == "PAUSE") return VK_PAUSE; // PAUSE key
		if (skey == "CAPS") return VK_CAPITAL; // CAPS LOCK key
		if (skey == "KANA") return VK_KANA; // IME Kana mode
											//if (skey == "VK_HANGUEL") return VK_HANGUL; // IME Hangul mode
		if (skey == "JUNJA") return VK_JUNJA; // IME Junja mode
		if (skey == "FINAL") return VK_FINAL; // IME final mode
		if (skey == "HANJA") return VK_HANJA; // IME Hanja mode
											  //if (skey == "VK_KANJI") return VK_KANJI; // IME Kanji mode
		if (skey == "ESC") return VK_ESCAPE; // ESC key
		if (skey == "CONV") return VK_CONVERT; // IME convert
		if (skey == "NCONV") return VK_NONCONVERT; // IME nonconvert
		if (skey == "ACCEPT") return VK_ACCEPT; // IME accept
		if (skey == "MCHANGE") return VK_MODECHANGE; // IME mode change request
		if (skey == "SPACE") return VK_SPACE; // SPACEBAR
		if (skey == "PAGEUP") return VK_PRIOR; // PAGE UP key
		if (skey == "PAGEDN") return VK_NEXT; // PAGE DOWN key
		if (skey == "END") return VK_END; // END key
		if (skey == "HOME") return VK_HOME; // HOME key
		if (skey == "LEFT") return VK_LEFT; // LEFT ARROW key
		if (skey == "UP") return VK_UP; // UP ARROW key
		if (skey == "RIGHT") return VK_RIGHT; // RIGHT ARROW key
		if (skey == "DOWN") return VK_DOWN; // DOWN ARROW key
		if (skey == "SELECT") return VK_SELECT; // SELECT key
		if (skey == "PRINT") return VK_PRINT; // PRINT key
		if (skey == "EXEC") return VK_EXECUTE; // EXECUTE key
		if (skey == "SSHOT") return VK_SNAPSHOT; // PRINT SCREEN key
		if (skey == "INSERT") return VK_INSERT; // INS key
		if (skey == "DELETE") return VK_DELETE; // DEL key
		if (skey == "HELP") return VK_HELP; // HELP key

		if (skey == "0") return '0';
		if (skey == "1") return '1';
		if (skey == "2") return '2';
		if (skey == "3") return '3';
		if (skey == "4") return '4';
		if (skey == "5") return '5';
		if (skey == "6") return '6';
		if (skey == "7") return '7';
		if (skey == "8") return '8';
		if (skey == "9") return '9';
		if (skey == "A") return 'A';
		if (skey == "B") return 'B';
		if (skey == "C") return 'C';
		if (skey == "D") return 'D';
		if (skey == "E") return 'E';
		if (skey == "F") return 'F';
		if (skey == "G") return 'G';
		if (skey == "H") return 'H';
		if (skey == "I") return 'I';
		if (skey == "J") return 'J';
		if (skey == "K") return 'K';
		if (skey == "L") return 'L';
		if (skey == "M") return 'M';
		if (skey == "N") return 'N';
		if (skey == "O") return 'O';
		if (skey == "P") return 'P';
		if (skey == "Q") return 'Q';
		if (skey == "R") return 'R';
		if (skey == "S") return 'S';
		if (skey == "T") return 'T';
		if (skey == "U") return 'U';
		if (skey == "V") return 'V';
		if (skey == "W") return 'W';
		if (skey == "X") return 'X';
		if (skey == "Y") return 'Y';
		if (skey == "Z") return 'Z';

		if (skey == "LWIN") return VK_LWIN; // Left Windows key (Natural keyboard)
		if (skey == "RWIN") return VK_RWIN; // Right Windows key (Natural keyboard)
		if (skey == "APPS") return VK_APPS; // Applications key (Natural keyboard)
		if (skey == "SLEEP") return VK_SLEEP; // Computer Sleep key
		if (skey == "NPAD0") return VK_NUMPAD0; // Numeric keypad 0 key
		if (skey == "NPAD1") return VK_NUMPAD1; // Numeric keypad 1 key
		if (skey == "NPAD2") return VK_NUMPAD2; // Numeric keypad 2 key
		if (skey == "NPAD3") return VK_NUMPAD3; // Numeric keypad 3 key
		if (skey == "NPAD4") return VK_NUMPAD4; // Numeric keypad 4 key
		if (skey == "NPAD5") return VK_NUMPAD5; // Numeric keypad 5 key
		if (skey == "NPAD6") return VK_NUMPAD6; // Numeric keypad 6 key
		if (skey == "NPAD7") return VK_NUMPAD7; // Numeric keypad 7 key
		if (skey == "NPAD8") return VK_NUMPAD8; // Numeric keypad 8 key
		if (skey == "NPAD9") return VK_NUMPAD9; // Numeric keypad 9 key
		if (skey == "MULT") return VK_MULTIPLY; // Multiply key
		if (skey == "ADD") return VK_ADD; // Add key
		if (skey == "SEP") return VK_SEPARATOR; // Separator key
		if (skey == "SUB") return VK_SUBTRACT; // Subtract key
		if (skey == "DEC") return VK_DECIMAL; // Decimal key
		if (skey == "DIV") return VK_DIVIDE; // Divide key
		if (skey == "F1") return VK_F1; // F1 key
		if (skey == "F2") return VK_F2; // F2 key
		if (skey == "F3") return VK_F3; // F3 key
		if (skey == "F4") return VK_F4; // F4 key
		if (skey == "F5") return VK_F5; // F5 key
		if (skey == "F6") return VK_F6; // F6 key
		if (skey == "F7") return VK_F7; // F7 key
		if (skey == "F8") return VK_F8; // F8 key
		if (skey == "F9") return VK_F9; // F9 key
		if (skey == "F10") return VK_F10; // F10 key
		if (skey == "F11") return VK_F11; // F11 key
		if (skey == "F12") return VK_F12; // F12 key
		if (skey == "F13") return VK_F13; // F13 key
		if (skey == "F14") return VK_F14; // F14 key
		if (skey == "F15") return VK_F15; // F15 key
		if (skey == "F16") return VK_F16; // F16 key
		if (skey == "F17") return VK_F17; // F17 key
		if (skey == "F18") return VK_F18; // F18 key
		if (skey == "F19") return VK_F19; // F19 key
		if (skey == "F20") return VK_F20; // F20 key
		if (skey == "F21") return VK_F21; // F21 key
		if (skey == "F22") return VK_F22; // F22 key
		if (skey == "F23") return VK_F23; // F23 key
		if (skey == "F24") return VK_F24; // F24 key
		if (skey == "NLOCK") return VK_NUMLOCK; // NUM LOCK key
		if (skey == "SCRL") return VK_SCROLL; // SCROLL LOCK key
		if (skey == "LSHFT") return VK_LSHIFT; // Left SHIFT key
		if (skey == "RSHFT") return VK_RSHIFT; // Right SHIFT key
		if (skey == "LCTRL") return VK_LCONTROL; // Left CONTROL key
		if (skey == "RCTRL") return VK_RCONTROL; // Right CONTROL key
		if (skey == "LALT") return VK_LMENU; // Left MENU key
		if (skey == "RALT") return VK_RMENU; // Right MENU key
		if (skey == "BBACK") return VK_BROWSER_BACK; // Browser Back key
		if (skey == "BFORW") return VK_BROWSER_FORWARD; // Browser Forward key
		if (skey == "BREFR") return VK_BROWSER_REFRESH; // Browser Refresh key
		if (skey == "BSTOP") return VK_BROWSER_STOP; // Browser Stop key
		if (skey == "BSEARCH") return VK_BROWSER_SEARCH; // Browser Search key
		if (skey == "BFAV") return VK_BROWSER_FAVORITES; // Browser Favorites key
		if (skey == "BHOME") return VK_BROWSER_HOME; // Browser Start and Home key
		if (skey == "MUTE") return VK_VOLUME_MUTE; // Volume Mute key
		if (skey == "V_DOWN") return VK_VOLUME_DOWN; // Volume Down key
		if (skey == "V_UP") return VK_VOLUME_UP; // Volume Up key
		if (skey == "NEXT") return VK_MEDIA_NEXT_TRACK; // Next Track key
		if (skey == "PREV") return VK_MEDIA_PREV_TRACK; // Previous Track key
		if (skey == "STOP") return VK_MEDIA_STOP; // Stop Media key
		if (skey == "MPLAY") return VK_MEDIA_PLAY_PAUSE; // Play/Pause Media key
		if (skey == "MAIL") return VK_LAUNCH_MAIL; // Start Mail key
		if (skey == "MSEL") return VK_LAUNCH_MEDIA_SELECT; // Select Media key
		if (skey == "APP1") return VK_LAUNCH_APP1; // Start Application 1 key
		if (skey == "APP2") return VK_LAUNCH_APP2; // Start Application 2 key
		if (skey == "OEM_1") return VK_OEM_1; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
		if (skey == "OEM_P") return VK_OEM_PLUS; // For any country/region, the '+' key
		if (skey == "COMMA") return VK_OEM_COMMA; // For any country/region, the ',' key
		if (skey == "MINUS") return VK_OEM_MINUS; // For any country/region, the '-' key
		if (skey == "PERIOD") return VK_OEM_PERIOD; // For any country/region, the '.' key
		if (skey == "OEM_2") return VK_OEM_2; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
		if (skey == "OEM_3") return VK_OEM_3; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
		if (skey == "ABNT_C1") return 0xC1; // Brazilian (ABNT) Keyboard
		if (skey == "ABNT_C2") return 0xC2; // Brazilian (ABNT) Keyboard
		if (skey == "OEM_4") return VK_OEM_4; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
		if (skey == "OEM_5") return VK_OEM_5; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
		if (skey == "OEM_6") return VK_OEM_6; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
		if (skey == "OEM_7") return VK_OEM_7; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
		if (skey == "OEM_8") return VK_OEM_8; // Used for miscellaneous characters; it can vary by keyboard.
		if (skey == "OEM102") return VK_OEM_102; // Either the angle bracket key or the backslash key on the RT 102-key keyboard
		if (skey == "PROCKEY") return VK_PROCESSKEY; // IME PROCESS key
		if (skey == "PACKET") return VK_PACKET; // Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP // 0xE8
		if (skey == "ATTN") return VK_ATTN; // Attn key
		if (skey == "CRSEL") return VK_CRSEL; // CrSel key
		if (skey == "EXSEL") return VK_EXSEL; // ExSel key
		if (skey == "EREOF") return VK_EREOF; // Erase EOF key
		if (skey == "PLAY") return VK_PLAY; // Play key
		if (skey == "ZOOM") return VK_ZOOM; // Zoom key
		if (skey == "NONAME") return VK_NONAME; // Reserved
		if (skey == "PA1") return VK_PA1; // PA1 key
		if (skey == "OCLEAR") return VK_OEM_CLEAR; // Clear key

		return 0;
	}

	// конвертировать код клавиши в строку
	static std::string _VKeyToStr(int vkey) {
		switch (vkey) {
		case VK_LBUTTON: return "LBTN"; // Left mouse button
		case VK_RBUTTON: return "RBTN"; // Right mouse button
		case VK_CANCEL: return "CANCEL"; // Control-break processing
		case VK_MBUTTON: return "MBTN"; // Middle mouse button (three-button mouse)
		case VK_XBUTTON1: return "XBTN1"; // X1 mouse button
		case VK_XBUTTON2: return "XBTN2"; // X2 mouse button
		case VK_BACK: return "BACK"; // BACKSPACE key
		case VK_TAB: return "TAB"; // TAB key
		case VK_CLEAR: return "CLEAR"; // CLEAR key
		case VK_RETURN: return "RETURN"; // ENTER key
		case VK_SHIFT: return "SHIFT"; // SHIFT key
		case VK_CONTROL: return "CTRL"; // CTRL key
		case VK_MENU: return "ALT"; // ALT key
		case VK_PAUSE: return "PAUSE"; // PAUSE key
		case VK_CAPITAL: return "CAPS"; // CAPS LOCK key
		case VK_KANA: return "KANA"; // IME Kana mode
									 //case VK_HANGUL: return "VK_HANGUEL"; // IME Hangul mode
		case VK_JUNJA: return "JUNJA"; // IME Junja mode
		case VK_FINAL: return "FINAL"; // IME final mode
		case VK_HANJA: return "HANJA"; // IME Hanja mode
									   //case VK_KANJI: return "VK_KANJI"; // IME Kanji mode
		case VK_ESCAPE: return "ESC"; // ESC key
		case VK_CONVERT: return "CONV"; // IME convert
		case VK_NONCONVERT: return "NCONV"; // IME nonconvert
		case VK_ACCEPT: return "ACCEPT"; // IME accept
		case VK_MODECHANGE: return "MCHANGE"; // IME mode change request
		case VK_SPACE: return "SPACE"; // SPACEBAR
		case VK_PRIOR: return "PAGEUP"; // PAGE UP key
		case VK_NEXT: return "PAGEDN"; // PAGE DOWN key
		case VK_END: return "END"; // END key
		case VK_HOME: return "HOME"; // HOME key
		case VK_LEFT: return "LEFT"; // LEFT ARROW key
		case VK_UP: return "UP"; // UP ARROW key
		case VK_RIGHT: return "RIGHT"; // RIGHT ARROW key
		case VK_DOWN: return "DOWN"; // DOWN ARROW key
		case VK_SELECT: return "SELECT"; // SELECT key
		case VK_PRINT: return "PRINT"; // PRINT key
		case VK_EXECUTE: return "EXEC"; // EXECUTE key
		case VK_SNAPSHOT: return "SSHOT"; // PRINT SCREEN key
		case VK_INSERT: return "INSERT"; // INS key
		case VK_DELETE: return "DELETE"; // DEL key
		case VK_HELP: return "HELP"; // HELP key

		case '0': return "0";
		case '1': return "1";
		case '2': return "2";
		case '3': return "3";
		case '4': return "4";
		case '5': return "5";
		case '6': return "6";
		case '7': return "7";
		case '8': return "8";
		case '9': return "9";
		case 'A': return "A";
		case 'B': return "B";
		case 'C': return "C";
		case 'D': return "D";
		case 'E': return "E";
		case 'F': return "F";
		case 'G': return "G";
		case 'H': return "H";
		case 'I': return "I";
		case 'J': return "J";
		case 'K': return "K";
		case 'L': return "L";
		case 'M': return "M";
		case 'N': return "N";
		case 'O': return "O";
		case 'P': return "P";
		case 'Q': return "Q";
		case 'R': return "R";
		case 'S': return "S";
		case 'T': return "T";
		case 'U': return "U";
		case 'V': return "V";
		case 'W': return "W";
		case 'X': return "X";
		case 'Y': return "Y";
		case 'Z': return "Z";

		case VK_LWIN: return "LWIN"; // Left Windows key (Natural keyboard)
		case VK_RWIN: return "RWIN"; // Right Windows key (Natural keyboard)
		case VK_APPS: return "APPS"; // Applications key (Natural keyboard)
		case VK_SLEEP: return "SLEEP"; // Computer Sleep key
		case VK_NUMPAD0: return "NPAD0"; // Numeric keypad 0 key
		case VK_NUMPAD1: return "NPAD1"; // Numeric keypad 1 key
		case VK_NUMPAD2: return "NPAD2"; // Numeric keypad 2 key
		case VK_NUMPAD3: return "NPAD3"; // Numeric keypad 3 key
		case VK_NUMPAD4: return "NPAD4"; // Numeric keypad 4 key
		case VK_NUMPAD5: return "NPAD5"; // Numeric keypad 5 key
		case VK_NUMPAD6: return "NPAD6"; // Numeric keypad 6 key
		case VK_NUMPAD7: return "NPAD7"; // Numeric keypad 7 key
		case VK_NUMPAD8: return "NPAD8"; // Numeric keypad 8 key
		case VK_NUMPAD9: return "NPAD9"; // Numeric keypad 9 key
		case VK_MULTIPLY: return "MULT"; // Multiply key
		case VK_ADD: return "ADD"; // Add key
		case VK_SEPARATOR: return "SEP"; // Separator key
		case VK_SUBTRACT: return "SUB"; // Subtract key
		case VK_DECIMAL: return "DEC"; // Decimal key
		case VK_DIVIDE: return "DIV"; // Divide key
		case VK_F1: return "F1"; // F1 key
		case VK_F2: return "F2"; // F2 key
		case VK_F3: return "F3"; // F3 key
		case VK_F4: return "F4"; // F4 key
		case VK_F5: return "F5"; // F5 key
		case VK_F6: return "F6"; // F6 key
		case VK_F7: return "F7"; // F7 key
		case VK_F8: return "F8"; // F8 key
		case VK_F9: return "F9"; // F9 key
		case VK_F10: return "F10"; // F10 key
		case VK_F11: return "F11"; // F11 key
		case VK_F12: return "F12"; // F12 key
		case VK_F13: return "F13"; // F13 key
		case VK_F14: return "F14"; // F14 key
		case VK_F15: return "F15"; // F15 key
		case VK_F16: return "F16"; // F16 key
		case VK_F17: return "F17"; // F17 key
		case VK_F18: return "F18"; // F18 key
		case VK_F19: return "F19"; // F19 key
		case VK_F20: return "F20"; // F20 key
		case VK_F21: return "F21"; // F21 key
		case VK_F22: return "F22"; // F22 key
		case VK_F23: return "F23"; // F23 key
		case VK_F24: return "F24"; // F24 key
		case VK_NUMLOCK: return "NLOCK"; // NUM LOCK key
		case VK_SCROLL: return "SCRL"; // SCROLL LOCK key
		case VK_LSHIFT: return "LSHFT"; // Left SHIFT key
		case VK_RSHIFT: return "RSHFT"; // Right SHIFT key
		case VK_LCONTROL: return "LCTRL"; // Left CONTROL key
		case VK_RCONTROL: return "RCTRL"; // Right CONTROL key
		case VK_LMENU: return "LALT"; // Left MENU key
		case VK_RMENU: return "RALT"; // Right MENU key
		case VK_BROWSER_BACK: return "BBACK"; // Browser Back key
		case VK_BROWSER_FORWARD: return "BFORW"; // Browser Forward key
		case VK_BROWSER_REFRESH: return "BREFR"; // Browser Refresh key
		case VK_BROWSER_STOP: return "BSTOP"; // Browser Stop key
		case VK_BROWSER_SEARCH: return "BSEARCH"; // Browser Search key
		case VK_BROWSER_FAVORITES: return "BFAV"; // Browser Favorites key
		case VK_BROWSER_HOME: return "BHOME"; // Browser Start and Home key
		case VK_VOLUME_MUTE: return "MUTE"; // Volume Mute key
		case VK_VOLUME_DOWN: return "V_DOWN"; // Volume Down key
		case VK_VOLUME_UP: return "V_UP"; // Volume Up key
		case VK_MEDIA_NEXT_TRACK: return "NEXT"; // Next Track key
		case VK_MEDIA_PREV_TRACK: return "PREV"; // Previous Track key
		case VK_MEDIA_STOP: return "STOP"; // Stop Media key
		case VK_MEDIA_PLAY_PAUSE: return "MPLAY"; // Play/Pause Media key
		case VK_LAUNCH_MAIL: return "MAIL"; // Start Mail key
		case VK_LAUNCH_MEDIA_SELECT: return "MSEL"; // Select Media key
		case VK_LAUNCH_APP1: return "APP1"; // Start Application 1 key
		case VK_LAUNCH_APP2: return "APP2"; // Start Application 2 key
		case VK_OEM_1: return "OEM_1"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
		case VK_OEM_PLUS: return "OEM_P"; // For any country/region, the '+' key
		case VK_OEM_COMMA: return "COMMA"; // For any country/region, the ',' key
		case VK_OEM_MINUS: return "MINUS"; // For any country/region, the '-' key
		case VK_OEM_PERIOD: return "PERIOD"; // For any country/region, the '.' key
		case VK_OEM_2: return "OEM_2"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
		case VK_OEM_3: return "OEM_3"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
		case 0xC1: return "ABNT_C1"; // Brazilian (ABNT) Keyboard
		case 0xC2: return "ABNT_C2"; // Brazilian (ABNT) Keyboard
		case VK_OEM_4: return "OEM_4"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
		case VK_OEM_5: return "OEM_5"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
		case VK_OEM_6: return "OEM_6"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
		case VK_OEM_7: return "OEM_7"; // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key
		case VK_OEM_8: return "OEM_8"; // Used for miscellaneous characters; it can vary by keyboard.
		case VK_OEM_102: return "OEM102"; // Either the angle bracket key or the backslash key on the RT 102-key keyboard
		case VK_PROCESSKEY: return "PROCKEY"; // IME PROCESS key
		case VK_PACKET: return "PACKET"; // Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP // 0xE8
		case VK_ATTN: return "ATTN"; // Attn key
		case VK_CRSEL: return "CRSEL"; // CrSel key
		case VK_EXSEL: return "EXSEL"; // ExSel key
		case VK_EREOF: return "EREOF"; // Erase EOF key
		case VK_PLAY: return "PLAY"; // Play key
		case VK_ZOOM: return "ZOOM"; // Zoom key
		case VK_NONAME: return "NONAME"; // Reserved
		case VK_PA1: return "PA1"; // PA1 key
		case VK_OEM_CLEAR: return "OCLEAR"; // Clear key
		}

		return "NOTHING";
	}

	// конвертировать код клавиши в строку
	static std::string CovertKeyCodeToString(unsigned int val)
	{
		std::string outstr;

		if (val == 0)
		{
			return "";
		}

		int KeyVal = (int)(val & 0xFF);

		outstr = _VKeyToStr(KeyVal);

		if ((val & 0x40000) > 0)
		{
			outstr = "SHIFT+" + outstr;
		}
		if ((val & 0x10000) > 0)
		{
			outstr = "ALT+" + outstr;
		}
		if ((val & 0x20000) > 0)
		{
			outstr = "CTRL+" + outstr;
		}

		if ( !KeyVal )
			return "";

		return outstr;
	}

	// конвертировать строку в код клавиши (комбинации клавиш)
	static unsigned int CovertStringToKeyCode(std::string code)
	{
		if (code.length() == 0)
			return 0;

		unsigned int outcode = 0;
		char * arg2;
		if ((arg2 = strstr(&code[0], "+")) != NULL)
		{
			arg2[0] = '\0';
			arg2++;
			std::string arg1 = &code[0];
			if (arg1 == "CTRL")
			{
				outcode = 0x20000;
			}
			else if (arg1 == "ALT")
			{
				outcode = 0x10000;
			}
			else if (arg1 == "SHIFT")
			{
				outcode = 0x40000;
			}
		}
		else arg2 = &code[0];

		int vkeyout = _StrToVKey(arg2);

		outcode += vkeyout;

		if (vkeyout == 0)
			return 0;

		return outcode;
	}

}

static std::set<HotkeyButton *> HotkeyButtonSet;


bool  HotkeyButton::IsAnyHotkeyActive()
{
	for (std::set<HotkeyButton *>::iterator iter = HotkeyButtonSet.begin();
		iter != HotkeyButtonSet.end(); ++iter)
	{
		if ((*iter)->isEnteringHotkey()) {
			if ((*iter)->getTimer())
			{
				return true;
			}

		}
	}

	return false;
}

static void HotkeyUpdate(Timer* tm)
{
	for (std::set<HotkeyButton *>::iterator iter = HotkeyButtonSet.begin();
		iter != HotkeyButtonSet.end(); ++iter)
	{
		if ((*iter)->isEnteringHotkey()) {
			if ((*iter)->getTimer())
			{
				(*iter)->setText((std::to_string((*iter)->getTimer()) + std::string("...")).c_str());
				(*iter)->setTimer((*iter)->getTimer() - 1);
			}
			else if ((*iter)->getTimer() <= 0)
			{
				(*iter)->setTimer(-1);
				(*iter)->toggleEnteringHotkey();
				(*iter)->setKeyEx(keyconverter::BuildKeyCode( ));
				SoundPlay("GlueScreenClick", NULL, 0);
				if ((*iter)->hotkeyButtonCallback)
				{
					(*iter)->hotkeyButtonCallback((*iter));
				}
			}

			break;
		}
	}
}

void CallbackHotkeyButton(Button *btn) {
	HotkeyButton *btnhot = (HotkeyButton *)btn;


	for ( std::set<HotkeyButton *>::iterator iter = HotkeyButtonSet.begin( );
		iter != HotkeyButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringHotkey( ) ) {
			return;
		}
	}


	btnhot->toggleEnteringHotkey( );

	if (btnhot->isEnteringHotkey()) {
		//set text to "..."
		btnhot->setTimer(2);
		btnhot->setText("3...");
	}
	else {
		//set back text
		btnhot->setKeyEx(btnhot->getKey());
	}
}

void DetectHotkey(const Event *evt) {
	KeyboardEventData *data = evt->data<KeyboardEventData>();
	//OutputScreen(10, "langid = %d, key = 0x%X, name = %s", (int)LOWORD((DWORD)GetKeyboardLayout(0)), data->code, KEYCODE::getStr(data->code));
	for (std::set<HotkeyButton *>::iterator iter = HotkeyButtonSet.begin();
		iter != HotkeyButtonSet.end(); ++iter)
	{
		if ((*iter)->isEnteringHotkey()) {
			(*iter)->toggleEnteringHotkey();
			(*iter)->setKey(data->code);
			SoundPlay("GlueScreenClick", NULL, 0);
			if ((*iter)->hotkeyButtonCallback)
			{
				(*iter)->hotkeyButtonCallback((*iter));
			}
			data->discard();	 
			DiscardCurrentEvent();
			break;
		}
	}
}

void HotkeyButton::setKey(int keyCode) {
	std::string str;
	str += "|cffffcc00";
	str += KEYCODE::
		//ToString(keyCode);
		getStr(keyCode);
	str += "|r";
	this->_hotkey = keyCode;
	this->setText(str.c_str());
	//	if (_profileApp && _profileKey) ProfileSetInt(_profileApp, _profileKey, keyCode);
	if (_pVarHotkey) *(_pVarHotkey) = keyCode;
}


void HotkeyButton::setKeyEx(unsigned int keyCode) {
	std::string str;
	str += "|cffffcc00";
	str += keyconverter::CovertKeyCodeToString(keyCode);
	str += "|r";
	this->_hotkey = keyCode;
	this->setText(str.c_str());
	//	if (_profileApp && _profileKey) ProfileSetInt(_profileApp, _profileKey, keyCode);
	if (_pVarHotkey) *(_pVarHotkey) = keyCode;
}

HotkeyButton::HotkeyButton(
	UISimpleFrame*			parent,
	float					width,
	float					height,
	int*					hotkeyVar,
	int						defaultHotkey,
	char*					profileApp,
	char*					profileKey,
	ButtonCallback			callback,
	bool					noSurface
) : Button(parent, width, height, UISimpleButton::MOUSEBUTTON_LEFT, UISimpleButton::STATE_ENABLED, CallbackHotkeyButton, NULL, false, noSurface),
_hotkey(NULL), _enteringHotkey(false), _pVarHotkey(hotkeyVar), _profileApp(profileApp), _profileKey(profileKey),
hotkeyButtonCallback(callback)
{
	if (hotkeyVar)
		*hotkeyVar = defaultHotkey;
	this->setKeyEx(defaultHotkey);
	this->setTimer(-1);
	HotkeyButtonSet.insert(this);
}

HotkeyButton::~HotkeyButton() {
}

void HotkeyButton::Init() {
	HotkeyButtonSet.clear();
	GetTimer(1, HotkeyUpdate, true,TimeType::TimeGame)->start();
}

void HotkeyButton::Cleanup() {
	HotkeyButtonSet.clear();
}

void HotkeyButton::StopInput( )
{
	for ( std::set<HotkeyButton *>::iterator iter = HotkeyButtonSet.begin( );
		iter != HotkeyButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringHotkey( ) ) {
			( *iter )->toggleEnteringHotkey( );
			( *iter )->setText( "????" );
		}
	}
}
