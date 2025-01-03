#ifdef __linux__

#include "Platform_Linux.h"

#include "Debug.h"
#include "Screen.h"
#include "InputSystem.h"
#include "Audio.h"

Display* display = nullptr;

noa::Platform_Linux::Platform_Linux() :noa::Platform()
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

noa::Platform_Linux::~Platform_Linux() {
	SDL_DestroyWindow(this->window);
}

int noa::Platform_Linux::Create(
	int width, int height
	, WindowMode windowMode
	, const std::string& gameName)
{
	window = SDL_CreateWindow(
			gameName.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			static_cast<uint32_t>(windowMode) | SDL_WINDOW_OPENGL
	);

	if (window == nullptr)
	{
		noa::Debug::Error("Create window failed");
		exit(-1);
	}

	SDL_DisplayMode displayMode;
	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
	{
		noa::Debug::Error("Unable to get display mode: " + std::string(SDL_GetError()));
		exit(-1);
	}

	Screen::hardwareScreenScale.x = displayMode.w;
	Screen::hardwareScreenScale.y = displayMode.h;

	//设置WindowID
	this->windowID = SDL_GetWindowID(window);

	//创建Event
	this->e = std::make_shared<InputEvent_Linux>();

	return 0;
}

void noa::Platform_Linux::EventLoop()
{

	//实现Windows系统写的事件循环
	// 下面对于SDL事件的封装要进行，不然没有办法跨平台
	this->e->PollEvent([this]() {Quit(); });

}

bool noa::Platform_Linux::CheckWindowClose()
{
	return !isRun;
}



noa::InputEvent_Linux::InputEvent_Linux() :InputEvent()
{
	display = XOpenDisplay(nullptr);
}

noa::InputEvent_Linux::~InputEvent_Linux()
{
	XCloseDisplay(display);
}

bool noa::InputEvent_Linux::GetKeyHold(noa::KeyCode key)
{
	KeyCode keyCode = XKeysymToKeycode(display, (key));
	char keys_return[32] = { 0 };
	XQueryKeymap(display, keys_return);
	return !!(keys_return[keyCode / 8] & (1 << (keyCode % 8)));
}

bool noa::InputEvent_Linux::GetKeyDown(noa::KeyCode key)
{
	KeyCode keyCode = XKeysymToKeycode(display, (key));
	char keys_return[32] = { 0 };
	XQueryKeymap(display, keys_return);
	return !!(keys_return[keyCode / 8] & (1 << (keyCode % 8)));
}

void noa::InputEvent_Linux::PollEvent(const std::function<void()>& quitCallback)
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
			this->UpdateMouseContext();
			break;
		}
	}
}

void noa::InputEvent_Linux::UpdateMouseContext()
{
	const Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);

	mouseContext->mouseKey[noa::MouseButton::LEFT_BUTTON].down = false;
	mouseContext->mouseKey[noa::MouseButton::LEFT_BUTTON].hold = false;
	mouseContext->mouseKey[noa::MouseButton::LEFT_BUTTON].up = false;

	mouseContext->mouseKey[noa::MouseButton::MIDDLE_BUTTON].down = false;
	mouseContext->mouseKey[noa::MouseButton::MIDDLE_BUTTON].hold = false;
	mouseContext->mouseKey[noa::MouseButton::MIDDLE_BUTTON].up = false;

	mouseContext->mouseKey[noa::MouseButton::RIGHT_BUTTON].down = false;
	mouseContext->mouseKey[noa::MouseButton::RIGHT_BUTTON].hold = false;
	mouseContext->mouseKey[noa::MouseButton::RIGHT_BUTTON].up = false;
	mouseContext->motion = false;

	mouseContext->position.x = e.motion.x;
	mouseContext->position.y = e.motion.y;

	mouseContext->mouseKey[MouseButton::LEFT_BUTTON].hold
		= mouseState & SDL_BUTTON((static_cast<int>(MouseButton::LEFT_BUTTON)));
	mouseContext->mouseKey[MouseButton::RIGHT_BUTTON].hold
		= mouseState & SDL_BUTTON((static_cast<int>(MouseButton::RIGHT_BUTTON)));
	mouseContext->mouseKey[MouseButton::MIDDLE_BUTTON].hold
		= mouseState & SDL_BUTTON((static_cast<int>(MouseButton::MIDDLE_BUTTON)));

	switch (e.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		mouseContext->mouseKey[static_cast<noa::MouseButton>(e.button.button)].down
			= true;
		break;
	case SDL_MOUSEBUTTONUP:
		mouseContext->mouseKey[static_cast<noa::MouseButton>(e.button.button)].up
			= true;
		break;
	case SDL_MOUSEMOTION:
		mouseContext->motion = true;
		mouseContext->delta.x += static_cast<double>(e.motion.xrel);
		mouseContext->delta.y += static_cast<double>(e.motion.yrel);
		break;
	case SDL_MOUSEWHEEL:
		mouseContext->wheel.x = e.wheel.x;
		mouseContext->wheel.y = e.wheel.y;
		break;
	default:

		mouseContext->delta = { 0,0 };
		mouseContext->wheel = { 0,0 };
		break;
	}

}

void noa::InputEvent_Linux::SetRelativeMouseMode(bool mode)
{
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(mode));
}

#endif //_WIN64
