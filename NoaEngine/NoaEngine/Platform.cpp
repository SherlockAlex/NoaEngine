#include "Platform.h"
#include "Debug.h"
#include "InputSystem.h"

namespace noa {

	extern std::shared_ptr<Renderer> renderer;

	

}

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
}

noa::InputEvent::~InputEvent() {

}


