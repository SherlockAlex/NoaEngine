#include "InputSystem.h"

#include <SDL2/SDL.h>
#include <iostream>
#include "NoaEngine.h"

#ifdef __linux
#include <X11/Xlib.h>
#include <X11/keysym.h>
#endif // __linux


namespace noa {
	InputSystem inputSystem;
	SDL_Event ioEvent;

#ifdef __linux

	Display* display = nullptr;

#endif // __linux


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
		return ioEvent.type == SDL_MOUSEMOTION;
	}

	void InputSystem::Update()
	{
		//ʵ�ָ���
		inputEvent.Invoke();
	}

	Vector<float> InputSystem::GetMouseMoveDelta()
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);

		Vector<float> delta;

		delta.x = ioEvent.motion.xrel;
		delta.y = ioEvent.motion.yrel;

		return delta;
	}

	Vector<float> InputSystem::GetMousePosition()
	{
		Vector<float> delta;

		delta.x = ioEvent.motion.x;
		delta.y = ioEvent.motion.y;

		return delta;
	}

	bool InputSystem::GetMouseButton(MOUSEKEY mouseButton)
	{
		int mouseX, mouseY;
		const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		return mouseState & SDL_BUTTON(mouseButton);
	}


}

