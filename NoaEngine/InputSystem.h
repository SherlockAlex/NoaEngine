#ifndef NOAGAMEENGINE_INPUTSYSTEM_H
#define NOAGAMEENGINE_INPUTSYSTEM_H

//Windows System
#ifdef _WIN64
#include <Windows.h>
#endif

//°´¼üÓ³Éä
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
public:
	bool GetKeyHold(char key);
	bool GetKeyDown(char key);
};

extern InputSystem inputSystem;

#endif // !NOAGAMEENGINE_INPUTSYSTEM_H