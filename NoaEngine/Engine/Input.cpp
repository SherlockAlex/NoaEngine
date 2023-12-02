#include "Input.h"
#include "Platform.h"
#include "Debug.h"

#include <iostream>

std::shared_ptr<noa::InputEvent> noa::Input::ioEvent = nullptr;

void noa::Input::InitInputSystem(std::shared_ptr<noa::InputEvent> e)
{
	//初始化InputSystem
	if (e == nullptr)
	{
		Debug::Error("Init Input subsystem error");
		Debug::Error("This InputEvent is null");
		exit(-1);
	}
	Input::ioEvent = e;

}

bool noa::Input::GetKeyDown(noa::KeyCode key)
{
	return ioEvent->GetKeyDown(key);
}

bool noa::Input::GetKeyDownOnce(noa::KeyCode key)
{
	return ioEvent->GetKeyDownOnce(key);
}

bool noa::Input::GetKeyUp(noa::KeyCode key) 
{
	return ioEvent->GetKeyUp(key);
}

bool noa::Input::GetMouseMoveState()
{
	return ioEvent->mouseContext->motion;
}

void noa::Input::SetRelativeMouseMode(bool mode)
{
	//设置鼠标相对模式
	ioEvent->SetRelativeMouseMode(mode);
}

noa::Vector<double> delta;
noa::Vector<double>& noa::Input::GetMouseMoveDelta()
{

	delta = ioEvent->mouseContext->delta;
	ioEvent->mouseContext->delta = { 0,0 };
	return delta;

}

noa::Vector<double>& noa::Input::GetMousePosition()
{
	return ioEvent->mouseContext->position;
}


noa::Vector<double> wheel;
noa::Vector<double>& noa::Input::GetMouseWheel()
{

	wheel = ioEvent->mouseContext->wheel;
	ioEvent->mouseContext->wheel = { 0,0 };
	return wheel;
}

bool noa::Input::GetMouseKeyDown(MouseButton mouseButton)
{
	return ioEvent->mouseContext->mouseKey[mouseButton].performed;
}

bool noa::Input::GetMouseKeyDownOnce(MouseButton mouseButton)
{
	return ioEvent->mouseContext->mouseKey[mouseButton].wasPerformedOnce;
}

bool noa::Input::GetMouseKeyUp(MouseButton mouseButton)
{
	return ioEvent->mouseContext->mouseKey[mouseButton].canceled;
}


