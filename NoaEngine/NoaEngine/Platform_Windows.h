#if defined(_WIN64)||defined(_WIN32)

#ifndef NOAENGINE_PLATFORM_WINDOWS_H
#define NOAENGINE_PLATFORM_WINDOWS_H

#include "Platform.h"
#include "SDLHelper.h"


namespace noa {
	class Platform_Windows:public Platform
	{
	public:
		Platform_Windows();
		~Platform_Windows();
		int Create(int width, int height,
				WindowMode windowMode,
				const std::string& gameName) override;
		void EventLoop() override;
		bool CheckWindowClose() override;
	private:
		SDL_Window* window = nullptr;

	};

	class InputEvent_Windows :public InputEvent 
	{
	public:
		InputEvent_Windows();
		~InputEvent_Windows();

		bool GetKeyHold(KeyCode key) override;
		bool GetKeyDown(KeyCode key) override;
		void PollEvent(const std::function<void()>& quitCallback) override;
		void ResetMouseContext() override;
		void UpdateMouseContext() override;
		void SetRelativeMouseMode(bool mode) override;
	};

}

#endif // !NOAENGINE_PLATFORM_WINDOWS_H

#endif // _WIN64