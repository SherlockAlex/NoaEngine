#include "InputSystem.h"

#include <SDL2/SDL.h>
#include <iostream>
#include "NoaEngine.h"

InputSystem inputSystem;

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
		SDL_PollEvent(&ioEvent);
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
		printf("key %c is pressed\n", key);
		SDL_PollEvent(&ioEvent);
		return true;
	}
#endif // _LINUX

#endif // _WIN64

	const Uint8* keyState = SDL_GetKeyboardState(nullptr);

	if (keyState[SDL_GetScancodeFromKey(key + 32)])
	{
		printf("key %c is pressed\n", key);
		SDL_PollEvent(&ioEvent);
		return true;
	}

	return false;

}