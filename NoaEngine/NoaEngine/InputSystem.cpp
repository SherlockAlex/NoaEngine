#include "InputSystem.h"
#include <iostream>

using namespace std;

namespace noa {
	InputSystem inputSystem;
}



#ifdef __linux

Display* display = nullptr;

#endif // __linux

void noa::InputSystem::Update()
{

	if (e.type == SDL_MOUSEMOTION)
	{
		mouseContext.delta.x += static_cast<double>(e.motion.xrel);
		mouseContext.delta.y += static_cast<double>(e.motion.yrel);
	}
	else {
		mouseContext.delta = { 0,0 };
	}

	mouseContext.motion = (e.type == SDL_MOUSEMOTION);

	if (e.type != SDL_MOUSEWHEEL)
	{
		mouseContext.wheel = { 0,0 };
	}
	else {
		mouseContext.wheel.x = e.wheel.x;
		mouseContext.wheel.y = e.wheel.y;
	}


	mouseContext.position.x = e.motion.x;
	mouseContext.position.y = e.motion.y;



}

noa::InputSystem::InputSystem()
{
	SDL_PollEvent(&e);
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
	return e.type == SDL_MOUSEMOTION;
}

void noa::InputSystem::SetRelativeMouseMode(bool mode)
{
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(mode));

}

noa::Vector<double> delta;
noa::Vector<double>& noa::InputSystem::GetMouseMoveDelta()
{

	delta = mouseContext.delta;
	mouseContext.delta = { 0,0 };
	return delta;

}

noa::Vector<double>& noa::InputSystem::GetMousePosition()
{
	return mouseContext.position;
}


noa::Vector<double> wheel;
noa::Vector<double>& noa::InputSystem::GetMouseWheel()
{

	wheel = mouseContext.wheel;
	mouseContext.wheel = { 0,0 };
	return wheel;
}

bool noa::InputSystem::GetMouseButton(noa::MouseButton mouseButton)
{
	int mouseX, mouseY;
	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	return mouseState & SDL_BUTTON((static_cast<int>(mouseButton)));
}


