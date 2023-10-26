#include "Platform.h"
#include "Debug.h"

namespace noa {

	extern std::shared_ptr<Renderer> renderer;

}

noa::Platform::Platform()
{
	
}

noa::Platform::~Platform() {
	
}

void noa::Platform::Quit()
{
	isRun = false;
}

uint32_t noa::Platform::GetWindowID()
{
	return this->windowID;
}


