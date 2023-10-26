#ifdef _WIN64

#include "Platform_Windows.h"
#include "Debug.h"
#include "Screen.h"
#include "InputSystem.h"
#include "Audio.h"

noa::Platform_Windows::Platform_Windows():noa::Platform()
{
	const int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		Debug::Error("Game init failed");
		exit(-1);
	}

	//初始化声音子系统
	if (Mix_OpenAudio(
		MIX_DEFAULT_FREQUENCY,
		MIX_DEFAULT_FORMAT,
		MIX_CHANNELS,
		4096
	) == -1)
	{
		//在一些没有声卡的设备中，将会初始化失败
		Debug::Error("Init audio device failed");
		Audio::isInitSuccessful = false;
	}
	else {
		Audio::isInitSuccessful = true;
	}

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

	//设置WindowID
	this->windowID = SDL_GetWindowID(window);

	//创建Event
	this->e = std::make_shared<InputEvent_Windows>();

	return 0;
}

void noa::Platform_Windows::EventLoop()
{

	//实现Windows系统写的事件循环
	// 下面对于SDL事件的封装要进行，不然没有办法跨平台
	this->e->PollEvent([this]() {Quit(); });
	
}

bool noa::Platform_Windows::CheckWindowClose()
{
	return !isRun;
}

noa::InputEvent_Windows::InputEvent_Windows() :InputEvent()
{

}

noa::InputEvent_Windows::~InputEvent_Windows()
{

}

bool noa::InputEvent_Windows::GetKeyHold(KeyCode key)
{
	return GetAsyncKeyState((unsigned short)key) & 0x8000;
}

bool noa::InputEvent_Windows::GetKeyDown(KeyCode key)
{
	return GetAsyncKeyState((unsigned short)key) & 1;
}

void noa::InputEvent_Windows::PollEvent(const std::function<void()>& quitCallback)
{
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			//退出
			quitCallback();
			break;
		default:
			this->Update();
			break;
		}
	}
}

void noa::InputEvent_Windows::Update()
{
	mouseContext->mouseKey[static_cast<noa::MouseButton>(e.button.button)].down = (e.type == SDL_MOUSEBUTTONDOWN);
	mouseContext->mouseKey[static_cast<noa::MouseButton>(e.button.button)].up = (e.type == SDL_MOUSEBUTTONUP);
	{
		int mouseX, mouseY;
		const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

		mouseContext->mouseKey[MouseButton::LEFT_BUTTON].hold = mouseState & SDL_BUTTON((static_cast<int>(MouseButton::LEFT_BUTTON)));
		mouseContext->mouseKey[MouseButton::RIGHT_BUTTON].hold = mouseState & SDL_BUTTON((static_cast<int>(MouseButton::RIGHT_BUTTON)));
		mouseContext->mouseKey[MouseButton::MIDDLE_BUTTON].hold = mouseState & SDL_BUTTON((static_cast<int>(MouseButton::MIDDLE_BUTTON)));
	}


	if (e.type == SDL_MOUSEMOTION)
	{
		mouseContext->delta.x += static_cast<double>(e.motion.xrel);
		mouseContext->delta.y += static_cast<double>(e.motion.yrel);
	}
	else {
		mouseContext->delta = { 0,0 };
	}

	mouseContext->motion = (e.type == SDL_MOUSEMOTION);

	if (e.type != SDL_MOUSEWHEEL)
	{
		mouseContext->wheel = { 0,0 };
	}
	else {
		mouseContext->wheel.x = e.wheel.x;
		mouseContext->wheel.y = e.wheel.y;
	}

	mouseContext->position.x = e.motion.x;
	mouseContext->position.y = e.motion.y;

}

void noa::InputEvent_Windows::SetRelativeMouseMode(bool mode)
{
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(mode));
}


#endif //_WIN64

