#include "InputSystem.h"
#include <iostream>

using namespace std;

namespace noa {
	InputSystem inputSystem;
}

#ifdef __linux

Display* display = nullptr;

#endif // __linux

void noa::InputSystem::Update(SDL_Event& e)
{

	mouseContext.mouseKey[static_cast<noa::MouseButton>(e.button.button)].down = (e.type == SDL_MOUSEBUTTONDOWN);
	mouseContext.mouseKey[static_cast<noa::MouseButton>(e.button.button)].up = (e.type == SDL_MOUSEBUTTONUP);
	{
		int mouseX, mouseY;
		const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

		mouseContext.mouseKey[MouseButton::LEFT_BUTTON].hold = mouseState & SDL_BUTTON((static_cast<int>(MouseButton::LEFT_BUTTON)));
		mouseContext.mouseKey[MouseButton::RIGHT_BUTTON].hold = mouseState & SDL_BUTTON((static_cast<int>(MouseButton::RIGHT_BUTTON)));
		mouseContext.mouseKey[MouseButton::MIDDLE_BUTTON].hold = mouseState & SDL_BUTTON((static_cast<int>(MouseButton::MIDDLE_BUTTON)));
	}
	

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

#if defined(_WIN64) || defined(_WIN32)
	if (GetAsyncKeyState((unsigned short)key) & 0x8000) {
		return true;
	}

#else 

#ifdef __linux__

	KeyCode keyCode = XKeysymToKeycode(display, (key));
	char keys_return[32] = { 0 };
	XQueryKeymap(display, keys_return);
	return !!(keys_return[keyCode / 8] & (1 << (keyCode % 8)));

#endif // LINUX

#endif // _WIN64 || _WIN32
	return false;
}

bool noa::InputSystem::GetKeyDown(noa::KeyCode key)
{
#if defined(_WIN64) || defined(_WIN32)
	if (GetAsyncKeyState((unsigned short)key) & 1) {
		return true;
	}

#else
#ifdef __linux__
	KeyCode keyCode = XKeysymToKeycode(display, (key));
	char keys_return[32] = { 0 };
	XQueryKeymap(display, keys_return);
	return !!(keys_return[keyCode / 8] & (1 << (keyCode % 8)));

#endif // _LINUX

#endif // _WIN64 || _WIN32

	return false;

}

bool noa::InputSystem::GetMouseMoveState()
{
	return this->mouseContext.motion;
}

void noa::InputSystem::SetRelativeMouseMode(bool mode)
{
	//设置鼠标相对模式
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

bool noa::InputSystem::GetMouseKeyDown(MouseButton mouseButton)
{
	return this->mouseContext.mouseKey[mouseButton].down;
}

bool noa::InputSystem::GetMouseKeyHold(noa::MouseButton mouseButton)
{
	return this->mouseContext.mouseKey[mouseButton].hold;
}

bool noa::InputSystem::GetMouseKeyUp(MouseButton mouseButton)
{
	return this->mouseContext.mouseKey[mouseButton].up;
}


