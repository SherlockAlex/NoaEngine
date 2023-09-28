#ifndef NOAGAMEENGINE_INPUTSYSTEM_H
#define NOAGAMEENGINE_INPUTSYSTEM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <SDL2/SDL.h>
#include <functional>
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

#include "NoaMath.h"

namespace noa {
	extern SDL_Event ioEvent;

	enum class MOUSEKEY {
		LeftButton = GLFW_MOUSE_BUTTON_LEFT,
		MiddleButton = GLFW_MOUSE_BUTTON_RIGHT,
		RightButton = GLFW_MOUSE_BUTTON_MIDDLE,
		// 添加其他鼠标按钮
	};

	enum class GRAPHIC
	{
		SDL = 0,
		OpenGL = 1
	};

	//按键映射
	enum class KEYCODE
	{
#ifdef __linux
		KeyA = 'a',
		KeyB = 'b',
		KeyC = 'c',
		KeyD = 'd',
		KeyE = 'e',
		KeyF = 'f',
		KeyG = 'g',
		KeyH = 'h',
		KeyI = 'i',
		KeyJ = 'j',
		KeyK = 'k',
		KeyL = 'l',
		KeyM = 'm',
		KeyN = 'n',
		KeyO = 'o',
		KeyP = 'p',
		KeyQ = 'q',
		KeyR = 'r',
		KeyS = 's',
		KeyT = 't',
		KeyU = 'u',
		KeyV = 'v',
		KeyW = 'w',
		KeyX = 'x',
		KeyY = 'y',
		KeyZ = 'z',
		KeySpace = ' ',
		KeyESC = XK_Escape
#endif // __linux
#ifdef _WIN64
		KeyA = 'A',
		KeyB = 'B',
		KeyC = 'C',
		KeyD = 'D',
		KeyE = 'E',
		KeyF = 'F',
		KeyG = 'G',
		KeyH = 'H',
		KeyI = 'I',
		KeyJ = 'J',
		KeyK = 'K',
		KeyL = 'L',
		KeyM = 'M',
		KeyN = 'N',
		KeyO = 'O',
		KeyP = 'P',
		KeyQ = 'Q',
		KeyR = 'R',
		KeyS = 'S',
		KeyT = 'T',
		KeyU = 'U',
		KeyV = 'V',
		KeyW = 'W',
		KeyX = 'X',
		KeyY = 'Y',
		KeyZ = 'Z',
		KeySpace = ' ',
		KeyESC = VK_ESCAPE
#endif // _WIN64
	};

	typedef struct MouseInput {
		Vector<double> position;
		Vector<double> delta;
		Vector<double> wheel;

		bool motion = false;

	}MouseInput;

	class InputSystem final
	{

	private:

		MouseInput mouseInput;

		//鼠标的位置信息
		double mouseX = 0;
		double mouseY = 0;

		GRAPHIC graphicAPI;

		GLFWwindow* window = nullptr;

	public:
		void Update();

		void SetGraphicAPI(GRAPHIC api);
		void SetGLWindow(GLFWwindow* window);
		InputSystem();
		~InputSystem();
	public:

		bool GetKeyHold(KEYCODE key);
		bool GetKeyDown(KEYCODE key);
		bool GetMouseMoveState();

		/*
		x > 0 鼠标向右移动 : x
		x < 0 鼠标向左移动 : -x
		y > 0 鼠标向下移动 : y
		y < 0 鼠标向上移动 : -y
		*/
		void SetRelativeMouseMode(bool mode);
		Vector<double> & GetMouseMoveDelta();
		Vector<double> & GetMousePosition();
		Vector<double> & GetMouseWheel();

		bool GetMouseButton(MOUSEKEY mouseButton);

	public:
		static Vector<double> scroll;
		static bool mouseWheelEventReceived;

		static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	};

	extern InputSystem inputSystem;
}



#endif // !NOAGAMEENGINE_INPUTSYSTEM_H