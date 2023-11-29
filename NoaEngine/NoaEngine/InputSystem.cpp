#include "InputSystem.h"
#include "Platform.h"
#include "Debug.h"

#include <iostream>

using namespace std;

std::shared_ptr<noa::InputEvent> noa::Input::e = nullptr;

noa::KeyboardContext::KeyboardContext() 
{
	this->keyMap[KeyCode::KEY_0] = { false,false };
	this->keyMap[KeyCode::KEY_1] = { false,false };
	this->keyMap[KeyCode::KEY_2] = { false,false };
	this->keyMap[KeyCode::KEY_3] = { false,false };
	this->keyMap[KeyCode::KEY_4] = { false,false };
	this->keyMap[KeyCode::KEY_5] = { false,false };
	this->keyMap[KeyCode::KEY_6] = { false,false };
	this->keyMap[KeyCode::KEY_7] = { false,false };
	this->keyMap[KeyCode::KEY_8] = { false,false };
	this->keyMap[KeyCode::KEY_9] = { false,false };

	this->keyMap[KeyCode::KEY_A] = { false,false };
	this->keyMap[KeyCode::KEY_B] = { false,false };
	this->keyMap[KeyCode::KEY_C] = { false,false };
	this->keyMap[KeyCode::KEY_D] = { false,false };
	this->keyMap[KeyCode::KEY_E] = { false,false };
	this->keyMap[KeyCode::KEY_F] = { false,false };
	this->keyMap[KeyCode::KEY_G] = { false,false };
	this->keyMap[KeyCode::KEY_H] = { false,false };
	this->keyMap[KeyCode::KEY_I] = { false,false };
	this->keyMap[KeyCode::KEY_J] = { false,false };
	this->keyMap[KeyCode::KEY_K] = { false,false };
	this->keyMap[KeyCode::KEY_L] = { false,false };
	this->keyMap[KeyCode::KEY_M] = { false,false };
	this->keyMap[KeyCode::KEY_N] = { false,false };
	this->keyMap[KeyCode::KEY_O] = { false,false };
	this->keyMap[KeyCode::KEY_P] = { false,false };
	this->keyMap[KeyCode::KEY_Q] = { false,false };
	this->keyMap[KeyCode::KEY_R] = { false,false };
	this->keyMap[KeyCode::KEY_S] = { false,false };
	this->keyMap[KeyCode::KEY_T] = { false,false };
	this->keyMap[KeyCode::KEY_U] = { false,false };
	this->keyMap[KeyCode::KEY_V] = { false,false };
	this->keyMap[KeyCode::KEY_W] = { false,false };
	this->keyMap[KeyCode::KEY_X] = { false,false };
	this->keyMap[KeyCode::KEY_Y] = { false,false };
	this->keyMap[KeyCode::KEY_Z] = { false,false };

	this->keyMap[KeyCode::KEY_SPACE] = { false,false };
	
	this->keyMap[KeyCode::KEY_ESC] = { false,false };
	this->keyMap[KeyCode::KEY_TAB] = { false,false };
	this->keyMap[KeyCode::KEY_CAPSLOCK] = { false,false };

	this->keyMap[KeyCode::KEY_LSHIFT] = { false,false };
	this->keyMap[KeyCode::KEY_RSHIFT] = { false,false };

	this->keyMap[KeyCode::KEY_ALT] = { false,false };
	this->keyMap[KeyCode::KEY_DEL] = { false,false };

	this->keyMap[KeyCode::KEY_F1] = { false,false };
	this->keyMap[KeyCode::KEY_F2] = { false,false };
	this->keyMap[KeyCode::KEY_F3] = { false,false };
	this->keyMap[KeyCode::KEY_F4] = { false,false };
	this->keyMap[KeyCode::KEY_F5] = { false,false };
	this->keyMap[KeyCode::KEY_F6] = { false,false };
	this->keyMap[KeyCode::KEY_F7] = { false,false };
	this->keyMap[KeyCode::KEY_F8] = { false,false };
	this->keyMap[KeyCode::KEY_F9] = { false,false };
	this->keyMap[KeyCode::KEY_F10] = { false,false };
	this->keyMap[KeyCode::KEY_F11] = { false,false };
	this->keyMap[KeyCode::KEY_F12] = { false,false };

	this->keyMap[KeyCode::KEY_LSHIFT] = { false,false };
	this->keyMap[KeyCode::KEY_RSHIFT] = { false,false };

	this->keyMap[KeyCode::KEY_COMMA] = { false,false };
	
}

noa::MouseContext::MouseContext() 
{
	this->mouseKey[noa::MouseButton::LEFT_BUTTON] = { false,false };
	this->mouseKey[noa::MouseButton::MIDDLE_BUTTON] = { false,false };
	this->mouseKey[noa::MouseButton::RIGHT_BUTTON] = { false,false };
}

void noa::Input::InitInputSystem(std::shared_ptr<noa::InputEvent> e)
{
	//初始化InputSystem
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
	//设置鼠标相对模式
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


bool noa::Input::GetMouseKeyUp(MouseButton mouseButton)
{
	return e->mouseContext->mouseKey[mouseButton].canceled;
}


