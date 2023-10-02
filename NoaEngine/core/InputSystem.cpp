#include "InputSystem.h"

#include <SDL2/SDL.h>
#include <iostream>
#include "NoaEngine.h"


using namespace std;

namespace noa {
	InputSystem inputSystem;
	bool InputSystem::mouseWheelEventReceived = false;
	Vector<double> InputSystem::scroll = { 0.0,0.0 };
}




#ifdef __linux

Display* display = nullptr;

#endif // __linux

void noa::InputSystem::Update()
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

noa::InputSystem::InputSystem()
{

	// 开启相对鼠标模式
#ifdef __linux

	display = XOpenDisplay(nullptr);

#endif // __linux

}

noa::InputSystem::~InputSystem()
{
#ifdef __linux
	XCloseDisplay(display);
#endif // __linux
}

bool noa::InputSystem::GetKeyHold(noa::KeyCode key) {
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
	char keys_return[32] = { 0 };
	XQueryKeymap(display, keys_return);
	return !!(keys_return[keyCode / 8] & (1 << (keyCode % 8)));

#endif // LINUX

#endif
	return false;
}

bool noa::InputSystem::GetKeyDown(noa::KeyCode key)
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

bool noa::InputSystem::GetMouseMoveState()
{
	return ioEvent.type == SDL_MOUSEMOTION;
}

void noa::InputSystem::SetRelativeMouseMode(bool mode)
{
	SDL_SetRelativeMouseMode(mode?SDL_TRUE:SDL_FALSE);

}

noa::Vector<double> delta;
noa::Vector<double>& noa::InputSystem::GetMouseMoveDelta()
{

	delta = mouseInput.delta;
	mouseInput.delta = { 0,0 };
	return delta;

}

noa::Vector<double>& noa::InputSystem::GetMousePosition()
{
	return mouseInput.position;
}


noa::Vector<double> wheel;
noa::Vector<double>& noa::InputSystem::GetMouseWheel()
{

	wheel = mouseInput.wheel;
	mouseInput.wheel = { 0,0 };
	return wheel;
}

bool noa::InputSystem::GetMouseButton(noa::MouseButton mouseButton)
{
	int mouseX, mouseY;
	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	return mouseState & SDL_BUTTON((static_cast<int>(mouseButton)));
}


