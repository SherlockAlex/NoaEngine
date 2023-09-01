#ifndef NOAGAMEENGINE_INPUTSYSTEM_H
#define NOAGAMEENGINE_INPUTSYSTEM_H

#include <SDL2/SDL.h>
#include <functional>
//Windows System
#ifdef _WIN64
#include <Windows.h>
#endif

#ifdef _WIN32

#include <windows.h>

#endif // _WIN32

#include "NoaMath.h"
#include "NoaAction.h"

using namespace std;

namespace noa {
	extern SDL_Event ioEvent;

	enum MOUSEKEY {
		LeftButton = SDL_BUTTON_LEFT,
		MiddleButton = SDL_BUTTON_MIDDLE,
		RightButton = SDL_BUTTON_RIGHT
	};

	//����ӳ��
	enum KEYCODE
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

	class InputSystem
	{
	private:
		//����λ����Ϣ
		int mouseX = 0;
		int mouseY = 0;

	public:
		InputSystem();
		~InputSystem();
		NoaEvent<void()> inputEvent;
	public:

		//�������ioEvent
		bool GetKeyHold(char key);
		bool GetKeyDown(char key);

		//����ioEvent
		bool GetMouseMoveState();

		void Update();

		/*
		x > 0 ��������ƶ� : x
		x < 0 ��������ƶ� : -x
		y > 0 ��������ƶ� : y
		y < 0 ��������ƶ� : -y
		*/
		Vector<float> GetMouseMoveDelta();
		Vector<float> GetMousePosition();

		bool GetMouseButton(MOUSEKEY mouseButton);

	};

	extern InputSystem inputSystem;
}



#endif // !NOAGAMEENGINE_INPUTSYSTEM_H