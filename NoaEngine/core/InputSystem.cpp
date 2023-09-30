#include "InputSystem.h"

#include <SDL2/SDL.h>
#include <iostream>
#include "NoaEngine.h"


using namespace std;

namespace noa {
	InputSystem inputSystem;
	SDL_Event ioEvent;

	bool InputSystem::mouseWheelEventReceived = false;
	Vector<double> InputSystem::scroll = { 0.0,0.0 };


#ifdef __linux

	Display* display = nullptr;

#endif // __linux


	void InputSystem::SetGLWindow(GLFWwindow* window)
	{
		this->window = window;
	}

	void InputSystem::SetGraphicAPI(GRAPHIC api)
	{
		this->graphicAPI = api;
	}

	void InputSystem::Update()
	{

		if (graphicAPI == GRAPHIC::SDL)
		{
			if (ioEvent.type == SDL_MOUSEMOTION)
			{
				mouseInput.delta.x += static_cast<double>(ioEvent.motion.xrel);
				mouseInput.delta.y += static_cast<double>(ioEvent.motion.yrel);
			}
			else {
				mouseInput.delta = { 0,0 };
			}


			mouseInput.motion = (ioEvent.type == SDL_MOUSEMOTION);

			if (ioEvent.type != SDL_MOUSEWHEEL)
			{
				mouseInput.wheel = { 0,0 };
			}
			else {
				mouseInput.wheel.x = ioEvent.wheel.x;
				mouseInput.wheel.y = ioEvent.wheel.y;
			}


			mouseInput.position.x = ioEvent.motion.x;
			mouseInput.position.y = ioEvent.motion.y;

		}
		else if(graphicAPI == GRAPHIC::OPENGL)
		{

			// 获取鼠标相对位移
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			mouseInput.delta.x += (xpos - mouseX);
			mouseInput.delta.y += (ypos - mouseY);
			
			mouseX = xpos;
			mouseY = ypos;

			// 获取鼠标位置
			mouseInput.position.x = xpos;
			mouseInput.position.y = ypos;

			// 鼠标滚轮
			if (!inputSystem.mouseWheelEventReceived)
			{
				mouseInput.wheel = { 0,0 };
			}
			else {
				mouseInput.wheel.x += scroll.x;
				mouseInput.wheel.y += scroll.y;
				inputSystem.mouseWheelEventReceived = false;
			}

			

		}

		

	}

	InputSystem::InputSystem()
	{

		// 开启相对鼠标模式
#ifdef __linux

		display = XOpenDisplay(nullptr);

#endif // __linux

	}

	InputSystem::~InputSystem()
	{
#ifdef __linux
		XCloseDisplay(display);
#endif // __linux
	}

	bool InputSystem::GetKeyHold(KEYCODE key) {
		//监听按键是否按住

#ifdef _WIN64
		if (GetAsyncKeyState((unsigned short)key) & 0x8000) {
			return true;
		}

#else 
#ifdef _WIN32
		if (GetAsyncKeyState((unsigned short)key) & 0x8000) {
			return true;
		}
#endif // _WIN32

#ifdef __linux__

		KeyCode keyCode = XKeysymToKeycode(display, (key));
		char keys_return[32] = {0};
		XQueryKeymap(display, keys_return);
		return !!(keys_return[keyCode / 8] & (1 << (keyCode % 8)));
		
#endif // LINUX

#endif
		return false;
	}

	bool InputSystem::GetKeyDown(KEYCODE key)
	{
		//检测按键按下瞬间
#ifdef _WIN64
		if (GetAsyncKeyState((unsigned short)key) & 1) {
			return true;
		}

#else
#ifdef _WIN32
		if (GetAsyncKeyState((unsigned short)key) & 1) {
			return true;
		}
#endif // _WIN32
#ifdef __linux__
		KeyCode keyCode = XKeysymToKeycode(display, (key));
		char keys_return[32] = { 0 };
		XQueryKeymap(display, keys_return);
		return !!(keys_return[keyCode / 8] & (1 << (keyCode % 8)));
		
#endif // _LINUX

#endif // _WIN64

		return false;

	}

	bool InputSystem::GetMouseMoveState()
	{
		if (graphicAPI == GRAPHIC::OPENGL)
		{
			return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
		}
		
		return ioEvent.type == SDL_MOUSEMOTION;
	}

	void InputSystem::SetRelativeMouseMode(bool mode)
	{
		if (graphicAPI == GRAPHIC::OPENGL)
		{
			glfwSetInputMode(
			window, GLFW_CURSOR, mode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
			glfwGetCursorPos(window, &mouseX, &mouseY); // 初始化鼠标位置
			return;
		}
		else if (graphicAPI == GRAPHIC::SDL) 
		{
			if (mode) {
				SDL_SetRelativeMouseMode(SDL_TRUE);
			}
			else {
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		}
		
		

	}

	Vector<double> delta;
	Vector<double> & InputSystem::GetMouseMoveDelta()
	{

		delta = mouseInput.delta;
		mouseInput.delta = { 0,0 };
		return delta;
		
	}

	Vector<double> & InputSystem::GetMousePosition()
	{
		return mouseInput.position;
	}


	Vector<double> wheel;
	Vector<double> & InputSystem::GetMouseWheel()
	{

		wheel = mouseInput.wheel;
		mouseInput.wheel = { 0,0 };
		return wheel;
	}

	bool InputSystem::GetMouseButton(MOUSEKEY mouseButton)
	{
		if (graphicAPI == GRAPHIC::OPENGL)
		{
			int state = glfwGetMouseButton(window, static_cast<int>(mouseButton));
			return state == GLFW_PRESS;
		}
		else if (graphicAPI == GRAPHIC::SDL)
		{
			int mouseX, mouseY;
			const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
			return mouseState & SDL_BUTTON((static_cast<int>(mouseButton) + 1));
		}
		
	}

	void InputSystem::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		scroll.x = xoffset;
		scroll.y = yoffset;
		mouseWheelEventReceived = true;
	}


}

