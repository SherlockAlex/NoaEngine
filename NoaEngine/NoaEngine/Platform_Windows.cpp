#if defined(_WIN64)||defined(_WIN32)

#include "Platform_Windows.h"
#include "Debug.h"
#include "Screen.h"
#include "InputSystem.h"
#include <windows.h>

noa::Platform_Windows::Platform_Windows():noa::Platform()
{
	const int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		Debug::Error("Game init failed");
		exit(-1);
	}

}

noa::Platform_Windows::~Platform_Windows() {
	SDL_DestroyWindow(window);
}

int noa::Platform_Windows::Create(
	int width, int height
	, WindowMode windowMode
	, const std::string& gameName
)
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
	this->ResetMouseContext();
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

void noa::InputEvent_Windows::ResetMouseContext()
{
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
}

void noa::InputEvent_Windows::UpdateMouseContext()
{

	int mouseX, mouseY;

	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	mouseContext->position.x = mouseX;
	mouseContext->position.y = mouseY;

	mouseContext->mouseKey[MouseButton::LEFT_BUTTON].hold
		= mouseState & SDL_BUTTON((static_cast<int>(MouseButton::LEFT_BUTTON)));
	mouseContext->mouseKey[MouseButton::RIGHT_BUTTON].hold
		= mouseState & SDL_BUTTON((static_cast<int>(MouseButton::RIGHT_BUTTON)));
	mouseContext->mouseKey[MouseButton::MIDDLE_BUTTON].hold
		= mouseState & SDL_BUTTON((static_cast<int>(MouseButton::MIDDLE_BUTTON)));

	mouseContext->delta = { 0,0 };
	mouseContext->wheel = { 0,0 };

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
		break;
	}

}

void noa::InputEvent_Windows::SetRelativeMouseMode(bool mode)
{
	SDL_SetRelativeMouseMode(static_cast<SDL_bool>(mode));
}


#endif //_WIN64

