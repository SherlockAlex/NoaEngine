#include "InputSystem.h"

#include <SDL2/SDL.h>
#include <iostream>
#include "NoaEngine.h"

InputSystem inputSystem;

SDL_Event ioEvent;

InputSystem::InputSystem()
{
	// ����������ģʽ
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

bool InputSystem::GetKeyHold(char key) {
	//���������Ƿ�ס

#ifdef _WIN64
	if (GetAsyncKeyState((unsigned short)key) & 0x8000) {
		return true;
	}

#else 
#ifdef __linux__

	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	if (keyState[SDL_GetScancodeFromKey(key + 32)])
	{
		//SDL_PollEvent(&ioEvent);
		printf("key %c is hold on\n", key);
		return true;
	}
#endif // LINUX

#endif
	return false;
}

bool InputSystem::GetKeyDown(char key)
{
	//��ⰴ������˲��
#ifdef _WIN64
	if (GetAsyncKeyState((unsigned short)key) & 0x0001) {
		return true;
	}

#else
#ifdef __linux__
	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	if (keyState[SDL_GetScancodeFromKey(key + 32)])
	{
		//SDL_PollEvent(&ioEvent);
		return true;
	}
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
}

Vector<float> InputSystem::GetMouseMoveDelta()
{
	Vector<float> delta;

	delta.x = ioEvent.motion.xrel;
	delta.y = ioEvent.motion.yrel;

	return delta;
}

bool InputSystem::GetMouseButton(MOUSEKEY mouseButton)
{
	int mouseX, mouseY;
	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	return mouseState&SDL_BUTTON(mouseButton);
}
