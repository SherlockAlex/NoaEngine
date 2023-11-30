#include "InputSystem.h"
#include "Platform.h"
#include "Debug.h"

#include <iostream>

using namespace std;

std::shared_ptr<noa::InputEvent> noa::Input::e = nullptr;

noa::KeyboardContext::KeyboardContext() 
{
	this->keyMap[KeyCode::KEY_0] = { false,false,false };
	this->keyMap[KeyCode::KEY_1] = { false,false,false };
	this->keyMap[KeyCode::KEY_2] = { false,false,false };
	this->keyMap[KeyCode::KEY_3] = { false,false,false };
	this->keyMap[KeyCode::KEY_4] = { false,false,false };
	this->keyMap[KeyCode::KEY_5] = { false,false,false };
	this->keyMap[KeyCode::KEY_6] = { false,false,false };
	this->keyMap[KeyCode::KEY_7] = { false,false,false };
	this->keyMap[KeyCode::KEY_8] = { false,false,false };
	this->keyMap[KeyCode::KEY_9] = { false,false,false };

	this->keyMap[KeyCode::KEY_A] = { false,false,false};
	this->keyMap[KeyCode::KEY_B] = { false,false,false};
	this->keyMap[KeyCode::KEY_C] = { false,false,false};
	this->keyMap[KeyCode::KEY_D] = { false,false,false};
	this->keyMap[KeyCode::KEY_E] = { false,false,false};
	this->keyMap[KeyCode::KEY_F] = { false,false,false};
	this->keyMap[KeyCode::KEY_G] = { false,false,false};
	this->keyMap[KeyCode::KEY_H] = { false,false,false};
	this->keyMap[KeyCode::KEY_I] = { false,false,false};
	this->keyMap[KeyCode::KEY_J] = { false,false,false};
	this->keyMap[KeyCode::KEY_K] = { false,false,false};
	this->keyMap[KeyCode::KEY_L] = { false,false,false};
	this->keyMap[KeyCode::KEY_M] = { false,false,false};
	this->keyMap[KeyCode::KEY_N] = { false,false,false};
	this->keyMap[KeyCode::KEY_O] = { false,false,false};
	this->keyMap[KeyCode::KEY_P] = { false,false,false};
	this->keyMap[KeyCode::KEY_Q] = { false,false,false};
	this->keyMap[KeyCode::KEY_R] = { false,false,false};
	this->keyMap[KeyCode::KEY_S] = { false,false,false};
	this->keyMap[KeyCode::KEY_T] = { false,false,false};
	this->keyMap[KeyCode::KEY_U] = { false,false,false};
	this->keyMap[KeyCode::KEY_V] = { false,false,false};
	this->keyMap[KeyCode::KEY_W] = { false,false,false};
	this->keyMap[KeyCode::KEY_X] = { false,false,false};
	this->keyMap[KeyCode::KEY_Y] = { false,false,false};
	this->keyMap[KeyCode::KEY_Z] = { false,false,false};

	this->keyMap[KeyCode::KEY_SPACE] = { false,false,false };
	
	this->keyMap[KeyCode::KEY_ESC] =		{ false,false,false };
	this->keyMap[KeyCode::KEY_TAB] =		{ false,false,false };
	this->keyMap[KeyCode::KEY_CAPSLOCK] =	{ false,false,false };

	this->keyMap[KeyCode::KEY_LSHIFT] =	{ false,false,false };
	this->keyMap[KeyCode::KEY_RSHIFT] =	{ false,false,false };

	this->keyMap[KeyCode::KEY_ALT] = { false,false,false };
	this->keyMap[KeyCode::KEY_DEL] = { false,false,false };

	this->keyMap[KeyCode::KEY_F1] = { false,false,false };
	this->keyMap[KeyCode::KEY_F2] = { false,false,false };
	this->keyMap[KeyCode::KEY_F3] = { false,false,false };
	this->keyMap[KeyCode::KEY_F4] = { false,false,false };
	this->keyMap[KeyCode::KEY_F5] = { false,false,false };
	this->keyMap[KeyCode::KEY_F6] = { false,false,false };
	this->keyMap[KeyCode::KEY_F7] = { false,false,false };
	this->keyMap[KeyCode::KEY_F8] = { false,false,false };
	this->keyMap[KeyCode::KEY_F9] = { false,false,false };
	this->keyMap[KeyCode::KEY_F10] = { false,false,false };
	this->keyMap[KeyCode::KEY_F11] = { false,false,false };
	this->keyMap[KeyCode::KEY_F12] = { false,false,false };

	this->keyMap[KeyCode::KEY_LSHIFT] = { false,false,false };
	this->keyMap[KeyCode::KEY_RSHIFT] = { false,false,false };

	this->keyMap[KeyCode::KEY_COMMA] = { false,false,false };

	this->keyMap[KeyCode::KEY_UP] = { false,false,false };
	this->keyMap[KeyCode::KEY_DOWN] = { false,false,false };
	this->keyMap[KeyCode::KEY_LEFT] = { false,false,false };
	this->keyMap[KeyCode::KEY_RIGHT] = { false,false,false };
	
}

noa::MouseContext::MouseContext() 
{
	this->mouseKey[noa::MouseButton::LEFT_BUTTON] = { false,false,false };
	this->mouseKey[noa::MouseButton::MIDDLE_BUTTON] = { false,false,false };
	this->mouseKey[noa::MouseButton::RIGHT_BUTTON] = { false,false,false };
}

void noa::Input::InitInputSystem(std::shared_ptr<noa::InputEvent> e)
{
	//��ʼ��InputSystem
	if (e == nullptr)
	{
		Debug::Error("Init Input subsystem error");
		Debug::Error("This InputEvent is null");
		exit(-1);
	}
	Input::e = e;

}

bool noa::Input::GetKeyDown(noa::KeyCode key)
{
	return e->GetKeyDown(key);
}

bool noa::Input::GetKeyDownOnce(noa::KeyCode key)
{
	return e->GetKeyDownOnce(key);
}

bool noa::Input::GetKeyUp(noa::KeyCode key) 
{
	return e->GetKeyUp(key);
}

bool noa::Input::GetMouseMoveState()
{
	return e->mouseContext->motion;
}

void noa::Input::SetRelativeMouseMode(bool mode)
{
	//����������ģʽ
	e->SetRelativeMouseMode(mode);
}

noa::Vector<double> delta;
noa::Vector<double>& noa::Input::GetMouseMoveDelta()
{

	delta = e->mouseContext->delta;
	e->mouseContext->delta = { 0,0 };
	return delta;

}

noa::Vector<double>& noa::Input::GetMousePosition()
{
	return e->mouseContext->position;
}


noa::Vector<double> wheel;
noa::Vector<double>& noa::Input::GetMouseWheel()
{

	wheel = e->mouseContext->wheel;
	e->mouseContext->wheel = { 0,0 };
	return wheel;
}

bool noa::Input::GetMouseKeyDown(MouseButton mouseButton)
{
	return e->mouseContext->mouseKey[mouseButton].performed;
}

bool noa::Input::GetMouseKeyDownOnce(MouseButton mouseButton)
{
	return e->mouseContext->mouseKey[mouseButton].wasPerformedOnce;
}

bool noa::Input::GetMouseKeyUp(MouseButton mouseButton)
{
	return e->mouseContext->mouseKey[mouseButton].canceled;
}


