#ifdef __linux__

#ifndef NOAENGINE_PLATFORM_LINUX_H
#define NOAENGINE_PLATFORM_LINUX_H

#include "Platform.h"
#include "Core.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace noa {
	class NOA_API Platform_Linux :public Platform
	{
	private:
		SDL_Window* window = nullptr;

	public:

		Platform_Linux();
		~Platform_Linux();

		//创建窗口
		int Create(int width, int height,
				WindowMode windowMode,
				const std::string& gameName) override;

		void EventLoop() override;

		//确定窗口关闭
		bool CheckWindowClose() override;

	};

	class NOA_API InputEvent_Linux :public InputEvent {
	private:
		SDL_Event e = {};
	public:
		InputEvent_Linux();
		~InputEvent_Linux();

		bool GetKeyHold(KeyCode key) override;
		bool GetKeyDown(KeyCode key) override;
		void PollEvent(const std::function<void()>& quitCallback) override;
		void UpdateMouseContext() override;
		void SetRelativeMouseMode(bool mode) override;
	};

}

#endif // !NOAENGINE_PLATFORM_WINDOWS_H

#endif // _WIN64