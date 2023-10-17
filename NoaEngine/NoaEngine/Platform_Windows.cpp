#ifdef _WIN64

#include "Platform_Windows.h"
#include "Debug.h"
#include "Screen.h"

noa::Platform_Windows::Platform_Windows():noa::Platform()
{

}

noa::Platform_Windows::~Platform_Windows() {

}

int noa::Platform_Windows::Create(int width, int height, WindowMode windowMode, std::string gameName)
{
	windows = SDL_CreateWindow(
			gameName.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			((uint32_t)windowMode)|SDL_WINDOW_OPENGL
	);

	if (windows == nullptr)
	{
		noa::Debug::Error("Create window failed");
		exit(-1);
	}

	SDL_DisplayMode displayMode;
	if (SDL_GetDesktopDisplayMode(0,&displayMode)!=0) 
	{
		noa::Debug::Error("Unable to get display mode: " + std::string(SDL_GetError()));
		exit(-1);
	}

	Screen::hardwareScreenScale.x = displayMode.w;
	Screen::hardwareScreenScale.y = displayMode.h;

	return 0;
}

bool noa::Platform_Windows::CheckWindowClose()
{
	return !isRun;
}

#endif //_WIN64