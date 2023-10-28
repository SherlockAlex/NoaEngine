#include "InputSystem.h"
#include "Platform.h"
#include "Debug.h"

#include <iostream>

using namespace std;

namespace noa {
	InputSystem inputSystem;
}

void noa::InputSystem::InitInputSystem(std::shared_ptr<noa::InputEvent> e)
{
	//初始化InputSystem
	if (e == nullptr)
	{
		Debug::Error("Init Input subsystem error");
		Debug::Error("This InputEvent is null");
		exit(-1);
	}
	this->e = e;

}

noa::InputSystem::InputSystem() {

}

noa::InputSystem::~InputSystem()
{

}

bool noa::InputSystem::GetKeyHold(noa::KeyCode key) {

	return this->e->GetKeyHold(key);
}

bool noa::InputSystem::GetKeyDown(noa::KeyCode key)
{
	return this->e->GetKeyDown(key);
}

bool noa::InputSystem::GetMouseMoveState()
{
	return this->e->mouseContext->motion;
}

void noa::InputSystem::SetRelativeMouseMode(bool mode)
{
	//设置鼠标相对模式
	this->e->SetRelativeMouseMode(mode);
}

noa::Vector<double> delta;
noa::Vector<double>& noa::InputSystem::GetMouseMoveDelta()
{

	delta = this->e->mouseContext->delta;
	this->e->mouseContext->delta = { 0,0 };
	return delta;

}

noa::Vector<double>& noa::InputSystem::GetMousePosition()
{
	return this->e->mouseContext->position;
}


noa::Vector<double> wheel;
noa::Vector<double>& noa::InputSystem::GetMouseWheel()
{

	wheel = this->e->mouseContext->wheel;
	this->e->mouseContext->wheel = { 0,0 };
	return wheel;
}

bool noa::InputSystem::GetMouseKeyDown(MouseButton mouseButton)
{
	return this->e->mouseContext->mouseKey[mouseButton].down;
}

bool noa::InputSystem::GetMouseKeyHold(noa::MouseButton mouseButton)
{
	return this->e->mouseContext->mouseKey[mouseButton].hold;
}

bool noa::InputSystem::GetMouseKeyUp(MouseButton mouseButton)
{
	return this->e->mouseContext->mouseKey[mouseButton].up;
}


