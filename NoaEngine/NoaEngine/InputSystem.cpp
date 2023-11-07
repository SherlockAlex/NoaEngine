#include "InputSystem.h"
#include "Platform.h"
#include "Debug.h"

#include <iostream>

using namespace std;

std::shared_ptr<noa::InputEvent> noa::Input::e = nullptr;

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

bool noa::Input::GetKeyHold(noa::KeyCode key) {

	return e->GetKeyHold(key);
}

bool noa::Input::GetKeyDown(noa::KeyCode key)
{
	return e->GetKeyDown(key);
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
	return e->mouseContext->mouseKey[mouseButton].down;
}

bool noa::Input::GetMouseKeyHold(noa::MouseButton mouseButton)
{
	return e->mouseContext->mouseKey[mouseButton].hold;
}

bool noa::Input::GetMouseKeyUp(MouseButton mouseButton)
{
	return e->mouseContext->mouseKey[mouseButton].up;
}


