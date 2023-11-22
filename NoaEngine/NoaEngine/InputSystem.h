#ifndef NOAGAMEENGINE_INPUTSYSTEM_H
#define NOAGAMEENGINE_INPUTSYSTEM_H

#include "SDLHelper.h"
#include "NoaMath.h"
#include "Graphic.h"
#include "NoaAction.h"

#include <unordered_map>
#include <memory>

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
		KEY_ESC = 0x1B
#endif // _WIN64
	};

	typedef struct MouseKey
	{
		bool down = false;
		bool hold = false;
		bool up = false;
	}MouseKey;

	//鼠标输入事件上下文
	typedef struct MouseContext
	{
		Vector<double> position;
		Vector<double> delta;
		Vector<double> wheel;
		bool motion = false;
		std::unordered_map<noa::MouseButton, noa::MouseKey> mouseKey =
		{
			{noa::MouseButton::LEFT_BUTTON,{false,false,false}},
			{noa::MouseButton::MIDDLE_BUTTON,{false,false,false}},
			{noa::MouseButton::RIGHT_BUTTON,{false,false,false}},
		};

	}MouseContext;

	class InputEvent;
	class Input final
	{
	private:
		static std::shared_ptr<InputEvent> e;
	public:

		static void InitInputSystem(std::shared_ptr<InputEvent> e);

		static bool GetKeyHold(KeyCode key);
		static bool GetKeyDown(KeyCode key);
		static bool GetMouseMoveState();

		static bool GetMouseKeyDown(MouseButton mouseButton);
		static bool GetMouseKeyHold(MouseButton mouseButton);
		static bool GetMouseKeyUp(MouseButton mouseButton);

		static void SetRelativeMouseMode(bool mode);
		static Vector<double> & GetMouseMoveDelta();
		static Vector<double> & GetMousePosition();
		static Vector<double> & GetMouseWheel();

	};

	//目标是这样的，一个按钮绑定一个事件，这个事件的返回值可以是Vector<double>,也可以是bool,最后使用ReadValue<>()开发者便可以读取到数据
}



#endif // !NOAGAMEENGINE_INPUTSYSTEM_H