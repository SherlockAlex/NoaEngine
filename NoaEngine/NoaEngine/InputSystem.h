#ifndef NOAGAMEENGINE_INPUTSYSTEM_H
#define NOAGAMEENGINE_INPUTSYSTEM_H

#include "SDLHelper.h"
#include "NoaMath.h"
#include "Graphic.h"
#include "NoaAction.h"

#include <unordered_map>
#include <memory>

namespace noa 
{
	enum class MouseButton {
		LEFT_BUTTON = 1,
		MIDDLE_BUTTON = 2,
		RIGHT_BUTTON = 3,
		// 添加其他鼠标按钮
	};

	//按键映射
	enum class KeyCode
	{
		KEY_0 = SDLK_0,
		KEY_1 = SDLK_1,
		KEY_2 = SDLK_2,
		KEY_3 = SDLK_3,
		KEY_4 = SDLK_4,
		KEY_5 = SDLK_5,
		KEY_6 = SDLK_6,
		KEY_7 = SDLK_7,
		KEY_8 = SDLK_8,
		KEY_9 = SDLK_9,

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
		KEY_CAPSLOCK = SDLK_CAPSLOCK,
		
		KEY_LCTRL = SDLK_LCTRL,
		KEY_RCTRL = SDLK_RCTRL,

		KEY_ALT = SDLK_ALTERASE,

		KEY_DEL = SDLK_DELETE,

		KEY_F1 = SDLK_F1,
		KEY_F2 = SDLK_F2,
		KEY_F3 = SDLK_F3,
		KEY_F4 = SDLK_F4,
		KEY_F5 = SDLK_F5,
		KEY_F6 = SDLK_F6,
		KEY_F7 = SDLK_F7,
		KEY_F8 = SDLK_F8,
		KEY_F9 = SDLK_F9,
		KEY_F10 = SDLK_F10,
		KEY_F11 = SDLK_F11,
		KEY_F12 = SDLK_F12,
		
		KEY_LSHIFT = SDLK_LSHIFT,
		KEY_RSHIFT = SDLK_RSHIFT,

		KEY_COMMA = SDLK_COMMA,

		KEY_UP = SDLK_UP,
		KEY_DOWN = SDLK_DOWN,
		KEY_LEFT = SDLK_LEFT,
		KEY_RIGHT = SDLK_RIGHT
		
	};

	typedef struct KeyState
	{
		bool performed = false;
		bool wasPerformedOnce = false;
		bool canceled = false;
	}KeyState;

	typedef struct KeyboardContext {
		std::unordered_map<noa::KeyCode, noa::KeyState> keyMap;
		KeyboardContext();
	}KeyboardContext;

	//鼠标输入事件上下文
	typedef struct MouseContext
	{
		Vector<double> position;
		Vector<double> delta;
		Vector<double> wheel;
		bool motion = false;

		std::unordered_map<noa::MouseButton, noa::KeyState> mouseKey;
		MouseContext();

	}MouseContext;

	class InputEvent;
	class Input final
	{
	private:
		static std::shared_ptr<InputEvent> e;
	public:

		static void InitInputSystem(std::shared_ptr<InputEvent> e);

		static bool GetKeyDown(KeyCode key);
		static bool GetKeyDownOnce(KeyCode key);
		static bool GetKeyUp(KeyCode key);

		static bool GetMouseMoveState();
		static bool GetMouseKeyDown(MouseButton mouseButton);
		static bool GetMouseKeyDownOnce(MouseButton mouseButton);
		static bool GetMouseKeyUp(MouseButton mouseButton);

		static void SetRelativeMouseMode(bool mode);
		static Vector<double> & GetMouseMoveDelta();
		static Vector<double> & GetMousePosition();
		static Vector<double> & GetMouseWheel();

	};

	//目标是这样的，一个按钮绑定一个事件，这个事件的返回值可以是Vector<double>,也可以是bool,最后使用ReadValue<>()开发者便可以读取到数据
}



#endif // !NOAGAMEENGINE_INPUTSYSTEM_H