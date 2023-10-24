#include "Platform.h"
#include "Debug.h"

namespace noa {

	extern std::shared_ptr<Renderer> renderer;

}

noa::Platform::Platform()
{
	int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		Debug::Error("Game init failed");
		exit(-1);
	}
}

noa::Platform::~Platform() {
	SDL_DestroyWindow(windows);
}

void noa::Platform::Quit()
{
	isRun = false;
}

SDL_Window* noa::Platform::GetWindow()
{
	return this->windows;
}


