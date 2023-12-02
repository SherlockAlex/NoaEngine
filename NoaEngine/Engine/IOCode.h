#ifndef NOAENGINE_IOCODE_H
#define NOAENGINE_IOCODE_H

#include "Core.h"

namespace noa {
	enum class NOA_API MouseButton {
		LEFT_BUTTON = 1,
		MIDDLE_BUTTON = 2,
		RIGHT_BUTTON = 3,
		// 添加其他鼠标按钮
	};

	//按键映射
	enum class NOA_API KeyCode
	{
		KEY_0 = 48,
		KEY_1 = 49,
		KEY_2 = 50,
		KEY_3 = 51,
		KEY_4 = 52,
		KEY_5 = 53,
		KEY_6 = 54,
		KEY_7 = 55,
		KEY_8 = 56,
		KEY_9 = 57,

		KEY_A = 'a',
		KEY_B = 'b',
		KEY_C = 'c',
		KEY_D = 'd',
		KEY_E = 'e',
		KEY_F = 'f',
		KEY_G = 'g',
		KEY_H = 'h',
		KEY_I = 'i',
		KEY_J = 'j',
		KEY_K = 'k',
		KEY_L = 'l',
		KEY_M = 'm',
		KEY_N = 'n',
		KEY_O = 'o',
		KEY_P = 'p',
		KEY_Q = 'q',
		KEY_R = 'r',
		KEY_S = 's',
		KEY_T = 't',
		KEY_U = 'u',
		KEY_V = 'v',
		KEY_W = 'w',
		KEY_X = 'x',
		KEY_Y = 'y',
		KEY_Z = 'z',

		KEY_SPACE = 32,
		KEY_ESC = 27,
		KEY_TAB = 9,
		KEY_CAPSLOCK = 1073741881,

		KEY_LCTRL = 1073742048,
		KEY_RCTRL = 1073742052,

		KEY_ALT = 1073741977,

		KEY_DEL = 127,

		KEY_F1 = 1073741882,
		KEY_F2 = 1073741883,
		KEY_F3 = 1073741884,
		KEY_F4 = 1073741885,
		KEY_F5 = 1078741886,
		KEY_F6 = 1073741887,
		KEY_F7 = 1073741888,
		KEY_F8 = 1073741889,
		KEY_F9 = 1073741890,
		KEY_F10 = 1073741891,
		KEY_F11 = 1073741892,
		KEY_F12 = 1073741893,

		KEY_LSHIFT = 1073742049,
		KEY_RSHIFT = 1073742053,

		KEY_COMMA = 44,

		KEY_UP = 1073741906,
		KEY_DOWN = 1073741905,
		KEY_LEFT = 1073741904,
		KEY_RIGHT = 1073741903

	};
}

#endif // !NOAENGINE_IOCODE_H

