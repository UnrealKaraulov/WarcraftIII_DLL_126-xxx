#include "stdafx.h"
#ifndef KEYCODE_H_
#define KEYCODE_H_

namespace KEYCODE {

	inline uint32_t ToVKeyCode(int war3key) {
		int result = 0;
		uint32_t key = static_cast<uint32_t>(war3key);
		uint32_t tmp;
		if ((key >= 0x30u && key <= 0x39u) || (key >= 0x41u && key <= 0x5Au))
			return war3key;
		if (key > 0x290u) {
			tmp = key - 0x290u;
			if (tmp >= 0x70u && tmp <= 0x7Bu)
				return war3key - 0x290u;
		}
		switch ( key ) {
			case 0x202u:
				result = 0x8;
				break;
			case 0x203u:
				result = 0x9;
				break;
			case 0x201u:
				result = 0xD;
				break;
			case 0:
				result = 0x10;
				break;
			case 0x1:
				result = 0x11;
				break;
			case 0x2:
				result = 0x12;
				break;
			case 0x20Eu:
				result = 0x14;
				break;
			case 0x200u:
				result = 0x1B;
				break;
			case 0x204u:
				result = 0x25;
				break;
			case 0x205u:
				result = 0x26;
				break;
			case 0x206u:
				result = 0x27;
				break;
			case 0x207u:
				result = 0x28;
				break;
			case 0x208u:
				result = 0x2D;
				break;
			case 0x209u:
				result = 0x2E;
				break;
			case 0x20Cu:
				result = 0x21;
				break;
			case 0x20Du:
				result = 0x22;
				break;
			case 0x20Bu:
				result = 0x23;
				break;
			case 0x20Au:
				result = 0x24;
				break;
			case 0x20Fu:
				result = 0x90;
				break;
			case 0x210u:
				result = 0x91;
				break;
			case 0x211u:
				result = 0x13;
				break;
			case 0x212u:
				result = 0x2C;
				break;
			case 0x20u:
				result = 0x20;
				break;
			case 0x100u:
				result = 0xC0;
				break;
			case 0x101u:
				result = 0x60;
				break;
			case 0x102u:
				result = 0x61;
				break;
			case 0x103u:
				result = 0x62;
				break;
			case 0x104u:
				result = 0x63;
				break;
			case 0x105u:
				result = 0x64;
				break;
			case 0x106u:
				result = 0x65;
				break;
			case 0x107u:
				result = 0x66;
				break;
			case 0x108u:
				result = 0x67;
				break;
			case 0x109u:
				result = 0x68;
				break;
			case 0x10Au:
				result = 0x69;
				break;
			case 0x10Bu:
				result = 0x6B;
				break;
			case 0x10Cu:
				result = 0x6D;
				break;
			case 0x10Du:
				result = 0x6A;
				break;
			case 0x10Eu:
				result = 0x6F;
				break;
			case 0x10Fu:
				result = 0x6E;
				break;
			case 0x110u:
				result = 0xBB;
				break;
			case 0x111u:
				result = 0xBD;
				break;
			case 0x112u:
				result = 0xDB;
				break;
			case 0x113u:
				result = 0xDD;
				break;
			case 0x114u:
				result = 0xDC;
				break;
			case 0x115u:
				result = 0xBA;
				break;
			case 0x116:
				result = 0xDE;
				break;
			case 0x117:
				result = 0xBC;
				break;
			case 0x118u:
				result = 0xBE;
				break;
			case 0x119u:
				result = 0xBF;
				break;
		}
		return result;
	}

	inline const char* ToString(int war3key) {
		static char Rv[50];
		static wchar_t Unicode[3] = {0};
		uint32_t vkeycode = ToVKeyCode(war3key);
		unsigned int scanCode = MapVirtualKey(vkeycode, MAPVK_VK_TO_VSC);

		// because MapVirtualKey strips the extended bit for some keys
		switch (vkeycode)
		{
			case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys
			case VK_PRIOR: case VK_NEXT: // page up and page down
			case VK_END: case VK_HOME:
			case VK_INSERT: case VK_DELETE:
			case VK_DIVIDE: // numpad slash
			case VK_NUMLOCK:
			{
				scanCode |= 0x100; // set extended bit
				break;
			}
		}

		if (GetKeyNameTextA(scanCode << 16, Rv, sizeof(Rv)) != 0)
		{
			return Rv;
		}
		else
		{
			return "[Error]";
		}
		return Rv;
	}

	inline int VKeyCodeToWar3KeyCode(int vkey)
	{
		int result = 0;    
		if ((vkey >= 0x30u && vkey <= 0x39u) || (vkey >= 0x41u && vkey <= 0x5Au))
			return vkey;
		if (vkey >= 0x70u && vkey <= 0x7Bu)
			return vkey + 0x290u;
		switch ( vkey ) {
			case 0x10:
        		result = 0;        	
				break;
			case 0x11:
        		result = 0x1;
        		break;
			case 0x12:
        		result = 0x2;
        		break;
			case 0x1B:
        		result = 0x200u;
        		break;
			case 0xD:
        		result = 0x201u;
        		break;
			case 8:
        		result = 0x202u;
        		break;
			case 9:
        		result = 0x203u;
        		break;
			case 0x25:
        		result = 0x204u;
        		break;
			case 0x26:
        		result = 0x205u;
        		break;
			case 0x27:
        		result = 0x206u;
        		break;
			case 0x28:
        		result = 0x207u;
        		break;
			case 0x2D:
        		result = 0x208u;
        		break;
			case 0x2E:
        		result = 0x209u;
        		break;
			case 0x24:
        		result = 0x20Au;
        		break;
			case 0x23:
        		result = 0x20Bu;
        		break;
			case 0x21:
        		result = 0x20Cu;
        		break;
			case 0x22:
        		result = 0x20Du;
        		break;
			case 0x14:
        		result = 0x20Eu;
        		break;
			case 0x90:
        		result = 0x20Fu;
        		break;
			case 0x91:
        		result = 0x210u;
        		break;
			case 0x13:
        		result = 0x211u;
        		break;
			case 0x2C:
        		result = 0x212u;
        		break;
			case 0x20:
        		result = 0x20u;
        		break;
			case 0xC0:
        		result = 0x100u;
        		break;
			case 0x60:
        		result = 0x101u;
        		break;
			case 0x61:
        		result = 0x102u;
        		break;
			case 0x62:
        		result = 0x103u;
        		break;
			case 0x63:
        		result = 0x104u;
        		break;
			case 0x64:
        		result = 0x105u;
        		break;
			case 0x65:
        		result = 0x106u;
        		break;
			case 0x66:
        		result = 0x107u;
        		break;
			case 0x67:
        		result = 0x108u;
        		break;
			case 0x68:
        		result = 0x109u;
        		break;
			case 0x69:
        		result = 0x10Au;
        		break;
			case 0x6B:
        		result = 0x10Bu;
        		break;
			case 0x6D:
        		result = 0x10Cu;
        		break;
			case 0x6A:
        		result = 0x10Du;
        		break;
			case 0x6F:
        		result = 0x10Eu;
        		break;
			case 0x6E:
        		result = 0x10Fu;
        		break;
			case 0xBB:
        		result = 0x110u;
        		break;
			case 0xBD:
        		result = 0x111u;
        		break;
			case 0xDB:
        		result = 0x112u;
        		break;
			case 0xDD:
        		result = 0x113u;
        		break;
			case 0xDC:
        		result = 0x114u;
        		break;
			case 0xBA:
        		result = 0x115u;
        		break;
			case 0xDE:
        		result = 278;
        		break;
			case 0xBC:
        		result = 279;
        		break;
			case 0xBE:
        		result = 0x118u;
        		break;
			case 0xBF:
        		result = 0x119u;
        		break;
		}
		return result;
	}


	const uint32_t KEY_SHIFT          = 0x0;
	const uint32_t KEY_CONTROL		  = 0x1;
	const uint32_t KEY_ALT			  = 0x2;
	const uint32_t KEY_SPACE          = 0x20;

	const uint32_t KEY_0              = 0x30;
	const uint32_t KEY_1              = 0x31;
	const uint32_t KEY_2              = 0x32;
	const uint32_t KEY_3              = 0x33;
	const uint32_t KEY_4              = 0x34;
	const uint32_t KEY_5              = 0x35;
	const uint32_t KEY_6              = 0x36;
	const uint32_t KEY_7              = 0x37;
	const uint32_t KEY_8              = 0x38;
	const uint32_t KEY_9              = 0x39;

	const uint32_t KEY_A              = 0x41;
	const uint32_t KEY_B              = 0x42;
	const uint32_t KEY_C              = 0x43;
	const uint32_t KEY_D              = 0x44;
	const uint32_t KEY_E              = 0x45;
	const uint32_t KEY_F              = 0x46;
	const uint32_t KEY_G              = 0x47;
	const uint32_t KEY_H              = 0x48;
	const uint32_t KEY_I              = 0x49;
	const uint32_t KEY_J              = 0x4A;
	const uint32_t KEY_K              = 0x4B;
	const uint32_t KEY_L              = 0x4C;
	const uint32_t KEY_M              = 0x4D;
	const uint32_t KEY_N              = 0x4E;
	const uint32_t KEY_O              = 0x4F;
	const uint32_t KEY_P              = 0x50;
	const uint32_t KEY_Q              = 0x51;
	const uint32_t KEY_R              = 0x52;
	const uint32_t KEY_S              = 0x53;
	const uint32_t KEY_T              = 0x54;
	const uint32_t KEY_U              = 0x55;
	const uint32_t KEY_V              = 0x56;
	const uint32_t KEY_W              = 0x57;
	const uint32_t KEY_X              = 0x58;
	const uint32_t KEY_Y              = 0x59;
	const uint32_t KEY_Z              = 0x5A;


	const uint32_t KEY_TILDE          = 0x100;
	const uint32_t KEY_NUMPAD0        = 0x101;
	const uint32_t KEY_NUMPAD1        = 0x102;
	const uint32_t KEY_NUMPAD2        = 0x103;
	const uint32_t KEY_NUMPAD3        = 0x104;
	const uint32_t KEY_NUMPAD4        = 0x105;
	const uint32_t KEY_NUMPAD5        = 0x106;
	const uint32_t KEY_NUMPAD6        = 0x107;
	const uint32_t KEY_NUMPAD7        = 0x108;
	const uint32_t KEY_NUMPAD8        = 0x109;
	const uint32_t KEY_NUMPAD9        = 0x10A;
	const uint32_t KEY_ADD            = 0x10B;
	const uint32_t KEY_SUBTRACT       = 0x10C;
	const uint32_t KEY_MULTIPLY       = 0x10D;
	const uint32_t KEY_DIVIDE         = 0x10E;
	const uint32_t KEY_DECIMAL        = 0x10F;

	const uint32_t KEY_OEM_PLUS       = 0x110;   // = +
	const uint32_t KEY_OEM_MINUS      = 0x111;   // - _
	const uint32_t KEY_OEM_OBRACKE    = 0x112;   // [ {
	const uint32_t KEY_OEM_CBRACKE    = 0x113;   // ] }
	const uint32_t KEY_OEM_BACKSLASH  = 0x114;   // \ |
	const uint32_t KEY_OEM_SEMICOLON  = 0x115;   // ; :
	const uint32_t KEY_OEM_SQUOTMARKS = 0x116;   // ' "
	const uint32_t KEY_OEM_COMMA      = 0x117;   // , <
	const uint32_t KEY_OEM_PERIOD     = 0x118;   // . >
	const uint32_t KEY_OEM_SLASH      = 0x119;   // / ?

	const uint32_t KEY_ESC            = 0x200;
	const uint32_t KEY_ENTER          = 0x201;
	const uint32_t KEY_BACKSPACE      = 0x202;
	const uint32_t KEY_TAB            = 0x203;
	const uint32_t KEY_LEFT           = 0x204;
	const uint32_t KEY_UP             = 0x205;
	const uint32_t KEY_RIGHT          = 0x206;
	const uint32_t KEY_DOWN           = 0x207;
	const uint32_t KEY_INSERT         = 0x208;
	const uint32_t KEY_DELETE         = 0x209;
	const uint32_t KEY_HOME           = 0x20A;
	const uint32_t KEY_END            = 0x20B;
	const uint32_t KEY_PAGEUP         = 0x20C;
	const uint32_t KEY_PAGEDOWN       = 0x20D;
	const uint32_t KEY_CAPSLOCK       = 0x20E;
	const uint32_t KEY_NUMLOCK        = 0x20F;
	const uint32_t KEY_SCROLLLOCK     = 0x210;
	const uint32_t KEY_PAUSE          = 0x211;
	const uint32_t KEY_PRINTSCREEN    = 0x212;

	const uint32_t KEY_F1             = 0x300;
	const uint32_t KEY_F2             = 0x301;
	const uint32_t KEY_F3             = 0x302;
	const uint32_t KEY_F4             = 0x303;
	const uint32_t KEY_F5             = 0x304;
	const uint32_t KEY_F6             = 0x305;
	const uint32_t KEY_F7             = 0x306;
	const uint32_t KEY_F8             = 0x307;
	const uint32_t KEY_F9             = 0x308;
	const uint32_t KEY_F10            = 0x309;
	const uint32_t KEY_F11            = 0x30A;
	const uint32_t KEY_F12            = 0x30B;

	//×Ô¶¨Òå
	const uint32_t KEY_MOUSE_LEFT			= 0x1000;
	const uint32_t KEY_MOUSE_MIDDLE			= 0x1001;
	const uint32_t KEY_MOUSE_RIGHT			= 0x1002;
	const uint32_t KEY_MOUSE_X1				= 0x1003;
	const uint32_t KEY_MOUSE_X2				= 0x1004;
	const uint32_t KEY_MOUSE_SCROLL_UP		= 0x1005;
	const uint32_t KEY_MOUSE_SCROLL_DOWN	= 0x1006;

	int MouseCodeToKeyCode(int mouseCode);

	const char *getStr(uint32_t keyCode);
	
}

#endif