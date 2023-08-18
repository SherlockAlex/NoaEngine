#ifndef NOAGAMEENGINE_INPUTSYSTEM_H
#define NOAGAMEENGINE_INPUTSYSTEM_H

#include <SDL2/SDL.h>
#include <functional>
//Windows System
#ifdef _WIN64
#include <Windows.h>
#endif

#include "NoaMath.h"

using namespace std;

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
};

class InputSystem
{
private:
	//����λ����Ϣ
	int mouseX = 0;
	int mouseY = 0;

public:
	InputSystem();

public:

	//�������ioEvent
	bool GetKeyHold(char key);
	bool GetKeyDown(char key);

	//����ioEvent
	bool GetMouseMoveState();
	
	void Update();

	void BindEvent(void (*eventFunc)(void));
	void BindEvent(function<void()> eventFunc);

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

#endif // !NOAGAMEENGINE_INPUTSYSTEM_H