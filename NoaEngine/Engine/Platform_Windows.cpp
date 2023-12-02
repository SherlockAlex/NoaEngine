#if defined(NOA_WINDOWS)

#include "Platform_Windows.h"
#include "Debug.h"
#include "Screen.h"
#include "Input.h"
#include <windows.h>

noa::Platform_Windows::Platform_Windows():noa::Platform()
{
	const int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		Debug::Error("Game init failed");
		exit(-1);
	}

	//创建游戏窗口
	SDL_DisplayMode displayMode;
	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0)
	{
		noa::Debug::Error("Unable to get display mode: " + std::string(SDL_GetError()));
		exit(-1);
	}

	hardwareScreenSize.x = displayMode.w;
	hardwareScreenSize.y = displayMode.h;

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
			static_cast<uint32_t>(windowMode) | SDL_WINDOW_OPENGL
	);

	if (window == nullptr)
	{
		noa::Debug::Error("Create window failed");
		exit(-1);
	}

	this->windowID = SDL_GetWindowID(window);

	this->ioEvent = std::make_shared<InputEvent_Windows>();

	return 0;
}

void noa::Platform_Windows::EventLoop()
{
	this->ioEvent->PollEvent([this]() {Quit(); });
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

bool noa::InputEvent_Windows::GetKeyDown(KeyCode key)
{
	return keyboardContext->keyMap[key].performed;
}

bool noa::InputEvent_Windows::GetKeyDownOnce(KeyCode key)
{
	return keyboardContext->keyMap[key].wasPerformedOnce;
}

bool noa::InputEvent_Windows::GetKeyUp(KeyCode key)
{
	
	return keyboardContext->keyMap[key].canceled;
}

static SDL_Event ioEvent = {};

void* noa::InputEvent_Windows::GetEventEntity() {
	return &ioEvent;
}

void noa::InputEvent_Windows::PollEvent(const std::function<void()>& quitCallback)
{
	ResetKeyboardContext();
	ResetMouseContext();
	while (SDL_PollEvent(&ioEvent))
	{
		eventPollAction();
		switch (ioEvent.type)
		{
		case SDL_QUIT:
			//退出
			quitCallback();
			break;
		default:
			this->UpdateKeyboardContext();
			this->UpdateMouseContext();
			break;
		}
	}

}

void noa::InputEvent_Windows::ResetKeyboardContext() {

	for (auto& state:keyboardContext->keyMap)
	{
		state.second.wasPerformedOnce = false;
		state.second.canceled = false;
	}
}

void noa::InputEvent_Windows::UpdateKeyboardContext() 
{

	switch (ioEvent.type)
	{
	case SDL_KEYDOWN:
		keyboardContext->keyMap[
			static_cast<noa::KeyCode>(ioEvent.key.keysym.sym)
		].performed = true;

		if (keyboardContext->keyMap[
			static_cast<noa::KeyCode>(ioEvent.key.keysym.sym)
		].wasReleased)
		{
			keyboardContext->keyMap[
				static_cast<noa::KeyCode>(ioEvent.key.keysym.sym)
			].wasPerformedOnce = true;
			keyboardContext->keyMap[
				static_cast<noa::KeyCode>(ioEvent.key.keysym.sym)
			].wasReleased = false;
		}
		
		break;
	case SDL_KEYUP:
		keyboardContext->keyMap[
			static_cast<noa::KeyCode>(ioEvent.key.keysym.sym)
		].performed = false;
		keyboardContext->keyMap[
			static_cast<noa::KeyCode>(ioEvent.key.keysym.sym)
		].canceled = true;
		keyboardContext->keyMap[
			static_cast<noa::KeyCode>(ioEvent.key.keysym.sym)
		].wasReleased = true;
		break;
	default:
		break;
	}
}

void noa::InputEvent_Windows::ResetMouseContext()
{

	mouseContext->delta = { 0,0 };
	mouseContext->wheel = { 0,0 };

	mouseContext->mouseKey[noa::MouseButton::LEFT_BUTTON].canceled = false;
	mouseContext->mouseKey[noa::MouseButton::MIDDLE_BUTTON].canceled = false;
	mouseContext->mouseKey[noa::MouseButton::RIGHT_BUTTON].canceled = false;

	mouseContext->mouseKey[noa::MouseButton::LEFT_BUTTON].wasPerformedOnce = false;
	mouseContext->mouseKey[noa::MouseButton::MIDDLE_BUTTON].wasPerformedOnce = false;
	mouseContext->mouseKey[noa::MouseButton::RIGHT_BUTTON].wasPerformedOnce = false;

	mouseContext->motion = false;

	

}

void noa::InputEvent_Windows::UpdateMouseContext()
{
	switch (ioEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		mouseContext->mouseKey[
			static_cast<noa::MouseButton>(ioEvent.button.button)
		].performed = true;
		mouseContext->mouseKey[
			static_cast<noa::MouseButton>(ioEvent.button.button)
		].wasPerformedOnce = true;
		break;
	case SDL_MOUSEBUTTONUP:
		mouseContext->mouseKey[
			static_cast<noa::MouseButton>(ioEvent.button.button)
		].performed = false;
		mouseContext->mouseKey[
			static_cast<noa::MouseButton>(ioEvent.button.button)
		].canceled = true;
		break;
	case SDL_MOUSEMOTION:
		mouseContext->motion = true;
		mouseContext->position.x = ioEvent.motion.x;
		mouseContext->position.y = ioEvent.motion.y;
		mouseContext->delta.x += static_cast<double>(ioEvent.motion.xrel);
		mouseContext->delta.y += static_cast<double>(ioEvent.motion.yrel);
		break;
	case SDL_MOUSEWHEEL:
		mouseContext->wheel.x = ioEvent.wheel.x;
		mouseContext->wheel.y = ioEvent.wheel.y;
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

