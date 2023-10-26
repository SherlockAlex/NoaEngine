#ifdef _WIN64

#include "Platform_Windows.h"
#include "Debug.h"
#include "Screen.h"
#include "InputSystem.h"
#include "Audio.h"

void noa::Platform_Windows::UpdateInputSystem()
{
	

}

noa::Platform_Windows::Platform_Windows():noa::Platform()
{
	const int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		Debug::Error("Game init failed");
		exit(-1);
	}

	//��ʼ��������ϵͳ
	if (Mix_OpenAudio(
		MIX_DEFAULT_FREQUENCY,
		MIX_DEFAULT_FORMAT,
		MIX_CHANNELS,
		4096
	) == -1)
	{
		//��һЩû���������豸�У������ʼ��ʧ��
		Debug::Error("Init audio device failed");
		Audio::isInitSuccessful = false;
	}
	else {
		Audio::isInitSuccessful = true;
	}

	SDL_PollEvent(&e);

}

noa::Platform_Windows::~Platform_Windows() {
	SDL_DestroyWindow(this->window);
}

int noa::Platform_Windows::Create(int width, int height, WindowMode windowMode, std::string gameName)
{
	window = SDL_CreateWindow(
			gameName.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			static_cast<uint32_t>(windowMode)|SDL_WINDOW_OPENGL
	);

	if (window == nullptr)
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

	//����WindowID
	this->windowID = SDL_GetWindowID(window);

	return 0;
}

void noa::Platform_Windows::EventLoop()
{

	//ʵ��Windowsϵͳд���¼�ѭ��
	// �������SDL�¼��ķ�װҪ���У���Ȼû�а취��ƽ̨
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			//�˳�
			Quit();
			break;
		default:
			inputSystem.Update(e);
			break;
		}
	}
}

bool noa::Platform_Windows::CheckWindowClose()
{
	return !isRun;
}

#endif //_WIN64