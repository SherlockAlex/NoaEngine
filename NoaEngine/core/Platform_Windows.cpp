#ifdef _WIN64

#include "Platform_Windows.h"
#include "NoaEngine.h"


noa::Platform_Windows::Platform_Windows():Platform()
{
}

int noa::Platform_Windows::Create(int width, int height, WINDOWMODE windowMode, std::string gameName)
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
		noa::Debug("Create window faild");
		exit(-1);
	}

	return 0;
}

bool noa::Platform_Windows::CheckWindowClose()
{
	return !isRun;
}

#endif //_WIN64