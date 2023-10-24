#ifndef NOAGAMEENGINE_INPUTSYSTEM_H
#define NOAGAMEENGINE_INPUTSYSTEM_H

#include "SDLHelper.h"
#include "NoaMath.h"
#include "Graphic.h"

//Windows System
#ifdef _WIN64
#include <Windows.h>
#endif

#ifdef _WIN32

#include <windows.h>

#endif // _WIN32

#ifdef __linux
#include <X11/Xlib.h>
#include <X11/keysym.h>
#endif // __linux



namespace noa {
	enum class MouseButton {
		LEFT_BUTTON = 1,
		MIDDLE_BUTTON = 2,
		RIGHT_BUTTON = 3,
		// 添加其他鼠标按钮
	};

	//按键映射
	enum class KeyCode
	{
#ifdef __linux
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
		KEY_SPACE = ' ',
		KEY_ESC = XK_Escape
#endif // __linux
#ifdef _WIN64
		KEY_A = 'A',
		KEY_B = 'B',
		KEY_C = 'C',
		KEY_D = 'D',
		KEY_E = 'E',
		KEY_F = 'F',
		KEY_G = 'G',
		KEY_H = 'H',
		KEY_I = 'I',
		KEY_J = 'J',
		KEY_K = 'K',
		KEY_L = 'L',
		KEY_M = 'M',
		KEY_N = 'N',
		KEY_O = 'O',
		KEY_P = 'P',
		KEY_Q = 'Q',
		KEY_R = 'R',
		KEY_S = 'S',
		KEY_T = 'T',
		KEY_U = 'U',
		KEY_V = 'V',
		KEY_W = 'W',
		KEY_X = 'X',
		KEY_Y = 'Y',
		KEY_Z = 'Z',
		KEY_SPACE = ' ',
		KEY_ESC = VK_ESCAPE
#endif // _WIN64
	};

	typedef struct MouseContext
	{
		Vector<double> position;
		Vector<double> delta;
		Vector<double> wheel;
		bool motion = false;

	}MouseContext;

	class InputSystem final
	{
	public:
		SDL_Event e = {};
	private:
		MouseContext mouseContext;
	public:
		void Update();
		InputSystem();
		~InputSystem();
	public:

		bool GetKeyHold(KeyCode key);
		bool GetKeyDown(KeyCode key);
		bool GetMouseMoveState();

		void SetRelativeMouseMode(bool mode);
		Vector<double> & GetMouseMoveDelta();
		Vector<double> & GetMousePosition();
		Vector<double> & GetMouseWheel();

		bool GetMouseButton(MouseButton mouseButton);

	};

	//目标是这样的，一个按钮绑定一个事件，这个事件的返回值可以是Vector<double>,也可以是bool,最后使用ReadValue<>()开发者便可以读取到数据

	extern InputSystem inputSystem;
}



#endif // !NOAGAMEENGINE_INPUTSYSTEM_H