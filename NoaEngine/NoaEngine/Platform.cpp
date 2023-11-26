#include "Platform.h"
#include "Debug.h"
#include "InputSystem.h"

#include "Renderer.h"

extern std::shared_ptr<noa::Renderer> noa::renderer;

noa::Platform::Platform()
{
	
}

noa::Platform::~Platform() {
	
}

std::shared_ptr<noa::InputEvent> noa::Platform::GetPlatformEvent()
{
	return this->e;
}

void noa::Platform::Quit()
{
	isRun = false;
}

uint32_t noa::Platform::GetWindowID()
{
	return this->windowID;
}

noa::InputEvent::InputEvent()
{
	this->mouseContext = std::make_shared<MouseContext>();
	this->keyboardContext = std::make_shared<KeyboardContext>();
}

noa::InputEvent::~InputEvent() {

}


