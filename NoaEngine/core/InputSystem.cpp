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

	InputSystem::InputSystem()
	{

		

		// ����������ģʽ
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

	bool InputSystem::GetKeyHold(char key) {
		//���������Ƿ�ס

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

	bool InputSystem::GetKeyDown(char key)
	{
		//��ⰴ������˲��
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
		return glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
		//return ioEvent.type == SDL_MOUSEMOTION;
	}

	void InputSystem::Update()
	{
		//ʵ�ָ���
		inputEvent.Invoke();
	}

	void InputSystem::SetRelativeMouseMode(bool mode)
	{
		glfwSetInputMode(
			window, GLFW_CURSOR, mode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
	}

	Vector<float> InputSystem::GetMouseMoveDelta()
	{

		Vector<float> delta;

		// ��ȡ������λ��
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		delta.x = static_cast<float>(xpos) - mouseX;
		delta.y = static_cast<float>(ypos) - mouseY;

		mouseX = static_cast<float>(xpos);
		mouseY = static_cast<float>(ypos);

		return delta;

		/*Vector<float> delta;

		delta.x = ioEvent.motion.xrel;
		delta.y = ioEvent.motion.yrel;

		return delta;*/
	}

	Vector<float> InputSystem::GetMousePosition()
	{
		Vector<float> delta;

		// ��ȡ���λ��
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		delta.x = static_cast<float>(xpos);
		delta.y = static_cast<float>(ypos);

		return delta;

		/*Vector<float> delta;

		delta.x = ioEvent.motion.x;
		delta.y = ioEvent.motion.y;

		return delta;*/
	}



	Vector<int> InputSystem::GetMouseWheel()
	{

		if (!InputSystem::mouseWheelEventReceived) 
		{
			return { 0,0 };
		}

		Vector<int> result = { 0, 0 };

		//glfwGetScrollValues(window, &scrollX, &scrollY);

		result.x = static_cast<int>(scroll.x);
		result.y = static_cast<int>(scroll.y);

		return result;

		/*if (ioEvent.type != SDL_MOUSEWHEEL)
		{
			return { 0,0 };
		}
		Vector<int> result;
		result.x = ioEvent.wheel.x;
		result.y = ioEvent.wheel.y;
		return result;*/
	}

	bool InputSystem::GetMouseButton(MOUSEKEY mouseButton)
	{
		int state = glfwGetMouseButton(window, static_cast<int>(mouseButton));
		return state == GLFW_PRESS;

		/*int mouseX, mouseY;
		const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		return mouseState & SDL_BUTTON(mouseButton);*/
	}

	void InputSystem::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		scroll.x = xoffset;
		scroll.y = yoffset;
		mouseWheelEventReceived = true;
	}


}

