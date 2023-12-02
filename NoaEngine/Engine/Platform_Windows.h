#if defined(NOA_WINDOWS)

#ifndef NOAENGINE_PLATFORM_WINDOWS_H
#define NOAENGINE_PLATFORM_WINDOWS_H

#include "Core.h"
#include "Platform.h"
#include "SDLHelper.h"


namespace noa {
	class NOA_API Platform_Windows:public Platform
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

	class NOA_API InputEvent_Windows :public InputEvent
	{
	public:
		InputEvent_Windows();
		~InputEvent_Windows();

		bool GetKeyDown(KeyCode key) override;
		bool GetKeyDownOnce(KeyCode key) override;
		bool GetKeyUp(KeyCode key) override;
		
		void* GetEventEntity() override;
		void PollEvent(const std::function<void()>& quitCallback) override;
		
		void ResetKeyboardContext() override;
		void UpdateKeyboardContext() override;
		
		void ResetMouseContext() override;
		void UpdateMouseContext() override;
		
		void SetRelativeMouseMode(bool mode) override;
	};

}

#endif // !NOAENGINE_PLATFORM_WINDOWS_H

#endif // _WIN64