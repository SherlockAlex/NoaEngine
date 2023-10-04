#include "InputSystem.h"
#include "NoaEngine.h"
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
		mouseEvent.delta.x += static_cast<double>(e.motion.xrel);
		mouseEvent.delta.y += static_cast<double>(e.motion.yrel);
	}
	else {
		mouseEvent.delta = { 0,0 };
	}


	mouseEvent.motion = (e.type == SDL_MOUSEMOTION);

	if (e.type != SDL_MOUSEWHEEL)
	{
		mouseEvent.wheel = { 0,0 };
	}
	else {
		mouseEvent.wheel.x = e.wheel.x;
		mouseEvent.wheel.y = e.wheel.y;
	}


	mouseEvent.position.x = e.motion.x;
	mouseEvent.position.y = e.motion.y;



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
	SDL_SetRelativeMouseMode(mode?SDL_TRUE:SDL_FALSE);

}

noa::Vector<double> delta;
noa::Vector<double>& noa::InputSystem::GetMouseMoveDelta()
{

	delta = mouseEvent.delta;
	mouseEvent.delta = { 0,0 };
	return delta;

}

noa::Vector<double>& noa::InputSystem::GetMousePosition()
{
	return mouseEvent.position;
}


noa::Vector<double> wheel;
noa::Vector<double>& noa::InputSystem::GetMouseWheel()
{

	wheel = mouseEvent.wheel;
	mouseEvent.wheel = { 0,0 };
	return wheel;
}

bool noa::InputSystem::GetMouseButton(noa::MouseButton mouseButton)
{
	int mouseX, mouseY;
	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	return mouseState & SDL_BUTTON((static_cast<int>(mouseButton)));
}


