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
		KeyU = 'V',
		KeyW = 'W',
		KeyX = 'X',
		KeyY = 'Y',
		KeyZ = 'Z',
		KeySpace = ' '
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

		bool GetMouseButton(MOUSEKEY mouseButton);

	};

	extern InputSystem inputSystem;
}



#endif // !NOAGAMEENGINE_INPUTSYSTEM_H