#ifndef NOAENGINE_PLATFORM
#define NOAENGINE_PLATFORM


#include <string>
#include <functional>
#include <chrono>
#include <memory>

#include "NoaAction.h"

namespace noa {

	//窗口类型
	enum class WindowMode
	{
		FULLSCREEN = 1,
		WINDOW = 2048
	};

	class InputEvent;
	//平台抽象类
	class Platform {
	public:
		Platform();
		virtual ~Platform();
		virtual int Create(int width, int height,
				WindowMode windowMode,
				const std::string & gameName) = 0;
		std::shared_ptr<InputEvent> GetPlatformEvent();
		virtual bool CheckWindowClose() = 0;
		virtual void EventLoop() = 0;
		void Quit();
		uint32_t GetWindowID();

	protected:
		uint32_t windowID = 0;
		bool isRun = true;
		std::shared_ptr<InputEvent> e = nullptr;

	};

	enum class KeyCode; // 前置声明
	struct MouseContext;
	struct KeyboardContext;
	// 平台输入事件类的抽象
	class InputEvent
	{
	private:
		friend class Input;
	
	public:
		InputEvent();
		virtual ~InputEvent();

		virtual bool GetKeyDown(noa::KeyCode key) = 0;
		virtual bool GetKeyUp(noa::KeyCode key) = 0;

		virtual void* GetEventEntity() = 0;
		virtual void PollEvent(const std::function<void()> & quitCallback) = 0;
		void AddPollEventCallback(std::function<void()> callback);

		virtual void ResetKeyboardContext() = 0;
		virtual void UpdateKeyboardContext() = 0;

		virtual void ResetMouseContext() = 0;
		virtual void UpdateMouseContext() = 0;
		
		virtual void SetRelativeMouseMode(bool mode) = 0;
	protected:
		std::shared_ptr<MouseContext> mouseContext = nullptr;
		std::shared_ptr<KeyboardContext> keyboardContext = nullptr;

		noa::NoaEvent<void> eventPollAction;

	};

	extern std::shared_ptr<noa::Platform> platform;

}


#endif

